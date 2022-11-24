#include "helpers.h"

#include <math.h>

float **allocate_float_matrix(int length, int width) {
  float **matrix;
  float *internal_ptr;

  matrix = (float **) malloc(sizeof(float *) * length);
  if (matrix == NULL) printf("[allocate_float_matrix]:: Memory error.");

  internal_ptr = (float*) malloc(sizeof(float) * length * width);
  if (internal_ptr == NULL) printf("[allocate_float_matrix]:: Memory error.");

  for (int i = 0; i < length; i++, internal_ptr += width) {
    matrix[i] = internal_ptr;
  }

  return matrix;
}

void free_float_matrix(float **m) {
  free(m[0]);
  free(m);
}

float compute_linear_value(float c) {
  if (c <= 0.04045) {
    return c / 12.92;
  }

  return pow((c + 0.055) / 1.055, 2.4);
}

void convert_rgb_to_ciexyz(float **cx, float **cy, float **cz, float **r, float **g, float **b, int length, int width) {
  float r_linear, g_linear, b_linear;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      r_linear = compute_linear_value(r[i][j] / 255.0);
      g_linear = compute_linear_value(g[i][j] / 255.0);
      b_linear = compute_linear_value(b[i][j] / 255.0);

      r_linear *= 100;
      g_linear *= 100;
      b_linear *= 100;

      cx[i][j] = r_linear * 0.4124 + g_linear * 0.3576 + b_linear * 0.1805;
      cy[i][j] = r_linear * 0.2126 + g_linear * 0.7152 + b_linear * 0.0722;
      cz[i][j] = r_linear * 0.0193 + g_linear * 0.1192 + b_linear * 0.9505;
    }
  }
}

float f_lab(float t) {
  float delta = 6/29.;

  if (t > pow(delta, 3)) return pow(t, 1/3.);
  return t / (3 * pow(delta, 2)) + 4/29.;
}

void convert_ciexyz_to_cielab(float **cl, float **ca, float **cb, float **x, float **y, float **z, int length, int width) {
  float xn = 95.0489;
  float yn = 100;
  float zn = 108.8840;
  float delta = 6 / 29.0;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      cl[i][j] = 116 * f_lab(y[i][j] / yn) - 16;
      ca[i][j] = 500 * (f_lab(x[i][j] / xn) - f_lab(y[i][j] / yn));
      cb[i][j] = 200 * (f_lab(y[i][j] / yn) - f_lab(z[i][j] / zn));
    }
  }
}

void convert_rgb_to_cielab(float **cl, float **ca, float **cb, float **r, float **g, float **b, int length, int width) {
  float **tmp1 = allocate_float_matrix(length, width);
  float **tmp2 = allocate_float_matrix(length, width);
  float **tmp3 = allocate_float_matrix(length, width);

  convert_rgb_to_ciexyz(tmp1, tmp2, tmp3, r, g, b, length, width);
  convert_ciexyz_to_cielab(cl, ca, cb, tmp1, tmp2, tmp3, length, width);

  free_float_matrix(tmp1);
  free_float_matrix(tmp2);
  free_float_matrix(tmp3);
}
