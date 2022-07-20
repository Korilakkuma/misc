#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <errno.h>
#include <alsa/asoundlib.h>
#include "WaveFormat.h"

static int wave_read_header(void);
static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static int direct_uchar(snd_pcm_t *handle);
static void usage(void);

static char *device = "plughw:0,0";
static snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;
static DWORD rate = 44100;
static DWORD channels = 1;
static DWORD buffer_time = 0;
static DWORD period_time = 0;
static snd_pcm_uframes_t buffer_size = 0;
static snd_pcm_uframes_t period_size = 0;
static snd_output_t *output = NULL;

static bool verbose  = false;
static bool resample = true;

static WAVEFormatDesc fmt_desc;
static WAVEFileDesc file_desc;

int main(int argc, char **argv) {
  static const char *optstring = "hD:mvn";

  static const struct option long_option[] = {
    { "help",       no_argument,       NULL, 'h' },
    { "device",     required_argument, NULL, 'D' },
    { "verbose",    no_argument,       NULL, 'v' },
    { "noresample", no_argument,       NULL, 'n' },
    { 0,            0,                 0,     0  }
  };

  int long_index;

  snd_pcm_t *handle = NULL;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;

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
      case 'v':
        verbose = true;
        break;
      case 'n':
        resample = false;
        break;
      default:
        fprintf(stderr, "`--help` confirm usage\n");
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

  err = direct_uchar(handle);

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

  if (s == -1) {
    exit(EXIT_FAILURE);
  }

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

  err = snd_pcm_hw_params_set_access(handle, hwparams, SND_PCM_ACCESS_MMAP_INTERLEAVED);

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

static int direct_uchar(snd_pcm_t *handle) {
  const snd_pcm_channel_area_t *areas;

  snd_pcm_uframes_t offset;
  snd_pcm_uframes_t frames;

  snd_pcm_sframes_t avail;
  snd_pcm_sframes_t transfer_frames;

  BYTE *frame_block;
  BYTE *mmap[4];

  const long total_frames = file_desc.frame_size;
  const WORD frame_bytes  = fmt_desc.data_frame_size;

  // HACK:
  period_size = total_frames;

  long number_of_frames = (long)period_size;
  long playback_frames  = 0;
  long rest_frames      = total_frames;

  bool can_start = true;

  int i   = 0;
  int err = 0;

  while (rest_frames > 0) {
    avail = snd_pcm_avail_update(handle);

    if (avail < 0) {
      if ((err = snd_pcm_recover(handle, (int)avail, 0)) < 0) {
        fprintf(stderr, "Reading writable frames is failed: %s\n", snd_strerror(err));
        return err;
      }

      can_start = true;
      continue;
    }

    if (avail < number_of_frames) {
      if (can_start) {
        can_start = false;

        err = snd_pcm_start(handle);

        if (err < 0) {
          fprintf(stderr, "Starting PCM is failed: %s\n", snd_strerror(err));
          return err;
        }
      } else {
        err = snd_pcm_wait(handle, -1);

        if (err < 0) {
          if ((err = snd_pcm_recover(handle, err, 0)) < 0) {
            fprintf(stderr, "Waiting PCM is failed: %s\n", snd_strerror(err));
            return err;
          }

          can_start = true;
        }
      }

      continue;
    }

    frames = (snd_pcm_uframes_t)number_of_frames;

    err = snd_pcm_mmap_begin(handle, &areas, &offset, &frames);

    if (err < 0) {
      if ((err = snd_pcm_recover(handle, err, 0)) < 0) {
        fprintf(stderr, "Access to mmap is failed: %s\n", snd_strerror(err));
        return err;
      }

      can_start = true;
    }

    if (i < 4) {
      mmap[i]  = (BYTE *)(areas->addr + (areas->first / 8));
      mmap[i] += (DWORD)(offset * (areas->step / 8));
    }

    frame_block = mmap[i % 4];

    long read_frames = (long)read(file_desc.fd, frame_block, (size_t)((frames * frame_bytes) / frame_bytes));

    if (read_frames == -1) {
      return read_frames;
    }

    transfer_frames = snd_pcm_mmap_commit(handle, offset, (snd_pcm_uframes_t)read_frames);

    if ((transfer_frames < 0) || ((snd_pcm_uframes_t)transfer_frames != frames)) {
      if ((err = snd_pcm_recover(handle, (transfer_frames >= 0 ? -EPIPE : (int)transfer_frames), 0)) < 0){
        fprintf(stderr, "Commit to mmap is failed: %s\n", snd_strerror(err));
        return err;
      }

      can_start = true;
    }

    playback_frames += (long)transfer_frames;

    rest_frames = total_frames - playback_frames;

    if (rest_frames <= (long)period_size) {
      number_of_frames = rest_frames;
    }

    i++;
  }

  printf("Summary: %lu frame played\n", playback_frames);

  return 0;
}

static void usage(void) {
  printf("Usage: ./wave_direct_play_uchar [options] ... [sound file] ...\n");
  printf("-h, --help                usage\n");
  printf("-D, --device=devicename   playback device\n");
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
