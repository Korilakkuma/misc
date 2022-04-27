#include <iostream>
#include <stdlib.h>
#include "wave.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Require IN/OUT WAVE file name" << std::endl;
        exit(EXIT_FAILURE);
    }

    STEREO_PCM pcm0, pcm1;

    WAVE::wave_read(&pcm0, argv[1]);

    pcm1.fs = pcm0.fs;
    pcm1.bits = pcm0.bits;
    pcm1.length = pcm0.length;

    pcm1.sL.reserve(pcm1.length);
    pcm1.sR.reserve(pcm1.length);

    for (int n = 0; n < pcm1.length; n++) {
        pcm1.sL[n] = pcm0.sL[n];
        pcm1.sR[n] = pcm0.sR[n];
    }

    WAVE::wave_write(&pcm1, argv[2]);

    return 0;
}
