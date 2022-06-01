#include<stdint.h>

#define FORMAT_CHUNK_PCM_SIZE 16
#define FORMAT_CHUNK_EX_SIZE 18
#define FORMAT_CHUNK_EXTENSIBLE_SIZE 40

#define WAVE_FORMAT_PCM 0x0001  // 2 byte + 14 byte = 16 byte
#define WAVE_FORMAT_EXTENSIBLE 0xfffe
#define WAVE_GUID_TAG "\x00\x00\x00\x00\x10\x00\x80\x00\x00\xAA\x00\x38\x9B\x71"

static char RIFF_ID[4] = { 'R', 'I', 'F', 'F' };
static char WAVE_ID[4] = { 'W', 'A', 'V', 'E' };
static char FMT_ID[4]  = { 'f', 'm', 't', ' ' };
static char DATA_ID[4] = { 'd', 'a', 't', 'a' };

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef DWORD    FOURCC;

typedef struct GUID {
  WORD sub_format_code;
  BYTE wave_guid_tag[14];
} GUID;

typedef struct FormatDescriptor {
  WORD  format_tag;
  WORD  number_of_channels;
  DWORD samples_per_sec;
  DWORD avg_bytes_per_sec;
  WORD  data_frame_size;
  WORD  bits_per_sample;
} WAVEFormatDesc;

typedef struct WAVEFileDescriptor {
  int  fd;
  long frame_size;
} WAVEFileDesc;
