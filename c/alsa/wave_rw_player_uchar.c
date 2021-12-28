#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include "alsa/asoundlib.h"
#include "WaveFormat.h"

#define _FILE_OFFSET_BITS 64
#define TRUE  1
#define FALSE 0

// TODO:
typedef struct {
} snd_pcm_t;

typedef struct {
} snd_pcm_hw_params_t;

typedef struct {
} snd_pcm_sw_params_t;

typedef uint8_t snd_pcm_uframes_t;
typedef int8_t  snd_pcm_sframes_t;
typedef int32_t snd_pcm_format_t;

static int wave_read_header(void);
static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static int write_uchar(snd_pcm_t *handle);
static void usage(void);
static snd_pcm_sframes_t *writei_func(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);

static char *device = "plughw:0,0";
static snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;
static unsigned int rate = 44100;
static unsigned int number_of_chanels = 1;
static unsigned int buffer_time = 0;
static unsigned int period_time = 0;
static snd_pcm_uframes_t buffer_size = 0;
static snd_pcm_uframes_t period_size = 0;
static snd_output_t *output = NULL;

static int mmap     = FALSE;
static int verbose  = FALSE;
static int resample = TRUE;

static WAVEFormatDesc fmt_desc;
static WAVEFileDesc file_desc;

int main(int argc, char **argv) {
  static const struct option long_option[] = {
    { "help",       0, NULL, 'h' },
    { "device",     1, NULL, 'D' },
    { "mmap",       0, NULL, 'm' },
    { "verbose",    0, NULL, 'v' },
    { "noresample", 0, NULL, 'n' },
  };

  int c;

  while ((c = getopt_long(argc, argv, "hD:mvn", long_option, NULL) != -1)) {
    switch (c) {
      case 'h':
        usage();
        return 0;
      case 'D':
        device = strdup(optarg);
        break;
      case 'm':
        mmap = TRUE;
        break;
      case 'v':
        verbose = TRUE;
        break;
      case 'n':
        resample = FALSE;
        break;
      default:
        fputs("`--help` confirm usage\n", stderr);
        exit(EXIT_FAILURE);
    }
  }

  if (optind > (argc - 1)) {
    usage();
    return 0;
  }

  return 0;
}
