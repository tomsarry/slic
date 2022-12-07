#include "ioHelpers.h"

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