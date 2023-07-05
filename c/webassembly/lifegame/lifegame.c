#include <stdlib.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define PIXEL_0 0xff1e0f0f
#define PIXEL_1 0xffc8ffc8

#define set_pixel(m, o, b) (m[o] = (b ? PIXEL_1 : PIXEL_0))
#define is_pixel1(m, o) (m[o] == PIXEL_1)

static int *map = NULL;
static int *tmp = NULL;

static int width  = 0;
static int height = 0;

static int *allocmap(int w, int h);

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void init(int w, int h) {
  if (map) {
    free(map);
  }

  if (tmp) {
    free(tmp);
  }

  map = allocmap(w, h);
  tmp = allocmap(w, h);

  width  = w;
  height = h;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void dot(int x, int y, int b) {
  int offset = (y * width) + x;

  set_pixel(map, offset, b);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
int *getmap(void) {
  return map;
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void lifegame(void) {
  int offset = 0;

  int src0;
  int src1 = width * (height - 1);
  int src2 = 0;

  for (int y = 0; y < height; y++) {
    src0 = src1;
    src1 = src2;
    src2 = width * ((y + 1) % height);

    int c0 = 0;
    int c1 = 0;

    int d = width - 1;

    if (is_pixel1(map, (src0 + d))) c0++;
    if (is_pixel1(map, (src1 + d))) c0++;
    if (is_pixel1(map, (src2 + d))) c0++;

    if (is_pixel1(map, src0)) c1++;
    if (is_pixel1(map, src1)) c1++;
    if (is_pixel1(map, src2)) c1++;

    for (int x = 0; x < width; x++) {
      int r = (x + 1) % width;

      int c2 = 0;

      if (is_pixel1(map, (src0 + r))) c2++;
      if (is_pixel1(map, (src1 + r))) c2++;
      if (is_pixel1(map, (src2 + r))) c2++;

      int c = c0 + c1 + c2;

      if (c == 3) {
        set_pixel(tmp, offset, 1);
      } else {
        if (c == 4) {
          set_pixel(tmp, offset, (is_pixel1(map, (src0 + x)) ? 1 : 0));
        } else {
          set_pixel(tmp, offset, 0);
        }
      }

      c0 = c1;
      c1 = c2;

      offset++;
    }
  }

  int *prev_map;

  prev_map = map;
  map      = tmp;
  tmp      = prev_map;
}

static int *allocmap(int w, int h) {
  int size = w * h;

  int *p = (int *)malloc(size * sizeof(int));

  for (int i = 0; i < size; i++) {
    set_pixel(p, i, 0);
  }

  return p;
}
