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
  int isBorder;
  int belongsToMainRegion; // if pixel in the same group as the center
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
  int pixelCount; // number of pixels in the region (not counting disjoint parts)
  struct Center *next;
} Center;

typedef struct LinkedListCenters {
  struct Center *head;
  struct Center *tail;
} LinkedListCenters;

#define FLT_MAX 3.402823466e+38F
#define SQUARE(X) ((X)*(X))


float **allocate_float_matrix(int length, int width);
void free_float_matrix(float **m);

ClusterData **allocate_clusterdata_matrix(int length, int width);
void free_clusterdata_matrix(ClusterData **m);
void print_clusterdata_matrix(ClusterData **m, int length, int width);

Center *allocate_center();

LinkedListCenters *allocate_linkedlist_centers();
void free_linkedlist_centers(LinkedListCenters *centers);
void print_linkedlist_centers(LinkedListCenters *centers);
void copy_centers(LinkedListCenters *copy, LinkedListCenters *centers);
float compute_residual_error(LinkedListCenters *previousCenters, LinkedListCenters *current);

void copyLAB(LAB *copy, LAB *lab, int length, int width);
