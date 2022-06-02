#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <alsa/asoundlib.h>
#include <FLAC/stream_decoder.h>
#include <FLAC/metadata.h>

typedef enum {
  FALSE,
  TRUE
} BOOL;

static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static void buffer2block(void);
static int flac_write_int(snd_pcm_t *handle);
static long flac_read_int_frames(int *datablock, long number_of_frames);

static char *device = "plughw:0,0";
static snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;
static unsigned int rate = 44100;
static unsigned int number_of_channels = 1;
static unsigned int buffer_time = 0;
static unsigned int period_time = 0;
static snd_pcm_uframes_t buffer_size = 0;
static snd_pcm_uframes_t period_size = 0;
static snd_output_t *output = NULL;

static FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *user_data);
static void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *user_data);
static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *user_data);

static void usage(void);

static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);

static BOOL mmap     = FALSE;
static BOOL verbose  = FALSE;
static BOOL resample = TRUE;

typedef struct {
  FLAC__StreamDecoder *decoder;
  FLAC__uint64 total_frames;
  const FLAC__Frame *frame;
  const FLAC__int32 *const *dec_buffer;
  int buffer_pos;
  int size_spec;
  int block_pos;
  long counter;
  unsigned int qbits;
  void *datablock;
} FLAC_DECODER;

static FLAC_DECODER dflac;

int main(int argc, char **argv) {
  const char *optstring = "hD:mvn";

  const struct option long_option[] = {
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
        mmap = TRUE;
        break;
      case 'v':
        verbose = TRUE;
        break;
      case 'n':
        resample = FALSE;
        break;
      default:
        break;
    }
  }

  if (optind > (argc - 1)) {
    usage();
    return 0;
  }

  FLAC__bool success = true;
  FLAC__StreamDecoderInitStatus init_status;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);

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

  const char *file_path = NULL;

  file_path = argv[optind];

  int fd = open(file_path, O_RDONLY);

  if (fd < 0) {
    fputs("Open FLAC File Error\n", stderr);
    status_code = errno;
    goto clean;
  }

  printf("=== FLAC file info ===\n");
  printf("Filename: %s\n", file_path);

  if ((dflac.decoder = FLAC__stream_decoder_new()) == NULL) {
    fprintf(stderr, "Assigning decoder failed\n");
    status_code = EXIT_FAILURE;
    goto clean;
  }

  init_status = FLAC__stream_decoder_init_file(dflac.decoder, file_path, write_callback, metadata_callback, error_callback, &dflac);

  if (init_status != FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    fprintf(stderr, "Assigning decoder failed: %s\n", FLAC__StreamDecoderInitStatusString[init_status]);
    status_code = EXIT_FAILURE;
    goto clean;
  }

  success = FLAC__stream_decoder_process_until_end_of_metadata(dflac.decoder);

  if (!success) {
    fprintf(stderr, "Decoding metadata failed\n");
    fprintf(stderr, "Decoder status is: %s\n", FLAC__StreamDecoderStateString[FLAC__stream_decoder_get_state(dflac.decoder)]);
    status_code = EXIT_FAILURE;
    goto clean;
  }

  double play_time = (double)dflac.total_frames / (double)rate;

  printf("Duration: %.01f sec\n\n", play_time);

  status_code = flac_write_int(handle);

  if (status_code != 0) {
    fprintf(stderr, "Playback transfer failed\n");
    goto clean;
  }

clean:
  if (init_status == FLAC__STREAM_DECODER_INIT_STATUS_OK) {
    FLAC__stream_decoder_finish(dflac.decoder);
  }

  if (dflac.decoder != NULL) {
    FLAC__stream_decoder_delete(dflac.decoder);
  }

  if (output != NULL) {
    snd_output_close(output);
  }

  if (handle != NULL) {
    snd_pcm_close(handle);
  }

  snd_config_update_free_global();

  return status_code;
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

  err = snd_pcm_hw_params_set_channels(handle, hwparams, number_of_channels);

  if (err < 0) {
    fprintf(stderr, "The number of channels (%i) is not applicable: %s\n", number_of_channels, snd_strerror(err));
    return err;
  }

  rate_near = rate;

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

static void buffer2block(void) {
  const FLAC__Frame *frame = dflac.frame;
  const FLAC__int32 *const *dec_buffer = dflac.dec_buffer;

  int *datablock = (int *)dflac.datablock;

  unsigned int shift = 32 - frame->header.bits_per_sample;

  for (int i = 0; (i < (int)frame->header.blocksize) && (dflac.counter > 0); i++) {
    if (dflac.buffer_pos >= (int)frame->header.blocksize) {
      break;
    }

    for (int j = 0; j < (int)frame->header.channels; j++) {
      datablock[dflac.block_pos + j] = dec_buffer[j][dflac.buffer_pos] << shift;
    }

    dflac.block_pos += (int)frame->header.channels;
    dflac.counter--;
    dflac.buffer_pos++;
  }

  return;
}

