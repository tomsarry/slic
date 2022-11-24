#pragma once

#include <stdio.h>
// #include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

float **allocate_float_matrix(int length, int width);
void free_float_matrix(float **m);

void convert_rgb_to_cielab(float **cl, float **ca, float **cb, float **r, float **g, float **b, int length, int width);
void convert_rgb_to_ciexyz(float **cx, float **cy, float **cz, float **r, float **g, float **b, int length, int width);