#include "socketlib.h"

char *trim_newline(char *str, int len) {
  size_t n = strlen(str);

  if ((n < len) && (str[n -1] == '\n')) {
    str[n - 1] = '\0';
  }

  return str;
}
