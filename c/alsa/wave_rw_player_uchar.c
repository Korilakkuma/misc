#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <errno.h>
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
static unsigned int number_of_channels = 1;
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

  snd_pcm_t *handle = NULL;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;

  unsigned char *transfer_method;
  unsigned short qbits;
  double play_time = 0.0;
  int c;
  int err;
  int exit_code = 0;

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

  const char *file_path = NULL;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);

  file_path = argv[optind];

  int fd = open(file_path, O_RDONLY);

  if (fd < 0) {
    fputs("Open WAVE File Error\n", stderr);
    exit_code = errno;
    goto clean;
  }

  file_desc.fd = fd;

  if (wave_read_header() != 0) {
    exit_code = EXIT_FAILURE;
    goto clean;
  }

  if (fmt_desc.bits_per_sample > 32) {
    fprintf(stderr, "Not support quantization bit (%d)\n", fmt_desc.bits_per_sample);
    exit_code = EXIT_FAILURE;
    goto clean;
  }

  number_of_channels = (unsigned int)fmt_desc.number_of_channels;
  rate               = fmt_desc.samples_per_sec;
  qbits              = fmt_desc.bits_per_sample;
  play_time          = (double)file_desc.frame_size / (double)rate;

  printf("=== WAVE file info ===\n");
  printf("filename: %s\n", file_path);
  printf("sample rate: %d Hz\n", rate);
  printf("number of channels: %d channels\n", number_of_channels);

  switch (qbits) {
    case 16:
      format = SND_PCM_FORMAT_S16_LE;
      printf("Quantization bits: signed 16 bits\n");
      break;
    case 24:
      format = SND_PCM_FORMAT_S24_LE;
      printf("Quantization bits: signed 24 bits\n");
      break;
    case 32:
    default:
      printf("Quantization bits: signed 32 bits or unknown\n");
      break;
  }

  printf("duration: %.01f sec\n\n", play_time);

  err = snd_output_stdio_attach(&output, stdout, 0);

  if (err < 0) {
    fprintf(stderr, "Attaching that is log settings for ALSA failed: %s\n", snd_strerror(err));
    exit_code = err;
    goto clean;
  }

  if (mmap) {
    writei_func = snd_pcm_mmap_writei;
    transfer_method = "mmap_write";
  } else {
    writei_func = snd_pcm_writei;
    transfer_method = "write";
  }

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf(stderr, "PCM OPEN failed: %s\n", snd_strerror(err));
    exit_code = err;
    goto clean;
  }

  if ((err = set_hwparams(&handle, hwparams)) < 0) {
    fprintf(stderr, "Set HW parameters failed: %s\n", snd_strerror(err));
    printf("=== PCM HW parameters ===\n");
    snd_pcm_hw_params_dump(hwparams, output);
    printf("\n");
    exit_code = err;
    goto clean;
  }

  if ((err = set_swparams(&handle, swparams)) < 0) {
    fprintf(stderr, "Set SW parameters failed: %s\n", snd_strerror(err));
    printf("=== PCM SW parameters ===\n");
    snd_pcm_sw_params_dump(swparams, output);
    printf("\n");
    exit_code = err;
    goto clean;
  }

  if (verbose) {
    printf("=== PCM info ===\n");
    snd_pcm_dump(handle, output);
    printf("\n");
  }

  printf("Inner Format: %s\n", snd_pcm_format_name(format));
  printf("PCM Device: %s\n", device);
  printf("Transfer Method: %s\n\n", transfer_method);

  err = write_uchar(handle);

  if (err != 0) {
    fputs("Playback failed\n", stderr);
    exit_code = err;
    goto clean;
  }

clean:
  if (output != NULL) {
    snd_output_close(output);
  }

  if (handle != NULL) {
    snd_pcm_close(handle);
  }

  snd_config_update_free_global();

  if (fd >= 0) {
    close(fd);
  }

  return 0;
}

static int wave_read_header(void) {
  FOURCC chunk_id;
  DWORD chunk_size;
  GUID sub_format;

  lseek(file_desc.fd, 0, SEEK_SET);
  read(file_desc.fd, &chunk_id, size(FOURCC));
  read(file_desc.fd, &chunk_size, size(DWORD));

  if (chunk_id != *(FOURCC *)RIFF_ID) {
    fputs("File error: Not RIFF form\n", stderr);
    exit(EXIT_FAILURE);
  }

  read(file_desc.fd, &chunk_id, sizeof(FOURCC));

  if (chunk_id != *(FOURCC *)WAVE_ID) {
    fputs("File error: Not WAVE form\n", stderr);
    exit(EXIT_FAILURE);
  }

  while (TRUE) {
    read(file_desc.fd, &chunk_id, sizeof(FOURCC));
    read(file_desc.fd, &chunk_size, sizeof(FOURCC));

    if (chunk_id == *(FOURCC *)FMT_ID) {
      if ((chunk_size != FORMAT_CHUNK_PCM_SIZE) && (chunk_size != FORMAT_CHUNK_EX_SIZE) && (chunk_size != FORMAT_CHUNK_EXTENSIBLE_SIZE)) {
        fprintf(stderr, "Chunk Size Error: %d\n", chunk_size);
        exit(EXIT_FAILURE);
      }

      read(file_desc.fd, &fmt_desc, FORMAT_CHUNK_PCM_SIZE);

      if ((fmt_desc.format_tag != WAVE_FORMAT_PCM) && (fmt_desc.format_tag != WAVE_FORMAT_EXTENSIBLE)) {
        fprintf(stderr, "Format Error: %x\n", fmt_desc.format_tag);
        exit(EXIT_FAILURE);
      }

      switch (chunk_size) {
        case FORMAT_CHUNK_EXTENSIBLE_SIZE:
          lseek(file_desc.fd, 8, SEEK_CUR);
          read(file_desc.fd, &sub_format, sizeof(GUID));

          if (sub_format.sub_format_code != WAVE_FORMAT_PCM) {
            fprintf(stderr, "Extended sub format code Error: %x\n", sub_format.sub_format_code);
            exit(EXIT_FAILURE);
          } else if (memcmp(sub_format.wave_guid_tag, WAVE_GUID_TAG, 14) != 0) {
            fprintf(stderr, "GUID = %x Not WAVE GUID TAG\n", (unsigned int)sub_format.wave_guid_tag);
          } else {
            printf("Chunk Size = %d, WAVE FORMAT EXTENSIBLE LPCM\n", chunk_size);
          }

          break;
        case FORMAT_CHUNK_EX_SIZE:
          lseek(file_desc.fd, 2, SEEK_CUR);
          printf("Chunk Size = %d WAVE FORMAT EX LPCM\n", chunk_size);
          break;
        default:
          printf("Chunk Size = %d Standard WAVE LPCM\n", chunk_size);
          break;
      }
    } else if (chunk_id == *(FOURCC *)DATA_ID) {
      file_desc.frame_size = (long)chunk_size / (long)fmt_desc.data_frame_size;
    } else {
      lseek(file_desc.fd, (off_t)chunk_size, SEEK_CUR);
    }
  }

  return 0;
}
