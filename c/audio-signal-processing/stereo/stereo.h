void Stereo(
  double delay,
  double *inL,
  double *inR,
  double *outL,
  double *outR,
  int fs,
  int length
) {
  double d = fs * delay;

  for (int n = 0; n < length; n++) {
    int m = (int)((double)n - d);

    if (m >= 0) {
      outL[n] = inL[n] + inL[m];
      outR[n] = inR[n] - inR[m];
    }
  }
}
