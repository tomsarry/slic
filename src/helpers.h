#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

float **allocate_float_matrix(int length, int width);
void free_float_matrix(float **m);

void convert_rgb_to_cielab(float **cl, float **ca, float **cb, float **r, float **g, float **b, int length, int width);
void convert_rgb_to_ciexyz(float **cx, float **cy, float **cz, float **r, float **g, float **b, int length, int width);
void convert_cielab_to_ciexyz(float **cx, float **cy, float **cz, float **cl, float **ca, float **cb, int length, int width);
void convert_cielab_to_rgb(float **r, float **g, float **b, float **cl, float **ca, float **cb, int length, int width);