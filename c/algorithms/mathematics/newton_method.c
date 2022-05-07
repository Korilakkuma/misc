#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define INT long long

static double quadratic_function(double n, INT c);
static double cubic_function(double n, INT c);
static double exponential(double n, INT c);

int main(int argc, char **argv) {
  if (argc != 3) {
    fputs("./newton_method [natural number] [natural number]\n", stderr);
    exit(EXIT_FAILURE);
  }

  double n = strtod(argv[1], NULL);
  INT c = strtoll(argv[2], NULL, 10);

  printf("x^2 = %f\n", quadratic_function(n, c));
  printf("x^3 = %f\n", cubic_function(n, c));
  printf("e^x = %f\n", exponential(n, c));

  return 0;
}

static double quadratic_function(double n, INT c) {
  double a = n;

  for (INT i = 0; i < c; i++) {
    double x = a;
    double y = pow(x, 2.0);

    double dc = 2.0 * pow(x, 1.0);
    double ct = y - (dc * x);

    a = (n - ct) / dc;
  }

  return a;
}

static double cubic_function(double n, INT c) {
  double a = n;

  for (INT i = 0; i < c; i++) {
    double x = a;
    double y = pow(x, 3.0);

    double dc = 3.0 * pow(x, 2.0);
    double ct = y - (dc * x);

    a = (n - ct) / dc;
  }

  return a;
}

static double exponential(double n, INT c) {
  double a = n;

  for (INT i = 0; i < c; i++) {
    double x = a;
    double y = exp(x);

    double dc = exp(x);
    double ct = y - (dc * x);

    a = (n - ct) / dc;
  }

  return a;
}
