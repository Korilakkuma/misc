mod utils;

use std::convert::{TryFrom, TryInfo};
use std::sync::Arc;

use rustfft::{
    num_complex::{Complex, ComplexFloat},
    Fft, FftPlanner,
};
use wasm_bindgen::prelude::*;

extern crate web_sys;

macro_rules! log {
    ( $( $t:tt )* ) => {
        web_sys::console::log_1(&format!( $( $t )* ).into());
    }
}

const MIN_DECIBEL: f32 = -48.0;
static mut MIN_DECIBEL_GAIN: f32 = 0.0;

struct RingBuffer<T: Clone + Copy> {
    buffer: Vec<T>,
    capacity: usize,
    front: usize,
    rear: usize,
}

impl<T: Clone + Copy> RingBuffer<T> {
    pub fn new(capacity: usize, value: T) -> RingBuffer<T> {
        RingBuffer {
            buffer: vec![value; capacity + 1],
            capacity,
            front: 0,
            rear: 0,
        }
    }

    pub fn capacity(&self) -> usize {
        self.capacity
    }

    pub fn readable_elements(&self) -> usize {
        if self.front <= self.rear {
            self.rear - self.front
        } else {
            self.rear + (self.capacity + 1) - self.front
        }
    }

    pub fn writable_elements(&self) -> usize {
        self.capacity - self.readable_elements()
    }

    pub fn is_empty(&self) -> bool {
        self.readable_elements() == 0
    }

    pub fn is_full(&self) -> bool {
        self.writable_elements() == 0
    }

    pub fn read(&mut self, dest: &mut [T]) -> bool {
        let length = dest.len();

        if self.readable_elements() < length {
            return false;
        }

        let buffer_length = self.capacity + 1;

        let copy_min = std::cmp::min((buffer_length - self.front), length);

        dest[0..copy_min].clone_from_slice(&self.buffer[self.front..(self.front + copy_min)]);

        if copy_min == length {
            return true;
        }

        let copy_max = std::cmp::max(length, copy_min) - copy_min;

        dest[copy_min..(copy_min + copy_max)].clone_from_slice(&self.buffer[0..copy_max]);

        true
    }

    pub fn write(&mut self, src: &[T]) -> bool {
        let length = src.len();

        if self.writable_elements() < length {
            return false;
        }

        let buffer_length = self.capacity + 1;

        let copy_min = std::cmp::min((buffer_length - self.rear), length);

        self.buffer[self.rear..(self.rear + copy_min)].clone_from_slice(&src[0..copy_min]);

        if copy_min == length {
            self.rear += copy_min;
            return true;
        }

        let copy_diff = length - copy_min;

        self.buffer[0..copy_diff].clone_from_slice(&src[copy_min..(copy_min + copy_diff)]);

        self.rear = copy_diff;

        true
    }

    pub fn fill(&mut self, length: usize, value: T) -> bool {
        if self.writable_elements() < length {
            return false;
        }

        let buffer_length = self.capacity + 1;

        let copy_min = std::cmp::min((buffer_length - self.rear), length);

        self.buffer[self.rear..(self.rear + copy_min)].fill(value.clone());

        if copy_min == length {
            self.rear += copy_min;
            return true;
        }

        let copy_diff = buffer_length - copy_min;

        self.buffer[0..copy_diff].fill(value.clone());
        self.rear = copy_diff;

        true
    }

    pub fn discard(&mut self, length: usize) -> bool {
        if self.readable_elements() < length {
            return false;
        }

        let buffer_length = self.capacity + 1;

        let discard_min = std::cmp::min((buffer_length - self.front), length);

        if discard_min == length {
            self.front += discard_min;
            return true;
        }

        let discard_diff = length - discard_min;

        self.front = discard_diff;

        true
    }

    pub fn discard_all(&mut self) -> bool {
        self.discard(self.readable_elements())
    }
}

trait OverlappedAddable<T> {
    fn overlap_add(&mut self, src: &[T], overlap_size: usize) -> bool;
}

