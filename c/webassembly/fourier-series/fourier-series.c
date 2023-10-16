#include <stdlib.h>
#include <math.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static const float RESOLUTION = 3500.0f;

static float *fourier_series = NULL;

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
float *square(const int N, const int K) {
  if (fourier_series) {
    free(fourier_series);
  }

  fourier_series = (float *)calloc(N, sizeof(float));

  for (int n = 0; n < N; n++) {
    fourier_series[n] = 0.0f;

    for (int k = 0; k < K; k++) {
      float x = n / RESOLUTION;
      int odd = (2 * k) + 1;

      //         -(M_PI / 4) (-M_PI < x < 0)
      // f(x) =
      //          (M_PI / 4) (0 < x < M_PI)
      //
      // += sin(2k + 1)x / (2k + 1)
      fourier_series[n] += sinf((float)(odd) * x) / (float)(odd);
    }
  }

  return fourier_series;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
float *sawtooth(const int N, const int K) {
  if (fourier_series) {
    free(fourier_series);
  }

  fourier_series = (float *)calloc(N, sizeof(float));

  for (int n = 0; n < N; n++) {
    fourier_series[n] = 0.0f;

    for (int k = 1; k < K; k++) {
      float x = n / RESOLUTION;

      // f(x) = 4x (-M_PI < x < M_PI)
      //
      // += (1 / 4) * (2 * ((-1)^(k + 1) / k) * sinkx)
      fourier_series[n] += (1.0f / 4.0f) * (2.0f * (powf(-1.0f, (float)(k + 1)) / k) * sinf((float)k * x));
    }
  }

  return fourier_series;
}