static int flac_write_int(snd_pcm_t *handle) {
  int *buf_ptr;
  const long number_of_sound_frames = (long)dflac.total_frames;
  long number_of_frames;
  long frame_count;
  long number_of_playback_frames = 0;
  long read_frames;
  long rest_frames = number_of_sound_frames;
  int bytes;

  int err = 0;

  dflac.datablock = NULL;

  int *frame_block = (int *)malloc(period_size * sizeof(int) * number_of_channels);

  if (frame_block == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    err = EXIT_FAILURE;
    goto clean;
  }

  number_of_frames = (long)period_size;

  while (rest_frames > 0) {
    read_frames = flac_read_int_frames(frame_block, number_of_frames);

    if (read_frames < 0) {
      fprintf(stderr, "Abort decoding FLAC by error\n");
      err = EXIT_FAILURE;
      goto clean;
    }

    frame_count = read_frames;

    buf_ptr = frame_block;

    while (frame_count > 0) {
      bytes = (int)writei_func(handle, buf_ptr, (snd_pcm_uframes_t)frame_count);

      if (bytes == -EAGAIN) {
        continue;
      }

      if (bytes < 0) {
        if (snd_pcm_recover(handle, bytes, 0) < 0) {
          fprintf(stderr, "Write transfer error: %s\n", snd_strerror(bytes));
          goto clean;
        }

        break;
      }

      buf_ptr     += (bytes * number_of_channels);
      frame_count -= bytes;
    }

    number_of_playback_frames += read_frames;

    if ((rest_frames = number_of_sound_frames - number_of_playback_frames) <= (long)period_size) {
      number_of_frames = rest_frames;
    }
  }


  snd_pcm_drop(handle);

  printf("Summary: %lu frame played\n", number_of_playback_frames);

  err = 0;

clean:
  if (frame_block != NULL) {
    free(frame_block);
  }

  return err;
}

static long flac_read_int_frames(int *datablock, long number_of_frames) {
  dflac.block_pos = 0;
  dflac.datablock = datablock;
  dflac.counter   = number_of_frames;
  dflac.size_spec = 1;

  if ((dflac.frame != NULL) && (dflac.buffer_pos < dflac.frame->header.blocksize)) {
    buffer2block();
  }

  while (dflac.block_pos < ((int)number_of_frames * (int)number_of_channels)) {
    if (FLAC__stream_decoder_process_single(dflac.decoder) == 0) {
      fprintf(stderr, "Decode error\n");
      return -1;
    }

    if (dflac.size_spec == 0) {
      return -2;
    }

    if (FLAC__stream_decoder_get_state(dflac.decoder) >= FLAC__STREAM_DECODER_END_OF_STREAM) {
      break;
    }
  }

  dflac.datablock = NULL;

  return (long)(dflac.block_pos / (int)number_of_channels);
}

static FLAC__StreamDecoderWriteStatus write_callback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 *const buffer[], void *user_data) {
  dflac.frame      = frame;
  dflac.dec_buffer = buffer;
  dflac.buffer_pos = 0;

  if (frame->header.blocksize > FLAC__MAX_BLOCK_SIZE) {
    fprintf(stderr, "Over FLAC block size (%d) > FLAC__MAX_BLOCK_SIZE (%d)\n", frame->header.blocksize, FLAC__MAX_BLOCK_SIZE);

    dflac.size_spec = 0;

    return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
  }

  buffer2block();

  return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

static void metadata_callback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *user_data) {
  if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO) {
    dflac.total_frames = metadata->data.stream_info.total_samples;
    rate               = metadata->data.stream_info.sample_rate;
    number_of_channels = metadata->data.stream_info.channels;
    dflac.qbits        = metadata->data.stream_info.bits_per_sample;

    printf("Sample Rate: %u Hz\n", rate);
    printf("Number of Channels: %u channels\n", number_of_channels);
    printf("Quantization Bits: %u bits\n", dflac.qbits);
    printf("Number of Frames: %ld\n", (long)dflac.total_frames);
  }

  return;
}

static void error_callback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *user_data) {
  fprintf(stderr, "Decode error: %s\n", FLAC__StreamDecoderErrorStatusString[status]);
  return;
}

static void usage(void) {
  printf("Usage: ./flac_rw_player_int [options] ... [sound file] ...\n");
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
