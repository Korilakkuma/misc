#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include <string.h>
#include <limits.h>
#include <sndfile.h>

static int sample_rate        = 44100;
static int quantization_bits  = 16;
static int number_of_channels = 2;

static float frequency = 440;
static float duration  = 5.0;
static double start    = 1.0;
static double end      = 1.0e-4;

static void usage(void);

int main(int argc, char **argv) {
  static const char *optstring = "hf:r:b:";

  static const struct option long_option[] = {
    { "help",   no_argument,       NULL, 'h' },
    { "format", required_argument, NULL, 'f' },
    { "rate",   required_argument, NULL, 'r' },
    { "bits",   required_argument, NULL, 'b' },
    { 0,        0,                 0,     0  }
  };

  int long_index;

  int c;
  char *formatID[] = { "16", "44100", "wav" };
  char filename[256] = "test_sound_";

  int format = SF_FORMAT_WAV;
  int subformat;

  SNDFILE *outfile;
  SF_INFO outfile_info = { 0 };

  while ((c = getopt_long(argc, argv, optstring, long_option, &long_index)) != -1) {
    switch (c) {
      case 'h':
        usage();
        return 0;
      case 'f':
        formatID[2] = strdup(optarg);

        if (strncmp(formatID[2], "flac", 4) == 0) {
          format = SF_FORMAT_FLAC;
        } else if (strncmp(formatID[2], "aiff", 4) == 0) {
          format = SF_FORMAT_AIFF;
        } else {
          usage();
          exit(EXIT_FAILURE);
        }

        break;
      case 'r':
        sample_rate = (int)strtol(optarg, NULL, 10);

        if ((sample_rate < 44100) || (sample_rate > 192000)) {
          usage();
          exit(EXIT_FAILURE);
        }

        formatID[1] = strdup(optarg);

        break;
      case 'b':
        quantization_bits = (int)strtol(optarg, NULL, 10);

        if (!((quantization_bits == 16) || (quantization_bits == 24) || (quantization_bits == 32))) {
          usage();
          exit(EXIT_FAILURE);
        }

        formatID[0] = strdup(optarg);

        break;
      default:
        fprintf(stderr, "`--help` confirm usage\n");
        exit(EXIT_FAILURE);
    }
  }

  strcat(filename, formatID[0]);
  strcat(filename, "_");
  strcat(filename, formatID[1]);
  strcat(filename, ".");
  strcat(filename, formatID[2]);

  switch (quantization_bits) {
    case 16:
      subformat = SF_FORMAT_PCM_16;
      break;
    case 24:
      subformat = SF_FORMAT_PCM_24;
      break;
    case 32:
      if (format == SF_FORMAT_FLAC) {
        fprintf(stderr, "FLAC does not support 32 bits as quantization bit\n");
        exit(EXIT_FAILURE);
      }

      subformat = SF_FORMAT_PCM_32;

      break;
    default:
      exit(EXIT_FAILURE);
  }

  int number_of_samples = (int)(sample_rate * duration);

  double angle_increment = 2.0 * M_PI * (frequency / sample_rate);
  double factor = pow((end / start), (1.0 / (double)number_of_samples));

  outfile_info.samplerate = sample_rate;
  outfile_info.channels   = number_of_channels;
  outfile_info.format     = format | subformat;

  if (!(outfile = sf_open(filename, SFM_WRITE, &outfile_info))) {
    fprintf(stderr, "Cannnot open file\n");
    exit(EXIT_FAILURE);
  }

  double current = start;

  for (int n = 0; n < number_of_samples; n++) {
    double fsample = current * sin(n * angle_increment);  // (2 * pi * f * n / fs)

    current *= factor;

    int sample[] = { (int)(INT_MAX * fsample), (int)(INT_MAX * fsample) };

    sf_write_int(outfile, sample, 2);
  }

  printf("The number of %d samples are generated to %s\n", number_of_samples, filename);

  if (outfile != NULL) {
    sf_close(outfile);
  }

  return 0;
}

static void usage(void) {
  printf("Usage: ./generate_test_sound [options] ...\n");
  printf("-h, --help                usage\n");
  printf("-f, --format              format (wav, flac, aiff. default value is wav)\n");
  printf("-r, --rate                sample rate (default value is %d)\n", sample_rate);
  printf("-b, --bits                quantization bits (16, 24, 32. default value is %d)\n", quantization_bits);
}
