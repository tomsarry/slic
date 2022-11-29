#include "helpers.h"

#define EPS 0.1

void initialize_test_matrix(float **r, float **g, float **b) {
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

  r[0][2] = 49;
  g[0][2] = 36;
  b[0][2] = 19;

  r[1][2] = 0;
  g[1][2] = 0;
  b[1][2] = 0;
}

int test_rgb_to_cielab_conversion() {
  float **r = allocate_float_matrix(2, 3);
  float **g = allocate_float_matrix(2, 3);
  float **b = allocate_float_matrix(2, 3);

  float **cl = allocate_float_matrix(2, 3);
  float **ca = allocate_float_matrix(2, 3);
  float **cb = allocate_float_matrix(2, 3);

  int result = 0;

  initialize_test_matrix(r, g, b);

  convert_rgb_to_cielab(cl, ca, cb, r, g, b, 2, 3);

  if (abs(cl[0][0] - 48.0) > EPS ||
      abs(ca[0][0] - 36.8) > EPS ||
      abs(cb[0][0] + 47.0) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", cl[0][0], ca[0][0], cb[0][0]);
        result += 1;
      }

  if (abs(cl[0][1] - 57.4) > EPS ||
      abs(ca[0][1] - 57.2) > EPS ||
      abs(cb[0][1] + 21.8) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", cl[0][1], ca[0][1], cb[0][1]);
        result += 1;
      }

  if (abs(cl[1][0] - 95.4) > EPS ||
      abs(ca[1][0] + 16.7) > EPS ||
      abs(cb[1][0] - 63.7) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", cl[1][0], ca[1][0], cb[1][0]);
        result += 1;
      }

  if (abs(cl[1][1] - 100.0) > EPS ||
      abs(ca[1][1] - 0.01) > EPS ||
      abs(cb[1][1] + 0.01) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cl[1][1], ca[1][1], cb[1][1]);
        result += 1;
      }

  if (abs(cl[0][2] - 15.3) > EPS ||
      abs(ca[0][2] - 3.5) > EPS ||
      abs(cb[0][2] - 13.7) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting fifth pixel <%.4f:%.4f:%.4f>.\n", cl[0][2], ca[0][2], cb[0][2]);
        result += 1;
      }

  if (abs(cl[1][2] - 0.0) > EPS ||
      abs(ca[1][2] - 0.0) > EPS ||
      abs(cb[1][2] - 0.0) > EPS) {
        printf("[test_rgb_to_cielab_conversion]:: Error converting sixth pixel <%.4f:%.4f:%.4f>.\n", cl[1][2], ca[1][2], cb[1][2]);
        result += 1;
      }

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);
  free_float_matrix(cl);
  free_float_matrix(ca);
  free_float_matrix(cb);

  return result;
}

int test_rgb_to_ciexyz_conversion() {
  float **r = allocate_float_matrix(2, 3);
  float **g = allocate_float_matrix(2, 3);
  float **b = allocate_float_matrix(2, 3);

  float **cx = allocate_float_matrix(2, 3);
  float **cy = allocate_float_matrix(2, 3);
  float **cz = allocate_float_matrix(2, 3);

  int result = 0;

  initialize_test_matrix(r, g, b);

  convert_rgb_to_ciexyz(cx, cy, cz, r, g, b, 2, 3);

  if (abs(cx[0][0] - 23.2) > EPS ||
      abs(cy[0][0] - 16.8) > EPS ||
      abs(cz[0][0] - 53.1) > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", cx[0][0], cy[0][0], cz[0][0]);
        result += 1;
      }

  if (abs(cx[0][1] - 39.6) > EPS ||
      abs(cy[0][1] - 25.3) > EPS ||
      abs(cz[0][1] - 44.4) > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", cx[0][1], cy[0][1], cz[0][1]);
        result += 1;
      }

  if (abs(cx[1][0] - 75.6) > EPS ||
      abs(cy[1][0] - 88.5) > EPS ||
      abs(cz[1][0] - 28.7) > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", cx[1][0], cy[1][0], cz[1][0]);
        result += 1;
      }

  if (abs(cx[1][1] - 95.0) > EPS ||
      abs(cy[1][1] - 100.) > EPS ||
      abs(cz[1][1] - 108.9) > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cx[1][1], cy[1][1], cz[1][1]);
        result += 1;
      }

  if (abs(cx[0][2] - 2.02) > EPS ||
      abs(cy[0][2] - 1.96) > EPS ||
      abs(cz[0][2] - 0.89) > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cx[1][1], cy[1][1], cz[1][1]);
        result += 1;
      }

  if (cx[1][2] - 0 > EPS ||
      cy[1][2] - 0 > EPS ||
      cz[1][2] - 0 > EPS) {
        printf("[test_rgb_to_ciexyz_conversion]:: Error converting sixth pixel <%.4f:%.4f:%.4f>.\n", cx[1][2], cy[1][2], cz[1][2]);
        result += 1;
      }

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);
  free_float_matrix(cx);
  free_float_matrix(cy);
  free_float_matrix(cz);

  return result;
}

