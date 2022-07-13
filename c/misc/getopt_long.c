#include <stdio.h>
#include <getopt.h>

int main(int argc, char **argv) {
  const char *optstring = "hD:mvn";

  const struct option long_option[] = {
    { "help",             no_argument, NULL, 'h' },
    { "device",     required_argument, NULL, 'D' },
    { "mmap",             no_argument, NULL, 'm' },
    { "verbose",          no_argument, NULL, 'v' },
    { "noresample",       no_argument, NULL, 'n' },
    {            0,                 0,    0,  0  }
  };

  int long_index;

  int c;

  while((c = getopt_long(argc, argv, optstring, long_option, &long_index)) != -1) {
    printf("%c %d\n", c, long_index);
  }

  return 0;
}
