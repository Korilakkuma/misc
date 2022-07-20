#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <alsa/asoundlib.h>
#include "WaveFormat.h"

static int wave_read_header(void);
static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static int write_uchar(snd_pcm_t *handle);
static void usage(void);
static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);

static char *device = "plughw:0,0";
static snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;
static DWORD rate = 44100;
static DWORD channels = 1;
static DWORD buffer_time = 0;
static DWORD period_time = 0;
static snd_pcm_uframes_t buffer_size = 0;
static snd_pcm_uframes_t period_size = 0;
static snd_output_t *output = NULL;

static bool mmap     = false;
static bool verbose  = false;
static bool resample = true;

static WAVEFormatDesc fmt_desc;
static WAVEFileDesc file_desc;

int main(int argc, char **argv) {
  const char *optstring = "hD:mvn";

  static const struct option long_option[] = {
    { "help",       no_argument,       NULL, 'h' },
    { "device",     required_argument, NULL, 'D' },
    { "mmap",       no_argument,       NULL, 'm' },
    { "verbose",    no_argument,       NULL, 'v' },
    { "noresample", no_argument,       NULL, 'n' },
    { 0,            0,                 0,     0  }
  };

  int long_index;

  snd_pcm_t *handle = NULL;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;

  char *transfer_method;
  WORD qbits;
  double play_time = 0.0;
  int c;
  int err;
  int status_code = 0;

  while ((c = getopt_long(argc, argv, optstring, long_option, &long_index)) != -1) {
    switch (c) {
      case 'h':
        usage();
        return 0;
      case 'D':
        device = strdup(optarg);
        break;
      case 'm':
        mmap = true;
        break;
      case 'v':
        verbose = true;
        break;
      case 'n':
        resample = false;
        break;
      default:
        break;
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
    fprintf(stderr, "Open WAVE File Error\n");
    status_code = errno;
    goto clean;
  }

  file_desc.fd = fd;

  if (wave_read_header() != 0) {
    status_code = EXIT_FAILURE;
    goto clean;
  }

  if (fmt_desc.bits_per_sample > 32) {
    fprintf(stderr, "Not support quantization bit (%d)\n", fmt_desc.bits_per_sample);
    status_code = EXIT_FAILURE;
    goto clean;
  }

  channels  = (DWORD)fmt_desc.number_of_channels;
  rate      = fmt_desc.samples_per_sec;
  qbits     = fmt_desc.bits_per_sample;
  play_time = (double)file_desc.frame_size / (double)rate;

  printf("=== WAVE file info ===\n");
  printf("Filename: %s\n", file_path);
  printf("Sample Rate: %d Hz\n", rate);
  printf("Number of Channels: %d channels\n", channels);

  switch (qbits) {
    case 16:
      format = SND_PCM_FORMAT_S16_LE;
      printf("Quantization Bits: signed 16 bits\n");
      break;
    case 24:
      format = SND_PCM_FORMAT_S24_LE;
      printf("Quantization Bits: signed 24 bits\n");
      break;
    case 32:
    default:
      printf("Quantization Bits: signed 32 bits or unknown\n");
      break;
  }

  printf("Duration: %.01f sec\n\n", play_time);

  err = snd_output_stdio_attach(&output, stdout, 0);

  if (err < 0) {
    fprintf(stderr, "Attaching that is log settings for ALSA failed: %s\n", snd_strerror(err));
    status_code = err;
    goto clean;
  }

  if (mmap) {
    writei_func     = snd_pcm_mmap_writei;
    transfer_method = "mmap_write";
  } else {
    writei_func     = snd_pcm_writei;
    transfer_method = "write";
  }

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    fprintf(stderr, "PCM OPEN failed: %s\n", snd_strerror(err));
    status_code = err;
    goto clean;
  }

  if ((err = set_hwparams(handle, hwparams)) < 0) {
    fprintf(stderr, "Set HW parameters failed: %s\n", snd_strerror(err));
    printf("=== PCM HW parameters ===\n");
    snd_pcm_hw_params_dump(hwparams, output);
    printf("\n");
    status_code = err;
    goto clean;
  }

  if ((err = set_swparams(handle, swparams)) < 0) {
    fprintf(stderr, "Set SW parameters failed: %s\n", snd_strerror(err));
    printf("=== PCM SW parameters ===\n");
    snd_pcm_sw_params_dump(swparams, output);
    printf("\n");
    status_code = err;
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
    fprintf(stderr, "Playback failed\n");
    status_code = err;
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

  return status_code;
}

static int wave_read_header(void) {
  FOURCC chunk_id;
  DWORD chunk_size;
  GUID sub_format;

  ssize_t s;

  lseek(file_desc.fd, 0, SEEK_SET);

  s = read(file_desc.fd, &chunk_id, sizeof(FOURCC));

  if (s == -1) {
    exit(EXIT_FAILURE);
  }

  s = read(file_desc.fd, &chunk_size, sizeof(DWORD));

  if (s == -1) {
    exit(EXIT_FAILURE);
  }

  if (chunk_id != *(FOURCC *)RIFF_ID) {
    fprintf(stderr, "File error: Not RIFF form\n");
    exit(EXIT_FAILURE);
  }

  s = read(file_desc.fd, &chunk_id, sizeof(FOURCC));

  if (chunk_id != *(FOURCC *)WAVE_ID) {
    fprintf(stderr, "File error: Not WAVE form\n");
    exit(EXIT_FAILURE);
  }

  while (true) {
    s = read(file_desc.fd, &chunk_id, sizeof(FOURCC));

    if (s == -1) {
      exit(EXIT_FAILURE);
    }

    s = read(file_desc.fd, &chunk_size, sizeof(FOURCC));

    if (s == -1) {
      exit(EXIT_FAILURE);
    }

    if (chunk_id == *(FOURCC *)FMT_ID) {
      if ((chunk_size != FORMAT_CHUNK_PCM_SIZE) && (chunk_size != FORMAT_CHUNK_NON_PCM_SIZE) && (chunk_size != FORMAT_CHUNK_EXTENSIBLE_SIZE)) {
        fprintf(stderr, "Chunk Size Error: %d\n", chunk_size);
        exit(EXIT_FAILURE);
      }

      s = read(file_desc.fd, &fmt_desc, FORMAT_CHUNK_PCM_SIZE);

      if (s == -1) {
        exit(EXIT_FAILURE);
      }

      if ((fmt_desc.format_tag != WAVE_FORMAT_PCM) && (fmt_desc.format_tag != WAVE_FORMAT_EXTENSIBLE)) {
        fprintf(stderr, "Format Error: %x\n", fmt_desc.format_tag);
        exit(EXIT_FAILURE);
      }

      switch (chunk_size) {
        case FORMAT_CHUNK_EXTENSIBLE_SIZE:
          lseek(file_desc.fd, 8, SEEK_CUR);

          s = read(file_desc.fd, &sub_format, sizeof(GUID));

          if (s == -1) {
            exit(EXIT_FAILURE);
          }

          if (sub_format.sub_format_code != WAVE_FORMAT_PCM) {
            fprintf(stderr, "Extended sub format code Error: %x\n", sub_format.sub_format_code);
            exit(EXIT_FAILURE);
          } else if (memcmp(sub_format.wave_guid_tag, WAVE_GUID_TAG, 14) != 0) {
            fprintf(stderr, "GUID = %s Not WAVE GUID TAG\n", sub_format.wave_guid_tag);
          } else {
            printf("Chunk Size = %d, WAVE FORMAT EXTENSIBLE LPCM\n", chunk_size);
          }

          break;
        case FORMAT_CHUNK_NON_PCM_SIZE:
          lseek(file_desc.fd, 2, SEEK_CUR);
          printf("Chunk Size = %d WAVE FORMAT EX LPCM\n", chunk_size);
          break;
        default:
          printf("Chunk Size = %d Standard WAVE LPCM\n", chunk_size);
          break;
      }
    } else if (chunk_id == *(FOURCC *)DATA_ID) {
      file_desc.frame_size = (long)chunk_size / (long)fmt_desc.data_frame_size;
      break;
    } else {
      lseek(file_desc.fd, (off_t)chunk_size, SEEK_CUR);
    }
  }

  return 0;
}

static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams) {
  unsigned int rate_near;
  int dir;
  int err;

  err = snd_pcm_hw_params_any(handle, hwparams);

  if (err < 0) {
    fprintf(stderr, "None applicable hardware config: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_set_rate_resample(handle, hwparams, resample);

  if (err < 0) {
    fprintf(stderr, "Resampling failed: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_set_access(handle, hwparams, mmap ? SND_PCM_ACCESS_MMAP_INTERLEAVED : SND_PCM_ACCESS_RW_INTERLEAVED);

  if (err < 0) {
    fprintf(stderr, "Access type is not applicable: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_set_format(handle, hwparams, format);

  if (err < 0) {
    fprintf(stderr, "Sample format is not applicable: %s\n", snd_strerror(err));
    fprintf(stderr, "Applicable formats\n");

    for (int fmt = 0; fmt < SND_PCM_FORMAT_LAST; fmt++) {
      if (snd_pcm_hw_params_test_format(handle, hwparams, (snd_pcm_format_t)fmt) == 0) {
        fprintf(stderr, "- %s\n", snd_pcm_format_name((snd_pcm_format_t)fmt));
      }
    }

    return err;
  }

  err = snd_pcm_hw_params_set_channels(handle, hwparams, channels);

  if (err < 0) {
    fprintf(stderr, "The number of channels (%i) is not applicable: %s\n", channels, snd_strerror(err));
    return err;
  }

  rate_near = (unsigned int)rate;

  err = snd_pcm_hw_params_set_rate_near(handle, hwparams, &rate_near, 0);

  if (err < 0) {
    fprintf(stderr, "Sample rate (%i Hz) is not applicable: %s\n", rate, snd_strerror(err));
    return err;
  }

  if (rate_near != rate) {
    fprintf(stderr, "Sample rate is not consistent (Expected %i Hz, Actual %i Hz)\n", rate, rate_near);
    return -EINVAL;
  }

  err = snd_pcm_hw_params_get_buffer_time_max(hwparams, &buffer_time, &dir);

  if (buffer_time > 500000) {
    buffer_time = 500000;
  }

  if (buffer_time > 0) {
    period_time = buffer_time / 4;
  } else {
    fprintf(stderr, "buffer time should be more than 0\n");
    return -EINVAL;
  }

  err = snd_pcm_hw_params_set_buffer_time_near(handle, hwparams, &buffer_time, &dir);

  if (err < 0) {
    fprintf(stderr, "buffer time (%i) is not applicable: %s\n", buffer_time, snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_set_period_time_near(handle, hwparams, &period_time, &dir);

  if (err < 0) {
    fprintf(stderr, "period time (%i) is not applicable: %s\n", period_time, snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params(handle, hwparams);

  if (err < 0) {
    fprintf(stderr, "Hardware parameters are not applicable: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size);

  if (err < 0) {
    fprintf(stderr, "buffer size cannot be gotten: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_hw_params_get_period_size(hwparams, &period_size, &dir);

  if (err < 0) {
    fprintf(stderr, "period size cannot be gotten: %s\n", snd_strerror(err));
    return err;
  }

  return 0;
}

static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams) {
  int err;

  err = snd_pcm_sw_params_current(handle, swparams);

  if (err < 0) {
    fprintf(stderr, "Current software parameters are unknown: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_sw_params_set_start_threshold(handle, swparams, ((buffer_size / period_size) * period_size));

  if (err < 0) {
    fprintf(stderr, "Start threshold is not applicable: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_size);

  if (err < 0) {
    fprintf(stderr, "Available minimum frame size is not applicable: %s\n", snd_strerror(err));
    return err;
  }

  err = snd_pcm_sw_params(handle, swparams);

  if (err < 0) {
    fprintf(stderr, "Software parameters are not applicable: %s\n", snd_strerror(err));
    return err;
  }

  return 0;
}

static int write_uchar(snd_pcm_t *handle) {
  const WORD frame_bytes = (WORD)fmt_desc.data_frame_size;
  const long total_frames = file_desc.frame_size;

  // HACK:
  period_size = total_frames;

  long total_bytes = (long)(period_size * frame_bytes);
  long frame_count = 0;

  long playback_frames = 0;
  long read_frames     = 0;
  long rest_frames     = total_frames;

  int err = 0;

  BYTE *frame_block = (BYTE *)malloc((size_t)total_bytes);

  if (frame_block == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    err = EXIT_FAILURE;
    goto clean;
  }

  BYTE *frame_buffer;

  while (rest_frames > 0) {
    read_frames = (long)read(file_desc.fd, frame_block, (size_t)(total_bytes / frame_bytes));
    frame_count = read_frames;

    frame_buffer = frame_block;

    while (frame_count > 0) {
      int transfer_frames = (int)writei_func(handle, frame_buffer, (snd_pcm_sframes_t)frame_count);

      if (transfer_frames == -EAGAIN) {
        continue;
      }

      if (transfer_frames < 0) {
        err = transfer_frames;

        if (snd_pcm_recover(handle, transfer_frames, 0) < 0) {
          fprintf(stderr, "Write transfer error: %s\n", snd_strerror(err));
          goto clean;
        }

        break;
      }

      frame_buffer += (transfer_frames * frame_bytes);
      frame_count  -= transfer_frames;
    }

    playback_frames += read_frames;

    rest_frames = total_frames - playback_frames;

    if (rest_frames <= (long)period_size) {
      total_bytes = (long)(rest_frames * frame_bytes);
    }
  }

  snd_pcm_drop(handle);

  printf("Summary: %lu frame played\n", playback_frames);

clean:
  if (frame_block != NULL) {
    free(frame_block);
  }

  return err;
}

static void usage(void) {
  printf("Usage: ./wave_rw_play_uchar [options] ... [sound file] ...\n");
  printf("-h, --help                usage\n");
  printf("-D, --device=devicename   playback device\n");
  printf("-m, --mmap                mmap_write transfer\n");
  printf("-v, --verbose             show parameters\n");
  printf("-n, --noresample          prohibit resample\n\n");

  printf("Applicable sample formats:");

  for (int k = 0; k < SND_PCM_FORMAT_LAST; k++) {
    const char *s = snd_pcm_format_name((snd_pcm_format_t)k);

    if (s) {
      printf(" %s", s);
    }
  }

  printf("\n");
}