int test_ciexyz_to_rgb_conversion() {
  float **r = allocate_float_matrix(2, 3);
  float **g = allocate_float_matrix(2, 3);
  float **b = allocate_float_matrix(2, 3);

  float **cx = allocate_float_matrix(2, 3);
  float **cy = allocate_float_matrix(2, 3);
  float **cz = allocate_float_matrix(2, 3);

  int result = 0;

  initialize_test_matrix(r, g, b);

  convert_rgb_to_ciexyz(cx, cy, cz, r, g, b, 2, 3);
  convert_ciexyz_to_rgb(r, g, b, cx, cy, cz, 2, 3);

  if (abs(r[0][0] - 132)  > EPS ||
      abs(g[0][0] - 94) > EPS ||
      abs(b[0][0] - 194) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", r[0][0], g[0][0], b[0][0]);
        result += 1;
      }

  if (abs(r[0][1] - 214)  > EPS ||
      abs(g[0][1] - 93) > EPS ||
      abs(b[0][1] - 177) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", r[0][1], g[0][1], b[0][1]);
        result += 1;
      }

  if (abs(r[1][0] - 249)  > EPS ||
      abs(g[1][0] - 248) > EPS ||
      abs(b[1][0] - 113) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", r[1][0], g[1][0], b[1][0]);
        result += 1;
      }

  if (abs(r[1][1] - 255)  > EPS ||
      abs(g[1][1] - 255) > EPS ||
      abs(b[1][1] - 255) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", r[1][1], g[1][1], b[1][1]);
        result += 1;
      }

  if (abs(r[0][2] - 49)  > EPS ||
      abs(g[0][2] - 36) > EPS ||
      abs(b[0][2] - 19) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting fifth pixel <%.4f:%.4f:%.4f>.\n", r[0][2], g[0][2], b[0][2]);
        result += 1;
      }

  if (abs(r[1][2] - 0)  > EPS ||
      abs(g[1][2] - 0) > EPS ||
      abs(b[1][2] - 0) > EPS) {
        printf("[test_ciexyz_to_rgb_conversion]:: Error converting sixth pixel <%.4f:%.4f:%.4f>.\n", r[1][2], g[1][2], b[1][2]);
        result += 1;
      }

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);
  free_float_matrix(cx);
  free_float_matrix(cy);
  free_float_matrix(cz);

  return result;
}

