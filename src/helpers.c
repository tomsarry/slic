#include "helpers.h"

float **allocate_float_matrix(int length, int width) {
  float **matrix;
  float *internal_ptr;

  matrix = (float **) malloc(sizeof(float *) * length);
  if (matrix == NULL) printf("[allocate_float_matrix]:: Memory error.");

  internal_ptr = (float *) malloc(sizeof(float) * length * width);
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

ClusterData **allocate_clusterdata_matrix(int length, int width) {
  struct ClusterData **matrix;
  struct ClusterData *internal_ptr;

  matrix = (struct ClusterData **) malloc(sizeof(struct ClusterData *) * length);
  if (matrix == NULL) printf("[allocate_clusterdata_matrix]:: Memory error.");

  internal_ptr = (struct ClusterData *) malloc(sizeof(struct ClusterData) * length * width);
  if (internal_ptr == NULL) printf("[allocate_clusterdata_matrix]:: Memory error.");

  for (int i = 0; i < length; i++, internal_ptr += width) {
    matrix[i] = internal_ptr;
  }

  return matrix;
}

void free_clusterdata_matrix(ClusterData **m) {
  free(m[0]);
  free(m);
}

void print_clusterdata_matrix(ClusterData **m, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      printf("([lab%d]:d%4.1f)", m[i][j].regionLabel, m[i][j].smallestDistance);
    }
    printf("\n");
  }
}

Center *allocate_center() {
  Center *center = (Center *)malloc(sizeof(Center));

  if (center == NULL) printf("Memory error\n");

  return center;
}

LinkedListCenters *allocate_linkedlist_centers() {
  LinkedListCenters *centers = (LinkedListCenters *)malloc(sizeof(LinkedListCenters));

  if (centers == NULL) printf("[allocate_linkedlist_centers]:: Memory error.");

  return centers;
}

void free_linkedlist_centers(LinkedListCenters *centers) {
  Center *ptr = centers->head;
  Center *tmp;
  while (ptr != NULL) {
    tmp = ptr->next;
    free(ptr);
    ptr = tmp;
  }

  free(centers);
}

void print_linkedlist_centers(LinkedListCenters *centers) {
  Center *ptr = centers->head;
  while (ptr != NULL) {
    printf("(<%.1f;%.1f> (%d))->", ptr->x, ptr->y, ptr->region);
    ptr = ptr->next;
  }
  printf("NULL\n");
}

void copyLAB(LAB *copy, LAB *lab, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      copy->l[i][j] = lab->l[i][j];
      copy->a[i][j] = lab->a[i][j];
      copy->b[i][j] = lab->b[i][j];
    }
  }
}

float compute_linear_value(float c) {
  if (c <= 0.04045) {
    return c / 12.92;
  }

  return pow((c + 0.055) / 1.055, 2.4);
}

void get_size_image(const char *filename, int *length, int *width) {
  FILE *fp;
  int len, wid, rgb_comp_color;
  char c;
  char buff[16];

  fp = fopen(filename, "rb");
  if (!fp) {
      fprintf(stderr, "Unable to open file '%s'\n", filename);
      exit(1);
  }

  // read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }

  // validate image format
  if (buff[0] != 'P' || buff[1] != '6') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(1);
  }

  // ignore comments
  c = getc(fp);
  while (c == '#') {
    while (getc(fp) != '\n') printf("%c\n", c);
    c = getc(fp);
  }

  ungetc(c, fp);

  // read image size information
  if (fscanf(fp, "%d %d", &wid, &len) != 2) {
    fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
    exit(1);
  }

  // assign image size information
  *length = len;
  *width = wid;
}

void read_ppm_image(float **r, float **g, float **b, const char *filename) {
  FILE *fp;
  int len, wid, rgb_comp_color;
  char c;
  char buff[16];

  fp = fopen(filename, "rb");
  if (!fp) {
      fprintf(stderr, "Unable to open file '%s'\n", filename);
      exit(1);
  }

  // read image format
  if (!fgets(buff, sizeof(buff), fp)) {
    perror(filename);
    exit(1);
  }

  // validate image format
  if (buff[0] != 'P' || buff[1] != '6') {
    fprintf(stderr, "Invalid image format (must be 'P6')\n");
    exit(1);
  }

  // ignore comments
  c = getc(fp);
  while (c == '#') {
    while (getc(fp) != '\n') printf("%c\n", c);
    c = getc(fp);
  }

  ungetc(c, fp);

  // read image size information
  if (fscanf(fp, "%d %d", &wid, &len) != 2) {
    fprintf(stderr, "Invalid image size (error loading '%s')\n", filename);
    exit(1);
  }

  //read rgb component
  if (fscanf(fp, "%d", &rgb_comp_color) != 1) {
      fprintf(stderr, "Invalid rgb component (error loading '%s')\n", filename);
      exit(1);
  }

  //check rgb component depth
  if (rgb_comp_color != 255) {
      fprintf(stderr, "'%s' does not have 8-bits components\n", filename);
      exit(1);
  }

  while (fgetc(fp) != '\n');

  for (int i = 0; i < len; i++) {
    for (int j = 0; j < wid; j++) {
      u_int8_t rgb_buff[3];

      fread(rgb_buff, 1, 3, fp);
      r[i][j] = rgb_buff[0];
      g[i][j] = rgb_buff[1];
      b[i][j] = rgb_buff[2];
    }
  }

  fclose(fp);
}

void save_ppm_image(const char *filename, RGB rgb, int length, int width) {
  FILE *fp = fopen(filename, "wb");
  (void) fprintf(fp, "P6\n%d %d\n255\n", width, length);

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      static unsigned char color[3];
      color[0] = (int) rgb.r[i][j];
      color[1] = (int) rgb.g[i][j];
      color[2] = (int) rgb.b[i][j];

      (void) fwrite(color, 1, 3, fp);
    }
  }

  fclose(fp);
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
