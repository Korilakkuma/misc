#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int fs;
    int bits;
    int length;
    double *s;
} MONO_PCM;

typedef struct {
    int fs;
    int bits;
    int length;
    double *sL;
    double *sR;
} STEREO_PCM;

void mono_wave_read(MONO_PCM *pcm, char *filename) {
    FILE *fp;
    char riff_chunk_id[4];
    long riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_id[4];
    long fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    long fmt_samples_per_sec;
    long fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_id[4];
    int data_chunk_size;
    short data;

    if ((fp = fopen(filename, "rb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fread(riff_chunk_id, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(riff_form_type, 1, 4, fp);
    fread(fmt_chunk_id, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&fmt_wave_format_type, 2, 1, fp);
    fread(&fmt_channel, 2, 1, fp);
    fread(&fmt_samples_per_sec, 4, 1, fp);
    fread(&fmt_bytes_per_sec, 4, 1, fp);
    fread(&fmt_block_size, 2, 1, fp);
    fread(&fmt_bits_per_sample, 2, 1, fp);
    fread(data_chunk_id, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = fmt_samples_per_sec;
    pcm->bits = fmt_bits_per_sample;
    pcm->length = data_chunk_size / (1 * (pcm->bits / 8));

    fputs("Read ...\n", stdout);
    printf("Sample Rate: %d\n", pcm->fs);
    printf("Quantization Bit: %d\n", pcm->bits);
    printf("Length: %d\n", pcm->length);

    if ((pcm->s = (double *)calloc(pcm->length, sizeof(double))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (int n = 0; n < pcm->length; n++) {
        fread(&data, sizeof(short), 1, fp);
        pcm->s[n] = (double)data / 32768.0;
    }

    if (fclose(fp) == EOF) {
        perror("fclose");
        free(pcm->s);
        exit(EXIT_FAILURE);
    }
}

void mono_wave_write(MONO_PCM *pcm, char *filename) {
    FILE *fp;
    char riff_chunk_id[4];
    long riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_id[4];
    long fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    long fmt_samples_per_sec;
    long fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_id[4];
    int data_chunk_size;
    short data;
    double s;

    riff_chunk_id[0] = 'R';
    riff_chunk_id[1] = 'I';
    riff_chunk_id[2] = 'F';
    riff_chunk_id[3] = 'F';

    riff_chunk_size = 36 + (1 * (16 / 8) * pcm->length);

    riff_form_type[0] = 'W';
    riff_form_type[1] = 'A';
    riff_form_type[2] = 'V';
    riff_form_type[3] = 'E';

    fmt_chunk_id[0] = 'f';
    fmt_chunk_id[1] = 'm';
    fmt_chunk_id[2] = 't';
    fmt_chunk_id[3] = ' ';

    fmt_chunk_size = 16;
    fmt_wave_format_type = 1;
    fmt_channel = 1;
    fmt_samples_per_sec = pcm->fs;
    fmt_bytes_per_sec = pcm->fs * (1 * (pcm->bits / 8));
    fmt_block_size = 1 * (pcm->bits / 8);
    fmt_bits_per_sample = pcm->bits;

    data_chunk_id[0] = 'd';
    data_chunk_id[1] = 'a';
    data_chunk_id[2] = 't';
    data_chunk_id[3] = 'a';

    data_chunk_size = 1 * (16 / 8) * pcm->length;

    if ((fp = fopen(filename, "wb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fwrite(riff_chunk_id, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(riff_form_type, 1, 4, fp);
    fwrite(fmt_chunk_id, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&fmt_wave_format_type, 2, 1, fp);
    fwrite(&fmt_channel, 2, 1, fp);
    fwrite(&fmt_samples_per_sec, 4, 1, fp);
    fwrite(&fmt_bytes_per_sec, 4, 1, fp);
    fwrite(&fmt_block_size, 2, 1, fp);
    fwrite(&fmt_bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_id, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    fputs("Write ...\n", stdout);
    printf("Sample Rate: %d\n", pcm->fs);
    printf("Quantization Bit: %d\n", pcm->bits);
    printf("Length: %d\n", pcm->length);

    for (int n = 0; n < pcm->length; n++) {
        s = ((pcm->s[n] + 1.0) / 2.0) * 65536.0;

        if (s > 65535.0) {
            s = 65535.0;
        }

        if (s < 0.0) {
            s = 0.0;
        }

        data = (short)((s + 0.5) - 32768);

        fwrite(&data, 2, 1, fp);
    }

    if (fclose(fp) == EOF) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
}

void stereo_wave_read(STEREO_PCM *pcm, char *filename) {
    FILE *fp;
    char riff_chunk_id[4];
    long riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_id[4];
    long fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    long fmt_samples_per_sec;
    long fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_id[4];
    int data_chunk_size;
    short dataL;
    short dataR;

    if ((fp = fopen(filename, "rb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fread(riff_chunk_id, 1, 4, fp);
    fread(&riff_chunk_size, 4, 1, fp);
    fread(riff_form_type, 1, 4, fp);
    fread(fmt_chunk_id, 1, 4, fp);
    fread(&fmt_chunk_size, 4, 1, fp);
    fread(&fmt_wave_format_type, 2, 1, fp);
    fread(&fmt_channel, 2, 1, fp);
    fread(&fmt_samples_per_sec, 4, 1, fp);
    fread(&fmt_bytes_per_sec, 4, 1, fp);
    fread(&fmt_block_size, 2, 1, fp);
    fread(&fmt_bits_per_sample, 2, 1, fp);
    fread(data_chunk_id, 1, 4, fp);
    fread(&data_chunk_size, 4, 1, fp);

    pcm->fs = fmt_samples_per_sec;
    pcm->bits = fmt_bits_per_sample;
    pcm->length = data_chunk_size / (2 * (pcm->bits / 8));

    fputs("Read ...\n", stdout);
    printf("Sample Rate: %d\n", pcm->fs);
    printf("Quantization Bit: %d\n", pcm->bits);
    printf("Length: %d\n", pcm->length);

    if ((pcm->sL = (double *)calloc(pcm->length, sizeof(double))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    if ((pcm->sR = (double *)calloc(pcm->length, sizeof(double))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (int n = 0; n < pcm->length; n++) {
        fread(&dataL, 2, 1, fp);
        fread(&dataR, 2, 1, fp);

        pcm->sL[n] = (double)dataL / 32768.0;
        pcm->sR[n] = (double)dataR / 32768.0;
    }

    if (fclose(fp) == EOF) {
        perror("fclose");
        free(pcm->sL);
        free(pcm->sR);
        exit(EXIT_FAILURE);
    }
}

void stereo_wave_write(STEREO_PCM *pcm, char *filename) {
    FILE *fp;
    char riff_chunk_id[4];
    long riff_chunk_size;
    char riff_form_type[4];
    char fmt_chunk_id[4];
    long fmt_chunk_size;
    short fmt_wave_format_type;
    short fmt_channel;
    long fmt_samples_per_sec;
    long fmt_bytes_per_sec;
    short fmt_block_size;
    short fmt_bits_per_sample;
    char data_chunk_id[4];
    int data_chunk_size;
    short dataL;
    short dataR;
    double sL;
    double sR;

    riff_chunk_id[0] = 'R';
    riff_chunk_id[1] = 'I';
    riff_chunk_id[2] = 'F';
    riff_chunk_id[3] = 'F';

    riff_chunk_size = 36 + (2 * (16 / 8) * pcm->length);

    riff_form_type[0] = 'W';
    riff_form_type[1] = 'A';
    riff_form_type[2] = 'V';
    riff_form_type[3] = 'E';

    fmt_chunk_id[0] = 'f';
    fmt_chunk_id[1] = 'm';
    fmt_chunk_id[2] = 't';
    fmt_chunk_id[3] = ' ';

    fmt_chunk_size = 16;
    fmt_wave_format_type = 1;
    fmt_channel = 2;
    fmt_samples_per_sec = pcm->fs;
    fmt_bytes_per_sec = pcm->fs * (2 * (pcm->bits / 8)) ;
    fmt_block_size = 2 * (pcm->bits / 8);
    fmt_bits_per_sample = pcm->bits;

    data_chunk_id[0] = 'd';
    data_chunk_id[1] = 'a';
    data_chunk_id[2] = 't';
    data_chunk_id[3] = 'a';

    data_chunk_size = 2 * (16 / 8) * pcm->length;

    if ((fp = fopen(filename, "wb")) == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    fwrite(riff_chunk_id, 1, 4, fp);
    fwrite(&riff_chunk_size, 4, 1, fp);
    fwrite(riff_form_type, 1, 4, fp);
    fwrite(fmt_chunk_id, 1, 4, fp);
    fwrite(&fmt_chunk_size, 4, 1, fp);
    fwrite(&fmt_wave_format_type, 2, 1, fp);
    fwrite(&fmt_channel, 2, 1, fp);
    fwrite(&fmt_samples_per_sec, 4, 1, fp);
    fwrite(&fmt_bytes_per_sec, 4, 1, fp);
    fwrite(&fmt_block_size, 2, 1, fp);
    fwrite(&fmt_bits_per_sample, 2, 1, fp);
    fwrite(data_chunk_id, 1, 4, fp);
    fwrite(&data_chunk_size, 4, 1, fp);

    fputs("Write ...\n", stdout);
    printf("Sample Rate: %d\n", pcm->fs);
    printf("Quantization Bit: %d\n", pcm->bits);
    printf("Length: %d\n", pcm->length);

    for (int n = 0; n < pcm->length; n++) {
        sL = ((pcm->sL[n] + 1.0) / 2.0) * 65536.0;
        sR = ((pcm->sR[n] + 1.0) / 2.0) * 65536.0;

        if (sL > 65535.0) {
            sL = 65535.0;
        }

        if (sL < 0.0) {
            sL = 0.0;
        }

        if (sR > 65535.0) {
            sR = 65535.0;
        }

        if (sR < 0.0) {
            sR = 0.0;
        }

        dataL = (short)((sL + 0.5) - 32768);
        dataR = (short)((sR + 0.5) - 32768);

        fwrite(&dataL, 2, 1, fp);
        fwrite(&dataR, 2, 1, fp);
    }

    if (fclose(fp) == EOF) {
        perror("fclose");
        exit(EXIT_FAILURE);
    }
}