int test_cielab_to_ciexyz_conversion() {
  float **r = allocate_float_matrix(2, 3);
  float **g = allocate_float_matrix(2, 3);
  float **b = allocate_float_matrix(2, 3);

  float **cl = allocate_float_matrix(2, 3);
  float **ca = allocate_float_matrix(2, 3);
  float **cb = allocate_float_matrix(2, 3);

  float **cx = allocate_float_matrix(2, 3);
  float **cy = allocate_float_matrix(2, 3);
  float **cz = allocate_float_matrix(2, 3);

  int result = 0;

  initialize_test_matrix(r, g, b);

  convert_rgb_to_cielab(cl, ca, cb, r, g, b, 2, 3);
  convert_cielab_to_ciexyz(cx, cy, cz, cl, ca, cb, 2, 3);

  if ((abs(cx[0][0] - 23.2) > EPS ||
      abs(cy[0][0] - 16.8) > EPS ||
      abs(cz[0][0] - 53.1) > EPS)) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", cx[0][0], cy[0][0], cz[0][0]);
        result += 1;
      }

  if (abs(cx[0][1] - 39.6) > EPS ||
      abs(cy[0][1] - 25.3) > EPS ||
      abs(cz[0][1] - 44.4) > EPS) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", cx[0][1], cy[0][1], cz[0][1]);
        result += 1;
      }

  if (abs(cx[1][0] - 75.6) > EPS ||
      abs(cy[1][0] - 88.5) > EPS ||
      abs(cz[1][0] - 28.7) > EPS) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", cx[1][0], cy[1][0], cz[1][0]);
        result += 1;
      }

  if (abs(cx[1][1] - 95.1) > EPS ||
      abs(cy[1][1] - 100.) > EPS ||
      abs(cz[1][1] - 108.9) > EPS) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", cx[1][1], cy[1][1], cz[1][1]);
        result += 1;
      }

  if (abs(cx[0][2] - 2.02) > EPS ||
      abs(cy[0][2] - 1.96) > EPS ||
      abs(cz[0][2] - 0.89) > EPS) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting fifth pixel <%.4f:%.4f:%.4f>.\n", cx[0][2], cy[0][2], cz[0][2]);
        result += 1;
      }

  if (abs(cx[1][2] - 0) > EPS ||
      abs(cy[1][2] - 0) > EPS ||
      abs(cz[1][2] - 0) > EPS) {
        printf("[test_cielab_to_ciexyz_conversion]:: Error converting sixth pixel <%.4f:%.4f:%.4f>.\n", cx[1][2], cy[1][2], cz[1][2]);
        result += 1;
      }

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);
  free_float_matrix(cl);
  free_float_matrix(ca);
  free_float_matrix(cb);
  free_float_matrix(cx);
  free_float_matrix(cy);
  free_float_matrix(cz);

  return result;
}

int test_cielab_to_rgb_conversion() {
  float **r = allocate_float_matrix(2, 3);
  float **g = allocate_float_matrix(2, 3);
  float **b = allocate_float_matrix(2, 3);

  float **cl = allocate_float_matrix(2, 3);
  float **ca = allocate_float_matrix(2, 3);
  float **cb = allocate_float_matrix(2, 3);

  int result = 0;

  initialize_test_matrix(r, g, b);

  convert_rgb_to_cielab(cl, ca, cb, r, g, b, 2, 3);
  convert_cielab_to_rgb(r, g, b, cl, ca, cb, 2, 3);

  if (abs(r[0][0] - 132)  > EPS ||
      abs(g[0][0] - 94) > EPS ||
      abs(b[0][0] - 194) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting first pixel <%.4f:%.4f:%.4f>.\n", r[0][0], g[0][0], b[0][0]);
        result += 1;
      }

  if (abs(r[0][1] - 214)  > EPS ||
      abs(g[0][1] - 93) > EPS ||
      abs(b[0][1] - 177) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting second pixel <%.4f:%.4f:%.4f>.\n", r[0][1], g[0][1], b[0][1]);
        result += 1;
      }

  if (abs(r[1][0] - 249)  > EPS ||
      abs(g[1][0] - 248) > EPS ||
      abs(b[1][0] - 113) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting third pixel <%.4f:%.4f:%.4f>.\n", r[1][0], g[1][0], b[1][0]);
        result += 1;
      }

  if (abs(r[1][1] - 255)  > EPS ||
      abs(g[1][1] - 255) > EPS ||
      abs(b[1][1] - 255) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting fourth pixel <%.4f:%.4f:%.4f>.\n", r[1][1], g[1][1], b[1][1]);
        result += 1;
      }

  if (abs(r[0][2] - 49)  > EPS ||
      abs(g[0][2] - 36) > EPS ||
      abs(b[0][2] - 19) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting fifth pixel <%.4f:%.4f:%.4f>.\n", r[0][2], g[0][2], b[0][2]);
        result += 1;
      }

  if (abs(r[1][2] - 0)  > EPS ||
      abs(g[1][2] - 0) > EPS ||
      abs(b[1][2] - 0) > EPS) {
        printf("[test_cielab_to_rgb_conversion]:: Error converting sixth pixel <%.4f:%.4f:%.4f>.\n", r[1][2], g[1][2], b[1][2]);
        result += 1;
      }

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);
  free_float_matrix(cl);
  free_float_matrix(ca);
  free_float_matrix(cb);

  return result;
}

int run_all_tests() {
  int result = 0;

  result += test_rgb_to_ciexyz_conversion();
  result += test_rgb_to_cielab_conversion();
  result += test_cielab_to_ciexyz_conversion();
  result += test_ciexyz_to_rgb_conversion();
  result += test_cielab_to_rgb_conversion();

  return result;
}
