void Compressor(
  std::vector<double> &dryL,
  std::vector<double> &dryR,
  int length,
  double threshold,
  double ratio) {
  double gain = 1.0 / ((threshold + (ratio * (1.0 - threshold))));

  for (int n = 0; n < length; n++) {
    if (dryL[n] > threshold) {
      dryL[n] = threshold + (ratio * (dryL[n] - threshold));
    }

    if (dryL[n] < -threshold) {
      dryL[n] = -threshold + (ratio * (dryL[n] + threshold));
    }

    dryL[n] = dryL[n] * gain;

    if (dryR[n] > threshold) {
      dryR[n] = threshold + (ratio * (dryR[n] - threshold));
    }

    if (dryR[n] < -threshold) {
      dryR[n] = -threshold + (ratio * (dryR[n] + threshold));
    }

    dryR[n] = dryR[n] * gain;
  }
}
