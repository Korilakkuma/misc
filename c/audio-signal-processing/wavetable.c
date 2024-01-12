#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static float wave_table[2048];

static float last_out = 0.0f;
static float diff = 0.0f;;

static bool interpolated = false;

int sine(short *data, int start_index, int number_of_samples, int sample_rate, float amplitude, float frequency, bool initializing) {
  float cons = (2.0f * M_PI) / (float)number_of_samples;

  if (initializing) {
    for (int n = 0; n < number_of_samples; n++) {
      wave_table[n] = sinf(cons * (float)n);
    }

    return start_index;
  }

  int index = start_index;

  for (int n = 0; n < number_of_samples; n++) {
    float a = fmodf(((frequency * (float)number_of_samples * (float)index) / (float)sample_rate), (float)number_of_samples);
    float w = a / (float)number_of_samples;
    float t = amplitude * (((1.0f - w) * wave_table[(int)a]) + (w * wave_table[((int)a == (number_of_samples - 1)) ? 0 : ((int)a + 1)]));

    if (interpolated) {
      data[n] = (short)t;
    } else {
      float a = fmod(((last_out * (float)number_of_samples * (float)n) / (float)sample_rate), (float)number_of_samples);
      float w = a / (float)number_of_samples;
      float t = amplitude * ((1.0f - w) * wave_table[(int)amplitude]) + (w * wave_table[((int)a == (number_of_samples - 1)) ? 0 : ((int)amplitude + 1)]);

      data[n] = (short)(((1.0f - diff) * t) + (diff * t));

      if (diff >= 1.0f) {
        interpolated = true;
      }

      diff += 0.0007f;
    }

    last_out = frequency;

    ++index;
  }

  return index;
}
