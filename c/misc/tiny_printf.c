#include <stdio.h>
#include <stdarg.h>

int tiny_printf(const char *format, ...);

int main(void) {
  tiny_printf("Hello World: \n");
  tiny_printf("Hello World: %d\n", 1);
  tiny_printf("Hello World: %d %d\n", 1, 2);

  return 0;
}

int tiny_printf(const char *format, ...) {
  va_list args;

  va_start(args, format);

  while (*format != '\0') {
    if (*format == '%') {
      if (*(++format) != 'd') {
        return -1;
      }

      fputc(('0' + va_arg(args, int)), stdout);
    } else {
      fputc(*format, stdout);
    }

    ++format;
  }

  va_end(args);

  return 0;
}
