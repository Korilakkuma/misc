#include <stdio.h>
#include <stdlib.h>
#include "wave.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        fputs("Require WAVE IN/OUT file name\n", stderr);
        exit(EXIT_FAILURE);
    }

    STEREO_PCM pcm0, pcm1;

    stereo_wave_read(&pcm0, argv[1]);

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;

    if ((pcm1.sL = (double *)calloc(pcm1.length, sizeof(double))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    if ((pcm1.sR = (double *)calloc(pcm1.length, sizeof(double))) == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    for (int n = 0; n < pcm1.length; n++) {
        pcm1.sL[n] = pcm0.sL[n];
        pcm1.sR[n] = pcm0.sR[n];
    }

    stereo_wave_write(&pcm1, argv[2]);

    free(pcm0.sL);
    free(pcm0.sR);
    free(pcm1.sL);
    free(pcm1.sR);

    return 0;
}
