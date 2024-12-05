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
    formant: f32,
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
        utils::set_panic_hook();

        unsafe {
            MIN_DECIBEL_GAIN = decibel_to_gain(MIN_DECIBEL);
        };

        let fft_size = 1024;
        let overlap_count = 4;
        let hop_size = fft_size / overlap_count;
        let overlap_size = fft_size - hop_size;

        log!(
            "FFT size: {}, overlap count: {}, hop size: {}, overlap size: {}",
            fft_size,
            overlap_count,
            hop_size,
            overlap_size
        );

        let mut fft_planner = FftPlanner::new();

        let fft = fft_planner.plan_fft_forward(fft_size);
        let ifft = fft_planner.plan_fft_inverse(fft_size);

        let origin = Complex::new(0.0f32, 0.0f32);

        let fft_scratch_buffer = vec![origin; fft.get_inplace_scratch_len()];
        let ifft_scratch_buffer = vec![origin; fft.get_inplace_scratch_len()];

        let signal_buffer = vec![origin; fft_size];
        let frequency_buffer = vec![origin; fft_size];
        let cepstrum_buffer = vec![origin; fft_size];

        let mut window = vec![0.0f32; fft_size];

        for (i, element) in window.iter_mut().enumerate() {
            let omega: f32 = 2.0f32 * std::f32::consts::PI / fft_size as f32;

            *element = 0.5f32 * (1.0f32 - (omega * i as f32).cos());
        }

        let mut input_ring_buffer = RingBuffer::<f32>::new(fft_size, 0.0f32);

        input_ring_buffer.discard_all();
        input_ring_buffer.fill(fft_size - hop_size, 0.0f32);

        log!(
            "input ring buffer: readable size {}",
            input_ring_buffer.readable_elements()
        );

        let mut output_ring_buffer =
            RingBuffer::<f32>::new(fft_size + overlap_size + 2 * block_size, 0.0f32);

        output_ring_buffer.discard_all();
        output_ring_buffer.fill(fft_size + overlap_size + block_size, 0.0f32);

        log!(
            "output ring buffer: capacity size {}",
            output_ring_buffer.capacity()
        );
        log!(
            "output ring buffer: readable size {}",
            output_ring_buffer.readable_elements()
        );

        let tmp_buffer = vec![0.0f32; fft_size];
        let wet_buffer = vec![0.0f32; block_size];

        let tmp_fft_buffer_0 = vec![origin; fft_size];
        let tmp_fft_buffer_1 = vec![origin; fft_size];
        let tmp_phase_buffer = vec![0.0f32; fft_size];

        let prev_input_phases = vec![0.0f32; fft_size];
        let prev_output_phases = vec![0.0f32; fft_size];

        let analysis_magnitude = vec![0.0f32; fft_size];
        let analysis_frequencies = vec![0.0f32; fft_size];

        let synthesis_magnitude = vec![0.0f32; fft_size];
        let synthesis_frequencies = vec![0.0f32; fft_size];

        let original_spectrum = vec![origin; fft_size];
        let shifted_spectrum = vec![origin; fft_size];
        let synthesis_spectrum = vec![origin; fft_size];

        let original_cepstrum = vec![origin; fft_size];
        let envelope = vec![origin; fft_size];
        let fine_structure = vec![origin; fft_size];

        WasmProcessor {
            sample_rate,
            block_size,
            fft,
            ifft,
            fft_scratch_buffer,
            ifft_scratch_buffer,
            fft_size,
            overlap_count,
            hop_size,
            input_ring_buffer,
            output_ring_buffer,
            signal_buffer,
            frequency_buffer,
            cepstrum_buffer,
            tmp_fft_buffer_0,
            tmp_fft_buffer_1,
            tmp_phase_buffer,
            window,
            prev_input_phases,
            prev_output_phases,
            analysis_magnitude,
            analysis_frequencies,
            synthesis_magnitude,
            synthesis_frequencies,
            original_spectrum,
            shifted_spectrum,
            synthesis_spectrum,
            original_cepstrum,
            envelope,
            fine_structure,
            tmp_buffer,
            wet_buffer,
            dry_wet: 0.8,
            formant: 0.0,
            pitch: 0.0,
            output_gain_decibel: 0.0,
            envelope_order: 5,
            input_level: MIN_DECIBEL,
            output_level: MIN_DECIBEL,
            level_reduction_per_sample: MIN_DECIBEL / sample_rate as f32,
        }
    }

    pub fn process(&mut self, buffer: &mut [f32], length: usize, levels: &mut [f32]) {
        let wet_level = self.dry_wet;
        let dry_level = 1.0f32 - wet_level;

        let new_input_gain_level = buffer
            .iter()
            .max_by(|x, y| x.abs().total_cmp(&y.abs()))
            .expect("buffer should not be empty");
        let reduced_level = MIN_DECIBEL
            .max(self.input_level + self.level_reduction_per_sample * buffer.len() as f32);

        self.input_level = gain_to_decibel(*new_input_gain_level, MIN_DECIBEL).max(reduced_level);

        levels[0] = self.input_level;

        let mut processed = 0;

        loop {
            if processed == length {
                break;
            }

            let writable_element = self.input_ring_buffer.writable_elements();

            if writable_element == 0 {}

            let min_writable_element = std::cmp::min(writable_element, length - processed);

            let write_result = self
                .input_ring_buffer
                .write(&buffer[processed..processed + min_writable_element]);

            if !write_result {}

            if self.input_ring_buffer.is_full() {
                self.process_fft_block();
            }

            let read_result = self
                .output_ring_buffer
                .read(&mut self.wet_buffer[processed..processed + min_writable_element]);

            if !read_result {}

            let discard_result = self.output_ring_buffer.discard(min_writable_element);

            if !discard_result {}

            processed += min_writable_element;
        }

        let output_gain = decibel_to_gain(self.output_gain_decibel);

        for (i, element) in buffer.iter_mut().enumerate() {
            let r = (*element * dry_level) + self.wet_buffer[i] * wet_level;

            *element = (-2.0f32).max(2.0f32.min(r * output_gain));
        }

        let new_output_level_gain = buffer
            .iter()
            .max_by(|x, y| x.abs().total_cmp(&y.abs()))
            .expect("buffer should not be empty");
        let reduced_level = MIN_DECIBEL
            .max(self.output_level + self.level_reduction_per_sample * buffer.len() as f32);

        self.output_level = gain_to_decibel(*new_output_level_gain, MIN_DECIBEL).max(reduced_level);

        levels[1] = self.output_level;
    }

    fn process_fft_block(&mut self) {
        let N = self.fft_size;

        let formant_expand_amount = 2.0f32.powf(self.formant);
        let pitch_change_amount = 2.0f32.powf(self.pitch);
        let envelope_amount = 1.0f32;
        let fine_structure_amount = 1.0f32;

        let read_result = self.input_ring_buffer.read(&mut self.tmp_buffer);

        if !read_result {
            log!("[ERROR] failed to read input ring buffer");
        }

        self.input_ring_buffer.discard(self.hop_size);

        for (i, element) in self.tmp_buffer.iter().enumerate() {
            self.signal_buffer[i] =
                Complex::new(element * self.window[i] / self.overlap_count as f32, 0.0f32);
        }

        self.frequency_buffer
            .clone_from_slice(&self.signal_buffer[..]);
        self.fft
            .process_with_scratch(&mut self.frequency_buffer, &mut self.fft_scratch_buffer);

        scale_complex(&mut self.frequency_buffer);

        for i in 0..N {
            self.original_spectrum[i] = self.frequency_buffer[i];
        }

        if WasmProcessor::get_buffer_effective_value_complex(&self.original_spectrum) == 0.0f32 {
            log!("[ERROR] invalid original_spectrum");
        }

        for (i, element) in self.frequency_buffer.iter().enumerate() {
            let mut amplitude = element.abs();

            if amplitude == 0.0f32 {
                amplitude = std::f32::EPSILON;
            }

            self.tmp_fft_buffer_0[i] = Complex::new(amplitude.ln(), 0.0f32);
        }

        self.cepstrum_buffer
            .clone_from_slice(&self.tmp_fft_buffer_0[..]);
        self.ifft
            .process_with_scratch(&mut self.cepstrum_buffer, &mut self.ifft_scratch_buffer);

        for i in 0..N {
            self.original_cepstrum[i] = self.cepstrum_buffer[i];
        }

        if WasmProcessor::get_buffer_effective_value_complex(&self.cepstrum_buffer) == 0.0f32 {
            log!("[ERROR] invalid original_cepstrum");
        }

        self.tmp_fft_buffer_0[0] = self.cepstrum_buffer[0];

        for i in 1..(N / 2 + 1) {
            let element = if i < self.envelope_order {
                self.cepstrum_buffer[i]
            } else {
                Complex::new(0.0f32, 0.0f32)
            };

            self.tmp_fft_buffer_0[i] = element;
            self.tmp_fft_buffer_0[N - 1] = element;
        }

        self.tmp_fft_buffer_1
            .clone_from_slice(&self.tmp_fft_buffer_0[..]);
        self.fft
            .process_with_scratch(&mut self.tmp_fft_buffer_1, &mut self.tmp_fft_buffer_0);

        scale_complex(&mut self.tmp_fft_buffer_1);

        for i in 0..N {
            self.envelope[i] = self.tmp_fft_buffer_1[i];
        }

        if WasmProcessor::get_buffer_effective_value_complex(&self.envelope) == 0.0f32 {
            log!("[ERROR] invalid envelope");
        }

        // Formant shift
        {
            self.tmp_fft_buffer_0.clone_from_slice(&self.envelope[..]);

            for i in 0..(N / 2 + 1) {
                let shifted_position = i as f32 / formant_expand_amount;
                let left_index = shifted_position.floor() as usize;
                let right_index = shifted_position.ceil() as usize;
                let diff = shifted_position - left_index as f32;

                let left_value = if left_index <= N / 2 {
                    self.tmp_fft_buffer_0[left_index].re()
                } else {
                    -1000.0f32
                };

                let right_value = if right_index <= N / 2 {
                    self.tmp_fft_buffer_0[right_index].re()
                } else {
                    -1000.0f32
                };

                let new_value = (1.0f32 - diff) * left_value + diff * right_value;

                self.envelope[i].re = new_value;
            }
        }

        // Pitch shift
        {
            let hop_size = self.hop_size;

            self.analysis_magnitude.fill(0.0f32);
            self.analysis_frequencies.fill(0.0f32);
            self.synthesis_magnitude.fill(0.0f32);
            self.analysis_frequencies.fill(0.0f32);

            for i in 0..(N / 2 + 1) {
                let magnitude = self.frequency_buffer[i].abs();
                let phase = self.frequency_buffer[i].arg();
                let bin_nyquist_frequency = 2.0f32 * std::f32::consts::PI * i as f32 / N as f32;

                let mut phase_diff = phase - self.prev_input_phases[i];

                self.prev_input_phases[i] = phase;

                phase_diff =
                    WasmProcessor::wrap_phase(phase_diff - bin_nyquist_frequency * hop_size as f32);

                let bin_deviation =
                    phase_diff * N as f32 / hop_size as f32 / (2.0f32 * std::f32::consts::PI);

                self.analysis_magnitude[i] = magnitude;
                self.analysis_frequencies[i] = i as f32 + bin_deviation;
            }

            for i in 0..(N / 2 + 1) {
                let bin_shifted = (i as f32 / pitch_change_amount + 0.5f32).floor() as usize;

                if bin_shifted > N / 2 {
                    break;
                }

                self.synthesis_magnitude[i] += self.analysis_magnitude[bin_shifted];
                self.synthesis_frequencies[i] =
                    self.analysis_frequencies[bin_shifted] * pitch_change_amount;
            }

            for i in 0..(N / 2 + 1) {
                let bin_deviation = self.synthesis_frequencies[i] - i as f32;
                let mut phase_diff =
                    bin_deviation * 2.0f32 + std::f32::consts::PI * hop_size as f32 / N as f32;
                let bin_nyquist_frequency = 2.0f32 * std::f32::consts::PI * i as f32 / N as f32;

                phase_diff += bin_nyquist_frequency * hop_size as f32;

                let phase = WasmProcessor::wrap_phase(self.prev_output_phases[i] + phase_diff);

                self.frequency_buffer[i] = Complex::new(
                    self.synthesis_magnitude[i] * phase.cos(),
                    self.synthesis_magnitude[i] * phase.sin(),
                );
                self.prev_output_phases[i] = phase;
            }

            for i in 1..(N / 2) {
                self.frequency_buffer[N - i] = self.frequency_buffer[i].conj();
            }
        }

        for i in 0..N {
            self.tmp_phase_buffer[i] = self.frequency_buffer[i].arg();
        }

        self.shifted_spectrum
            .clone_from_slice(&self.frequency_buffer);

        // Granular synthesis
        {
            for i in 0..N {
                let amplitude = self.frequency_buffer[i].abs();
                let r = (amplitude + std::f32::EPSILON).ln();

                self.tmp_fft_buffer_0[i] = Complex::new(r, 0.0f32);
            }

            self.cepstrum_buffer
                .clone_from_slice(&self.tmp_fft_buffer_0);
            self.ifft
                .process_with_scratch(&mut self.cepstrum_buffer, &mut self.ifft_scratch_buffer);

            self.tmp_fft_buffer_0[0] = Complex::new(0.0f32, 0.0f32);

            for i in 1..(N / 2 + 1) {
                let element = if i >= self.envelope_order {
                    self.cepstrum_buffer[i]
                } else {
                    Complex::new(0.0f32, 0.0f32)
                };

                self.tmp_fft_buffer_0[i] = element;
                self.tmp_fft_buffer_0[N - i] = element;
            }

            self.tmp_fft_buffer_1
                .clone_from_slice(&self.tmp_fft_buffer_0);
            self.fft
                .process_with_scratch(&mut self.tmp_fft_buffer_1, &mut self.fft_scratch_buffer);

            scale_complex(&mut self.tmp_fft_buffer_1);

            if WasmProcessor::get_buffer_effective_value_complex(&self.tmp_fft_buffer_1) == 0.0f32 {
                log!("[ERROR] invalid fine structure");
            }

            if pitch_change_amount < 1.0f32 {
                let new_nyquist_position =
                    (N as f32 * 0.5f32 * pitch_change_amount).round() as usize;

                for i in new_nyquist_position..(N / 2) {
                    self.tmp_fft_buffer_1[i] = Complex::new(0.0f32, 0.0f32);
                }

                for i in 1..(N / 2) {
                    self.tmp_fft_buffer_1[N - i] = self.tmp_fft_buffer_1[i];
                }
            }

            for i in 0..N {
                self.fine_structure[i] = self.tmp_fft_buffer_1[i];
            }
        }

        for i in 0..(N / 2 + 1) {
            let amplitude = (self.envelope[i].re() * envelope_amount
                + self.fine_structure[i].re()
                + fine_structure_amount)
                .exp();

            self.frequency_buffer[i] = Complex::new(
                amplitude * self.tmp_phase_buffer[i].cos(),
                amplitude * self.tmp_phase_buffer[i].sin(),
            );
        }

        for i in 1..(N / 2) {
            self.frequency_buffer[N - i] = self.frequency_buffer[i].conj();
        }

        for i in 0..N {
            self.synthesis_spectrum[i] = self.frequency_buffer[i];
        }

        self.signal_buffer.clone_from_slice(&self.frequency_buffer);
        self.ifft
            .process_with_scratch(&mut self.signal_buffer, &mut self.fft_scratch_buffer);

        let energy = WasmProcessor::get_buffer_effective_value_complex(&self.signal_buffer);

        if energy.is_nan() {
            log!("frequency buffer is NaN: {:?}", &self.frequency_buffer);
        }

        for i in 0..N {
            self.tmp_buffer[i] = self.signal_buffer[i].re() * self.window[i];
        }

        if WasmProcessor::get_buffer_effective_value_f32(&self.tmp_buffer) == 0.0f32 {
            log!("[ERROR] invalid output signal");
        }

        let overlap_add_result = self
            .output_ring_buffer
            .overlap_add(&self.tmp_buffer, N - self.hop_size);

        if !overlap_add_result {
            log!("[ERROR] failed to overlapped add");
        }
    }

    fn get_buffer_effective_value_complex(buffer: &[Complex<f32>]) -> f32 {
        if buffer.len() == 0 {
            return 0.0f32;
        }

        let mean_squared =
            buffer.iter().fold(0.0f32, |acc, cur| acc + cur.norm_sqr()) / buffer.len() as f32;

        mean_squared
    }

    fn get_buffer_effective_value_f32(buffer: &[f32]) -> f32 {
        if buffer.len() == 0 {
            return 0.0f32;
        }

        let mean_squared = buffer.iter().fold(0.0f32, |acc, cur| acc + cur) / buffer.len() as f32;

        mean_squared.sqrt()
    }

    fn wrap_phase(phase_in: f32) -> f32 {
        let pi = std::f32::consts::PI;

        if phase_in >= 0.0f32 {
            (phase_in + pi) % (2.0f32 * pi) - pi
        } else {
            (phase_in - pi) % (-2.0f32 * pi) + pi
        }
    }

    pub fn set_dry_wet(&mut self, value: f32) {
        self.dry_wet = value;
    }

    pub fn set_output_gain_decibel(&mut self, value: f32) {
        self.output_gain_decibel = value;
    }

    pub fn set_pitch_shift(&mut self, value: f32) {
        self.pitch = value;
    }

    pub fn set_formant_shift(&mut self, value: f32) {
        self.formant = value;
    }

    pub fn set_envelope_order(&mut self, value: f32) {
        self.envelope_order = value;
    }
}
