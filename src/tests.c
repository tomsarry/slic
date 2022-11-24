#include "helpers.h"

#define EPS 0.1

int test_rgb_to_cielab_conversion() {
  float **r = allocate_float_matrix(2, 2);
  float **g = allocate_float_matrix(2, 2);
  float **b = allocate_float_matrix(2, 2);

  float **cl = allocate_float_matrix(2, 2);
  float **ca = allocate_float_matrix(2, 2);
  float **cb = allocate_float_matrix(2, 2);

  // Color1 : #845ec2
  r[0][0] = 132;
  g[0][0] = 94;
  b[0][0] = 194;

  // Color2 : #d65db1
  r[0][1] = 214;
  g[0][1] = 93;
  b[0][1] = 177;

  // Color3 : #f9f871
  r[1][0] = 249;
  g[1][0] = 248;
  b[1][0] = 113;

  // Color4 : #ffffff
  r[1][1] = 255;
  g[1][1] = 255;
  b[1][1] = 255;

  convert_rgb_to_cielab(cl, ca, cb, r, g, b, 2, 2);


  if (cl[0][0] - 48.0 > EPS ||
      ca[0][0] - 36.8 > EPS ||
      cb[0][0] + 47.0 > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", cl[0][0], ca[0][0], cb[0][0]);
        return 1;
      }

  if (cl[0][1] - 57.4 > EPS ||
      ca[0][1] - 57.2 > EPS ||
      cb[0][1] + 21.8 > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", cl[0][1], ca[0][1], cb[0][1]);
        return 1;
      }

  if (cl[1][0] - 95.4 > EPS ||
      ca[1][0] + 16.7 > EPS ||
      cb[1][0] - 63.7 > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", cl[1][0], ca[1][0], cb[1][0]);
        return 1;
      }

  if (cl[1][1] - 100.0 > EPS ||
      ca[1][1] - 0.01 > EPS ||
      cb[1][1] + 0.01 > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cl[1][1], ca[1][1], cb[1][1]);
        return 1;
      }

  return 0;
}

int test_rgb_to_ciexyz_conversion() {
  float **r = allocate_float_matrix(2, 2);
  float **g = allocate_float_matrix(2, 2);
  float **b = allocate_float_matrix(2, 2);

  float **cx = allocate_float_matrix(2, 2);
  float **cy = allocate_float_matrix(2, 2);
  float **cz = allocate_float_matrix(2, 2);

  // Color1 : #845ec2
  r[0][0] = 132;
  g[0][0] = 94;
  b[0][0] = 194;

  // Color2 : #d65db1
  r[0][1] = 214;
  g[0][1] = 93;
  b[0][1] = 177;

  // Color3 : #f9f871
  r[1][0] = 249;
  g[1][0] = 248;
  b[1][0] = 113;

  // Color4 : #ffffff
  r[1][1] = 255;
  g[1][1] = 255;
  b[1][1] = 255;

  convert_rgb_to_ciexyz(cx, cy, cz, r, g, b, 2, 2);

  if (cx[0][0] - 23.2 > EPS ||
      cy[0][0] - 16.8 > EPS ||
      cz[0][0] - 53.1 > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", cx[0][0], cy[0][0], cz[0][0]);
        return 1;
      }

  if (cx[0][1] - 39.6 > EPS ||
      cy[0][1] - 25.3 > EPS ||
      cz[0][1] - 44.4 > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", cx[0][1], cy[0][1], cz[0][1]);
        return 1;
      }

  if (cx[1][0] - 75.6 > EPS ||
      cy[1][0] - 88.5 > EPS ||
      cz[1][0] - 28.7 > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", cx[1][0], cy[1][0], cz[1][0]);
        return 1;
      }

  if (cx[1][1] - 95.0 > EPS ||
      cy[1][1] - 100. > EPS ||
      cz[1][1] - 109. > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cx[1][1], cy[1][1], cz[1][1]);
        return 1;
      }

  return 0;
}

int run_all_tests() {
  int result = 0;

  result += test_rgb_to_ciexyz_conversion();
  result += test_rgb_to_cielab_conversion();

  return result;
}
