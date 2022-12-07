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

void copy_cluster_data(ClusterData **copy, ClusterData **d, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      copy[i][j].regionLabel = d[i][j].regionLabel;
      copy[i][j].distanceToLocalCenter = d[i][j].distanceToLocalCenter;
      copy[i][j].smallestDistance = d[i][j].smallestDistance;
      copy[i][j].lastRegionVisited = d[i][j].lastRegionVisited;
      copy[i][j].isBorder = d[i][j].isBorder;
      copy[i][j].belongsToMainRegion = d[i][j].belongsToMainRegion;
      copy[i][j].x = d[i][j].x;
      copy[i][j].y = d[i][j].y;
    }
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

void copy_center(Center *copy, Center *center) {
  copy->x = center->x;
  copy->y = center->y;
  copy->l = center->l;
  copy->a = center->a;
  copy->b = center->b;
  copy->region = center->region;
  copy->pixelCount = center->pixelCount;
  copy->next = NULL;
}

void copy_centers(LinkedListCenters *copy, LinkedListCenters *centers) {
  Center *center = centers->head;
  Center *newCenter;
  Center *previousCenter = NULL;

  while (center != NULL) {
    newCenter = allocate_center();
    copy_center(newCenter, center);

    if (previousCenter == NULL) {
      copy->head = newCenter;
    } else {
      previousCenter->next = newCenter;
    }

    previousCenter = newCenter;

    center = center->next;
  }
}

float compute_residual_error(LinkedListCenters *previousCenters, LinkedListCenters *centers) {
  float sum = 0;

  Center *previousCurrent = previousCenters->head;
  Center *current = centers->head;

  while (previousCurrent != NULL && current != NULL) {
    // compute L1 distance between the two centers
    sum += fabs(previousCurrent->x - current->x);
    sum += fabs(previousCurrent->y - current->y);

    previousCurrent = previousCurrent->next;
    current = current->next;
  }

  if (current != NULL || previousCurrent != NULL) {
    printf("[compute_residual_error]:: Warning: LinkedLists have different size.\n");
  }

  return sum;
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