impl<T> OverlappedAddable<T> for RingBuffer<T>
where
    T: std::ops::AddAssign + Clone + Copy,
{
    fn overlap_add(&mut self, src: &[T], overlap_size: usize) -> bool {
        let length = src.len();

        if length < overlap_size {
            return false;
        }

        if self.readable_elements() < overlap_size {
            return false;
        }

        let written_size = length - overlap_size;

        if self.writable_elements() < written_size {
            return false;
        }

        let buffer_length = self.capacity + 1;

        let write_start = if self.rear >= overlap_size {
            self.rear - overlap_size
        } else {
            buffer_length - (overlap_size - self.rear)
        };

        let copy_min = std::cmp::min(buffer_length - write_start, overlap_size);

        for n in 0..copy_min {
            self.buffer[write_start + n] += src[n];
        }

        if copy_min != overlap_size {
            let copy_size = overlap_size - copy_min;

            for n in 0..copy_size {
                self.buffer[n] += src[copy_min + n];
            }
        }

        self.write(&src[overlap_size..length])
    }
}

fn scale_complex(complex_numbers: &mut [Complex<f32>]) {
    if complex_numbers.len() == 0 {
        return;
    }

    let scale = complex_numbers.len() as f32;

    for c in complex_numbers.iter_mut() {
        *c = Complex::new((c.re() / scale), (c.im() / scale))
    }
}

fn has_buffer_f32(buffer: &[f32]) -> bool {
    buffer.iter().find(|x| x.is_nan()).is_none()
}

fn has_buffer_complex(buffer: &[Complex<f32>]) -> bool {
    buffer
        .iter()
        .find(|x| (x.re().is_nan() || x.im().is_nan()))
        .is_none()
}

fn decibel_to_gain(decibel: f32) -> f32 {
    (10.0f32).powf(decibel / 20.0f32)
}

fn gain_to_decibel(gain: f32, min_decibel: f32) -> f32 {
    if gain == 0.0f32 {
        return min_decibel;
    }

    let db = 20.0f32 * gain.abs().log10();

    db.max(min_decibel)
}

#[wasm_bindgen]
pub struct WasmProcessor {
    sample_rate: usize,
    block_size: usize,
    fft: Arc<dyn Fft<f32>>,
    ifft: Arc<dyn Fft<f32>>,
    fft_scratch_buffer: Vec<Complex<f32>>,
    ifft_scratch_buffer: Vec<Complex<f32>>,
    fft_size: usize,
    overlap_count: usize,
    hop_size: usize,
    input_ring_buffer: RingBuffer<f32>,
    output_ring_buffer: RingBuffer<f32>,
    signal_buffer: Vec<Complex<f32>>,
    frequency_buffer: Vec<Complex<f32>>,
    cepstrum_buffer: Vec<Complex<f32>>,
    tmp_fft_buffer_0: Vec<Complex<f32>>,
    tmp_fft_buffer_1: Vec<Complex<f32>>,
    tmp_phase_buffer: Vec<f32>,
    window: Vec<f32>,
    prev_input_phases: Vec<f32>,
    prev_output_phases: Vec<f32>,
    analysis_magnitude: Vec<f32>,
    analysis_frequencies: Vec<f32>,
    synthesis_magnitude: Vec<f32>,
    synthesis_frequencies: Vec<f32>,
    original_spectrum: Vec<Complex<f32>>,
    shifted_spectrum: Vec<Complex<f32>>,
    synthesis_spectrum: Vec<Complex<f32>>,
    original_cepstrum: Vec<Complex<f32>>,
    envelope: Vec<Complex<f32>>,
    fine_structure: Vec<Complex<f32>>,
    tmp_buffer: Vec<f32>,
    wet_buffer: Vec<f32>,
    dry_wet: f32,
    format: f32,
    pitch: f32,
    output_gain_decibel: f32,
    envelope_order: usize,
    input_level: f32,
    output_level: f32,
    level_reduction_per_sample: f32,
}

#[wasm_bindgen]
impl WasmProcessor {
    pub fn new(sample_rate: usize, block_size: usize) -> WasmProcessor {
        unsafe {
            MIN_DECIBEL_GAIN = decibel_to_gain(MIN_DECIBEL);
        };
    }
}
