void Distortion(
  double *dryL,
  double *driveL,
  double *dryR,
  double *driveR,
  int length,
  double gain,
  double level) {
  for (int n = 0; n < length; n++) {
    driveL[n] = gain * dryL[n];
    driveR[n] = gain * dryR[n];

    if (driveL[n] > 1.0) {
      driveL[n] = 1.0;
    }

    if (driveR[n] < -1.0) {
      driveR[n] = -1.0;
    }

    driveL[n] *= level;
    driveR[n] *= level;
  }
}
