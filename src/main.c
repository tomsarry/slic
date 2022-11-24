#include "helpers.h"
#include "tests.h"

#define NAME_IMG_IN "img.ppm"
#define NAME_IMG_OUT "img_out.ppm"

int main() {
  int length, width;
  float **r;
  float **g;
  float **b;
  int n_superpixels;

  if (run_all_tests()) return 1;

  get_size_image(NAME_IMG_IN, &length, &width);

  r = allocate_float_matrix(length, width);
  g = allocate_float_matrix(length, width);
  b = allocate_float_matrix(length, width);

  read_ppm_image(r, g, b, NAME_IMG_IN, &length, &width);

  printf("Enter number of superpixels: ");
  scanf("%d", &n_superpixels);

  free_float_matrix(r);
  free_float_matrix(g);
  free_float_matrix(b);

  return 0;
}