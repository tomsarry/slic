#pragma once

#include "helpers.h"

// TODO: replace 3 matrices with RGB / LAB structs
void convert_rgb_to_cielab(float **cl, float **ca, float **cb, float **r, float **g, float **b, int length, int width);
void convert_rgb_to_ciexyz(float **cx, float **cy, float **cz, float **r, float **g, float **b, int length, int width);
void convert_ciexyz_to_rgb(float **r, float **g, float **b, float **cx, float **cy, float **cz, int length, int width);
void convert_cielab_to_ciexyz(float **cx, float **cy, float **cz, float **cl, float **ca, float **cb, int length, int width);
void convert_cielab_to_rgb(float **r, float **g, float **b, float **cl, float **ca, float **cb, int length, int width);