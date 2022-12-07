#include "colorHelpers.h"

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

void convert_ciexyz_to_cielab(float **cl, float **ca, float **cb, float **cx, float **cy, float **cz, int length, int width) {
  float xn = 95.0489;
  float yn = 100;
  float zn = 108.8840;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      cl[i][j] = 116 * f_lab(cy[i][j] / yn) - 16;
      ca[i][j] = 500 * (f_lab(cx[i][j] / xn) - f_lab(cy[i][j] / yn));
      cb[i][j] = 200 * (f_lab(cy[i][j] / yn) - f_lab(cz[i][j] / zn));
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

float f_lab_inverse(float t) {
  float delta = 6/29.;

  if (t > delta) return pow(t, 3);
  return 3 * pow(delta, 2) * (t - 4/29.);
}

void convert_cielab_to_ciexyz(float **cx, float **cy, float **cz, float **cl, float **ca, float **cb, int length, int width) {
  float xn = 95.0489;
  float yn = 100;
  float zn = 108.8840;
  float tmp;

  float varx, vary, varz;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      tmp = (cl[i][j] + 16) / 116.;

      cx[i][j] = xn * f_lab_inverse(tmp + ca[i][j] / 500.);
      cy[i][j] = yn * f_lab_inverse(tmp);
      cz[i][j] = zn * f_lab_inverse(tmp - cb[i][j] / 200.);
    }
  }
}

float compute_rgb_value(float c) {
  if (c <= 0.0031308) return 12.92 * c;
  return 1.055 * pow(c, 1/2.4) - 0.055;
}

void convert_ciexyz_to_rgb(float **r, float **g, float **b, float **cx, float **cy, float **cz, int length, int width) {
  float r_linear, g_linear, b_linear;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      r_linear = 3.2406 * cx[i][j] - 1.5372 * cy[i][j] - 0.4986 * cz[i][j];
      g_linear = -0.9689 * cx[i][j] + 1.8758 * cy[i][j] + 0.0415 * cz[i][j];
      b_linear = 0.0557 * cx[i][j] - 0.2040 * cy[i][j] + 1.0570 * cz[i][j];

      r[i][j] = round(compute_rgb_value(r_linear / 100.) * 255);
      g[i][j] = round(compute_rgb_value(g_linear / 100.) * 255);
      b[i][j] = round(compute_rgb_value(b_linear / 100.) * 255);
    }
  }
}

void convert_cielab_to_rgb(float **r, float **g, float **b, float **cl, float **ca, float **cb, int length, int width) {
  float **tmp1 = allocate_float_matrix(length, width);
  float **tmp2 = allocate_float_matrix(length, width);
  float **tmp3 = allocate_float_matrix(length, width);

  convert_cielab_to_ciexyz(tmp1, tmp2, tmp3, cl, ca, cb, length, width);
  convert_ciexyz_to_rgb(r, g, b, tmp1, tmp2, tmp3, length, width);

  free_float_matrix(tmp1);
  free_float_matrix(tmp2);
  free_float_matrix(tmp3);
}