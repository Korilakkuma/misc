#include <cstdlib>
#include <getopt.h>
#include <pthread.h>
#include "alsa/asoundlib.h"
#include "sndfile.h"
#include "FL/Fl.H"
#include "FL/Fl_Window.H"
#include "FL/Fl_File_Chooser.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Box.H"
#include "FL/fl_ask.H"
#include "FL/Fl_Menu_Bar.H"
#include "FL/Fl_Menu_Item.H"
#include "FL/Fl_Choice.H"
#include "FL/Fl_Hor_Value_Slider.H"

static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams);
static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams);
static int gui_write_int(snd_pcm_t *handle);
static void *player(void *arg);
static snd_pcm_sframes_t (*writei_func)(snd_pcm_t *handle, const void *buffer, snd_pcm_uframes_t size);

// static std::string device = "plughw:0,0";
static char *device = "plughw:0,0";
static snd_pcm_format_t format = SND_PCM_FORMAT_S32_LE;
static unsigned int rate = 44100;
static unsigned int number_of_channels = 1;
static unsigned int buffer_time = 0;
static unsigned int period_time = 0;
static snd_pcm_uframes_t buffer_size = 0;
static snd_pcm_uframes_t period_size = 0;
static snd_output_t *output = NULL;

static bool mmap     = false;
static bool resample = true;

static char file_path[256] = { 0 };

static bool is_play  = false;
static bool is_pause = false;

static pthread_t play_thread;

static SNDFILE *infile;
static SF_INFO *infile_info;

static Fl_Window *main_window;
static Fl_File_Chooser *file_dialog;
static Fl_Choice *pcm_device;
static Fl_Box *player_file;
static Fl_Box *player_state;
static Fl_Hor_Value_Slider *time_bar;

static void load_file_callback(Fl_Menu_Item *w, void *d);
static void exit_callback(Fl_Menu_Item *w, void *d);
static void play_callback(Fl_Button *w, void *d);
static void pause_callback(Fl_Button *w, void *d);
static void pcm_device_callback(Fl_Choice *w, void *d);

int main() {
  main_window = new Fl_Window(0, 0, 400, 350, "ALSA Player");

  Fl_Menu_Item file_item[] = {
    { "File",            0,                                     0, 0, FL_SUBMENU, FL_NORMAL_LABEL, 0, 14, 0 },
    { "Open",  FL_ALT + 'o', (Fl_Callback *)load_file_callback, 0, 0,             FL_NORMAL_LABEL, 0, 14, 0 },
    { "Close", FL_ALT + 'x', (Fl_Callback *)exit_callback,      0, 0,             FL_NORMAL_LABEL, 0, 14, 0 },
    { 0 },
    { 0 }
  };

  Fl_Menu_Bar *menu = new Fl_Menu_Bar(0, 0, 80, 35);

  menu->menu(file_item);

  player_file = new Fl_Box(25, 40, 350, 40, "--- Audio File ---");

  Fl_Menu_Item device_item[] = {
    { "plughw:0,0", 0, 0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0 },
    { "hw:0,0",     0, 0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0 },
    { "plughw:1,0", 0, 0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0 },
    { "hw:1,0",     0, 0, 0, 0, FL_NORMAL_LABEL, 0, 14, 0 }
  };

  pcm_device = new Fl_Choice(175, 85, 115, 30, "PCM devices");

  pcm_device->down_box(FL_BORDER_BOX);
  pcm_device->callback((Fl_Callback *)pcm_device_callback);
  pcm_device->menu(device_item);

  player_state = new Fl_Box(150, 130, 100, 35, "--- Player State ---");

  Fl_Button *play_button = new Fl_Button(90, 180, 90, 45, "@>");

  play_button->callback((Fl_Callback *)play_callback);

  Fl_Button *pause_button = new Fl_Button(220, 180, 90, 45, "@square");

  pause_button->callback((Fl_Callback *)pause_callback);

  new Fl_Box(115, 225, 50, 15, "Play");
  new Fl_Box(245, 225, 50, 15, "Play");

  time_bar = new Fl_Hor_Value_Slider(50, 270, 300, 30);

  time_bar->type(FL_HOR_FILL_SLIDER);
  time_bar->selection_color(FL_BLUE);

  main_window->end();

  file_dialog = new Fl_File_Chooser(".", "Audio File (wav, aif, aiff, flac)", Fl_File_Chooser::SINGLE, "Open Audio File");

  main_window->show();

  Fl::lock();

  return Fl::run();
}

