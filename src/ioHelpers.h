#pragma once

#include "helpers.h"

void get_size_image(const char *filename, int *length, int *width);
void read_ppm_image(float **r, float **g, float **b, const char *filename);
void save_ppm_image(const char *filaname, RGB rgb, int length, int width);
