#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

typedef struct ClusterData {
  int regionLabel;
  float distanceToLocalCenter;
  float smallestDistance;
  int lastRegionVisited;
  int x;
  int y;
} ClusterData;

typedef struct RGB {
  float **r;
  float **g;
  float **b;
} RGB;

typedef struct LAB {
  float **l;
  float **a;
  float **b;
} LAB;

typedef struct Center {
  float x;
  float y;
  float l;
  float a;
  float b;
  int region;
  struct Center *next;
} Center;

typedef struct LinkedListCenters {
  struct Center *head;
  struct Center *tail;
} LinkedListCenters;

#define FLT_MAX 3.402823466e+38F
#define SQUARE(X) pow(X, 2)


float **allocate_float_matrix(int length, int width);
void free_float_matrix(float **m);

ClusterData **allocate_clusterdata_matrix(int length, int width);
void free_clusterdata_matrix(ClusterData **m);
void print_clusterdata_matrix(ClusterData **m, int length, int width);

Center *allocate_center();

LinkedListCenters *allocate_linkedlist_centers();
void free_linkedlist_centers(LinkedListCenters *centers);
void print_linkedlist_centers(LinkedListCenters *centers);

// TODO: replace 3 matrices with RGB / LAB structs
void convert_rgb_to_cielab(float **cl, float **ca, float **cb, float **r, float **g, float **b, int length, int width);
void convert_rgb_to_ciexyz(float **cx, float **cy, float **cz, float **r, float **g, float **b, int length, int width);
void convert_cielab_to_ciexyz(float **cx, float **cy, float **cz, float **cl, float **ca, float **cb, int length, int width);
void convert_cielab_to_rgb(float **r, float **g, float **b, float **cl, float **ca, float **cb, int length, int width);

void get_size_image(const char *filename, int *length, int *width);
void read_ppm_image(float **r, float **g, float **b, const char *filename);