static int set_hwparams(snd_pcm_t *handle, snd_pcm_hw_params_t *hwparams) {
  unsigned int rate_near;
  int dir;
  int err;

  err = snd_pcm_hw_params_any(handle, hwparams);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_set_rate_resample(handle, hwparams, resample ? 1 : 0);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_set_access(handle, hwparams, mmap ? SND_PCM_ACCESS_MMAP_INTERLEAVED : SND_PCM_ACCESS_RW_INTERLEAVED);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_set_format(handle, hwparams, format);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_set_channels(handle, hwparams, number_of_channels);

  if (err < 0) {
    return err;
  }

  rate_near = rate;

  err = snd_pcm_hw_params_set_rate_near(handle, hwparams, &rate_near, 0);

  if (err < 0) {
    return err;
  }

  if (rate_near != rate) {
    return -EINVAL;
  }

  err = snd_pcm_hw_params_get_buffer_time_max(hwparams, &buffer_time, &dir);

  if (buffer_time > 500000) {
    buffer_time = 500000;
  }

  if (buffer_time > 0) {
    period_time = buffer_time / 4;
  } else {
    return -EINVAL;
  }

  err = snd_pcm_hw_params_set_buffer_time_near(handle, hwparams, &buffer_time, &dir);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_set_period_time_near(handle, hwparams, &period_time, &dir);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params(handle, hwparams);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_get_buffer_size(hwparams, &buffer_size);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_hw_params_get_period_size(hwparams, &period_size, &dir);

  if (err < 0) {
    return err;
  }

  return 0;
}

static int set_swparams(snd_pcm_t *handle, snd_pcm_sw_params_t *swparams) {
  int err;

  err = snd_pcm_sw_params_current(handle, swparams);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_sw_params_set_start_threshold(handle, swparams, ((buffer_size / period_size) * period_size));

  if (err < 0) {
    return err;
  }

  err = snd_pcm_sw_params_set_avail_min(handle, swparams, period_size);

  if (err < 0) {
    return err;
  }

  err = snd_pcm_sw_params(handle, swparams);

  if (err < 0) {
    return err;
  }

  return 0;
}

static int gui_write_int(snd_pcm_t *handle) {
  int *buf_ptr;
  const long number_of_sound_frames = infile_info->frames;
  long number_of_frames;
  long frame_count;
  long number_of_playback_frames = 0;
  long read_frames;
  long rest_frames = number_of_sound_frames;
  int bytes;

  int *frame_block = (int *)malloc(period_size * sizeof(int) * number_of_channels);

  if (frame_block == NULL) {
    return EXIT_FAILURE;
  }

  rate = infile_info->samplerate;

  Fl::lock();
  time_bar->range(0.0, ((double)number_of_sound_frames / (double)rate));
  Fl::unlock();

  number_of_frames = period_size;

  while ((rest_frames > 0) && !is_pause) {
    read_frames = (long)sf_readf_int(infile, frame_block, (snd_pcm_uframes_t)number_of_frames);

    if (read_frames < 0) {
      return EXIT_FAILURE;
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
          return EXIT_FAILURE;
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

    Fl::lock();
    time_bar->value((double)number_of_playback_frames / (double)rate);
    Fl::awake();
    Fl::unlock();
  }

  snd_pcm_drop(handle);

  if (!is_pause) {
    Fl::lock();
    player_state->label("Ended");
    Fl::awake();
    Fl::unlock();
  }

  return 0;
}

static void *player(void *arg) {
  snd_pcm_t *handle = NULL;
  snd_pcm_hw_params_t *hwparams;
  snd_pcm_sw_params_t *swparams;

  int err;

  is_play = true;

  snd_pcm_hw_params_alloca(&hwparams);
  snd_pcm_sw_params_alloca(&swparams);

  if (mmap) {
    writei_func = snd_pcm_mmap_writei;
  } else {
    writei_func = snd_pcm_writei;
  }

  if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
    goto clean;
  }

  if ((err = set_hwparams(handle, hwparams)) < 0) {
    snd_pcm_hw_params_dump(hwparams, output);
    goto clean;
  }

  if ((err = set_swparams(handle, swparams)) < 0) {
    snd_pcm_sw_params_dump(swparams, output);
    goto clean;
  }

  if (!(infile = sf_open(file_path, SFM_READ, infile_info))) {
    goto clean;
  }

  err = gui_write_int(handle);

clean:
  is_play  = false;
  is_pause = false;

  if (output != NULL) {
    snd_output_close(output);
  }

  if (handle != NULL) {
    snd_pcm_close(handle);
  }

  snd_config_update_free_global();

  return ((void *)0);
}

static void load_file_callback(Fl_Menu_Item *w, void *d) {
  static char current_dir[256] = "/home";

  file_dialog->directory(current_dir);
  file_dialog->preview(0);
  file_dialog->show();

  while (file_dialog->visible()) {
    Fl::wait();
  }

  strcpy(current_dir, file_dialog->directory());

  if ((file_dialog->count() > 0) && (current_dir[0] != '\0')) {
    strcpy(file_path, file_dialog->value());
    player_file->label(file_path);
  }

  return;
}

static void exit_callback(Fl_Menu_Item *w, void *d) {
  main_window->hide();
  return;
}

static void play_callback(Fl_Button *w, void *d) {
  if (file_path[0] == '\0') {
    fl_message("Not select file");
    return;
  }

  if (is_play) {
    fl_message("Pause by clicking pause button");
  } else {
    int device_index = pcm_device->value();

    switch (device_index) {
      case 0:
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      default:
        break;
    }

    player_state->label("Playing");
    pthread_create(&play_thread, NULL, player, NULL);
  }
}

static void pause_callback(Fl_Button *w, void *d) {
  if (is_play) {
    is_pause = true;
    player_state->label("Paused");
  }
}

static void pcm_device_callback(Fl_Choice *w, void *d) {
  if (is_play) {
    fl_message("Apply to next");
  }

  return;
}
