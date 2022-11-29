#include "helpers.h"
#include "tests.h"

#define NAME_IMG_IN "img_32_32.ppm"
#define NAME_IMG_OUT "img_out.ppm"

float computeGradient(LAB lab, int i, int j, int length, int width) {
  float xGradient, yGradient;

  if (i >= length-1 || j >= width-1 || i < 1 || j < 1) return FLT_MAX;

  xGradient = sqrt(SQUARE(lab.l[i+1][j]-lab.l[i-1][j])
                 + SQUARE(lab.a[i+1][j]-lab.a[i-1][j])
                 + SQUARE(lab.b[i+1][j]-lab.a[i-1][j]));

  yGradient = sqrt(SQUARE(lab.l[i][j+1]-lab.l[i][j-1])
                 + SQUARE(lab.a[i][j+1]-lab.a[i][j-1])
                 + SQUARE(lab.b[i][j+1]-lab.a[i][j-1]));

  return xGradient + yGradient;
}

int isWithinImageBouds(int i, int j, int length, int width) {
  return i >= 0 && j >= 0 && i < length && j < width;
}

void adjustClusterCenters(ClusterData **d, LAB lab, LinkedListCenters *centers, int length, int width) {
  // gradient values for adjacent pixels
  float current, top, topRight, right, bottomRight, bottom, bottomLeft, left, topLeft;
  int x, y;
  Center *node = centers->head;

  while (node != NULL) {
    x = node->x;
    y = node->y;

    current = computeGradient(lab, x, y, length, width);
    top = computeGradient(lab, x-1, y, length, width);
    if (current > top) {
      node->x = x-1;
      current = top;
    }

    topRight = computeGradient(lab, x-1, y+1, length, width);
    if (current > topRight) {
      node->x = x-1;
      node->y = y+1;
      current = topRight;
    }

    right = computeGradient(lab, x, y+1, length, width);
    if (current > right) {
      node->y = y+1;
      current = right;
    }

    bottomRight = computeGradient(lab, x+1, y+1, length, width);
    if (current > bottomRight) {
      node->x = x+1;
      node->y = y+1;
      current = bottomRight;
    }

    bottom = computeGradient(lab, x+1, y, length, width);
    if (current > bottom) {
      node->x = x+1;
      current = bottom;
    }

    bottomLeft = computeGradient(lab, x+1, y-1, length, width);
    if (current > bottomLeft) {
      node->x = x+1;
      node->y = y-1;
      current = bottomLeft;
    }

    left = computeGradient(lab, x, y-1, length, width);
    if (current > left) {
      node->y = y-1;
      current = left;
    }

    topLeft = computeGradient(lab, x-1, y-1, length, width);
    if (current > topLeft) {
      node->x = x-1;
      node->y = y-1;
    }

    node = node->next;
  }
}

void initializePixelData(ClusterData *d, int x, int y, int distance) {
  d->x = x;
  d->y = y;
  d->regionLabel = 0;
  d->distanceToLocalCenter = distance;
}

void initializeClusterCenters(ClusterData **d, LAB lab, LinkedListCenters *centers, int nSuperpixels, int length, int width) {
  int gap = (int) sqrt(length * width / (float) nSuperpixels);
  int regionCounter = 0;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      initializePixelData(&d[i][j], i, j, length * width);

      if (i % gap == 2 && j % gap == 2) {
        Center *center = allocate_center();

        d[i][j].regionLabel = regionCounter++;

        center->x = i;
        center->y = j;
        center->region = regionCounter;
        center->l = lab.l[i][j];
        center->a = lab.a[i][j];
        center->b = lab.b[i][j];
        center->next = NULL;

        if (centers->head == NULL) {
          centers->head = center;
          centers->tail = center;
        } else {
          centers->tail->next = center;
          centers->tail = centers->tail->next;
        }
      }
    }
  }

  // print_linkedlist_centers(centers);
  adjustClusterCenters(d, lab, centers, length, width);
}

float computeDistance(float x1, float y1, float x2, float y2) {
  return sqrt(SQUARE(x1 - x2) + SQUARE(y1 - y2));
}

int isPixelAlreadyVisitedByCurrentRegion(Center *center, ClusterData *currentPixel) {
  return center->region == currentPixel->lastRegionVisited;
}

void propagateCenter(ClusterData **d, Center *center, int i, int j, int distanceThreshold, int length, int width) {
  if (!isWithinImageBouds(i, j, length, width)) return;

  if (isPixelAlreadyVisitedByCurrentRegion(center, &d[i][j])) return;
  d[i][j].lastRegionVisited = center->region;

  float distanceToCenter = computeDistance(center->x, center->y, i, j);
  if (distanceToCenter > distanceThreshold) return;

  if (d[i][j].distanceToLocalCenter > distanceToCenter) {
    d[i][j].regionLabel = center->region;
    d[i][j].distanceToLocalCenter = distanceToCenter;
  }

  // recursively propagate to the 4 nearest pixels
  propagateCenter(d, center, i+1, j, distanceThreshold, length, width);
  propagateCenter(d, center, i, j+1, distanceThreshold, length, width);
  propagateCenter(d, center, i, j-1, distanceThreshold, length, width);
  propagateCenter(d, center, i-1, j, distanceThreshold, length, width);
}

void associatePixelsToNearestClusterCenter(ClusterData **d, LinkedListCenters *centers, int nSuperpixels, int length, int width) {
  int distanceThreshold = sqrt(length * width / nSuperpixels);
  int x, y;

  Center *center = centers->head;
  while (center != NULL) {
    x = (int) center->x;
    y = (int) center->y;

    d[x][y].distanceToLocalCenter = 0;
    d[x][y].regionLabel = center->region;
    propagateCenter(d, center, x, y, distanceThreshold, length, width);

    center = center->next;
  }
}

void restorePixelsMetadata(ClusterData **d, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      d[i][j].lastRegionVisited = 0;
      d[i][j].smallestDistance = FLT_MAX;
      d[i][j].isBorder = 0;
    }
  }
}

float computeLabDistance(Center *c, float l, float a, float b) {
  return sqrt(SQUARE(c->l - l) + SQUARE(c->a - a) + SQUARE(c->b - b));
}

float compute5dDistance(Center *center, LAB lab, int i, int j, float S) {
  float dlab, dxy;
  int m = 10;

  dlab = computeLabDistance(center, lab.l[i][j], lab.a[i][j], lab.b[i][j]);
  dxy = computeDistance(center->x, center->y, i, j);
  return dlab + (m / S) * dxy;
}

void propagateRegion(ClusterData **d, LAB lab, Center *center, int i, int j, int distanceThreshold, int length, int width) {
  if (!isWithinImageBouds(i, j, length, width)) return;

  if (isPixelAlreadyVisitedByCurrentRegion(center, &d[i][j])) return;
  d[i][j].lastRegionVisited = center->region;

  float distanceToCenter = computeDistance(center->x, center->y, i, j);
  if (distanceToCenter > distanceThreshold) return;

  float generalizedDistance = compute5dDistance(center, lab, i, j, distanceThreshold);

  if (d[i][j].smallestDistance > generalizedDistance) {
    d[i][j].smallestDistance = generalizedDistance;
    d[i][j].regionLabel = center->region;
  }

  propagateRegion(d, lab, center, i+1, j, distanceThreshold, length, width);
  propagateRegion(d, lab, center, i, j+1, distanceThreshold, length, width);
  propagateRegion(d, lab, center, i, j-1, distanceThreshold, length, width);
  propagateRegion(d, lab, center, i-1, j, distanceThreshold, length, width);
}

void assignPixelsToRegion(ClusterData **d, LAB lab, Center *center, int nSuperpixels, int length, int width) {
  int distanceThreshold = sqrt(length * width / nSuperpixels);
  int x, y;

  x = (int) center->x;
  y = (int) center->y;

  propagateRegion(d, lab, center, x, y, distanceThreshold, length, width);
}

void updateCenterFromAverage(ClusterData **d, LAB lab, Center *center, int length, int width) {
  float nPoints, sumX, sumY, sumL, sumA, sumB;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      if (d[i][j].regionLabel != center->region) continue;

      nPoints += 1;
      sumX += i;
      sumY += j;
      sumL += lab.l[i][j];
      sumA += lab.a[i][j];
      sumB += lab.b[i][j];
    }
  }

  center->x = sumX / nPoints;
  center->y = sumY / nPoints;
  center->l = sumL / nPoints;
  center->a = sumA / nPoints;
  center->b = sumB / nPoints;
}

void recomputeCenters(ClusterData **d, LAB lab, LinkedListCenters *centers, int length, int width) {
  Center *center = centers->head;

  while (center != NULL) {
    updateCenterFromAverage(d, lab, center, length, width);

    center = center->next;
  }
}

int isSameRegion(ClusterData **d, ClusterData *data, int i, int j, int length, int width) {
  if (!isWithinImageBouds(i, j, length, width)) return 1;

  return data->regionLabel == d[i][j].regionLabel;
}

int isPartOfBorder(ClusterData **d, int i, int j, int length, int width) {
  return !(isSameRegion(d, &d[i][j], i-1, j, length, width) &&
         isSameRegion(d, &d[i][j], i+1, j, length, width) &&
         isSameRegion(d, &d[i][j], i, j-1, length, width) &&
         isSameRegion(d, &d[i][j], i, j+1, length, width));
}

void assignBordersToPixels(ClusterData **d, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      if (!isPartOfBorder(d, i, j, length, width)) continue;

      d[i][j].isBorder = 1;
    }
  }
}

void drawBorders(ClusterData **d, LAB lab, int length, int width) {
  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      if (d[i][j].isBorder) {
        // Black borders
        lab.l[i][j] = 100.0;
        lab.a[i][j] = 0.0;
        lab.b[i][j] = 0.0;
      }
    }
  }
}

void segmentImage(ClusterData **d, LAB lab, int length, int width) {
  assignBordersToPixels(d, length, width);
  drawBorders(d, lab, length, width);
}

int main() {
  int length, width;
  RGB rgb;
  LAB lab;
  ClusterData **data;
  int nSuperpixels;
  LinkedListCenters *centers = allocate_linkedlist_centers();

  if (run_all_tests()) return 1;

  get_size_image(NAME_IMG_IN, &length, &width);

  rgb.r = allocate_float_matrix(length, width);
  rgb.g = allocate_float_matrix(length, width);
  rgb.b = allocate_float_matrix(length, width);
  lab.l = allocate_float_matrix(length, width);
  lab.a = allocate_float_matrix(length, width);
  lab.b = allocate_float_matrix(length, width);

  read_ppm_image(rgb.r, rgb.g, rgb.b, NAME_IMG_IN);

  convert_rgb_to_cielab(lab.l, lab.a, lab.b, rgb.r, rgb.g, rgb.b, length, width);

  // printf("Enter number of superpixels: ");
  // scanf("%d", &nSuperpixels);
  nSuperpixels = 8;

  data = allocate_clusterdata_matrix(length, width);
  initializeClusterCenters(data, lab, centers, nSuperpixels, length, width);
  associatePixelsToNearestClusterCenter(data, centers, nSuperpixels, length, width);

  // print_clusterdata_matrix(data, length, width);

  // printf("\n\n");

  Center *center = centers->head;
  // todo: change for residual error against threshold
  for (int i = 0; i < 1; i++) {
    restorePixelsMetadata(data, length, width);

    while (center != NULL) {
      assignPixelsToRegion(data, lab, center, nSuperpixels, length, width);
      center = center->next;
    }

    recomputeCenters(data, lab, centers, length, width);
    // computeResidualError()
  }

  // segmentImage(data, lab, length, width);

  // enforce connectivity

  // lab->rgb conversion
  convert_cielab_to_rgb(rgb.r, rgb.g, rgb.b, lab.l, lab.a, lab.b, length, width);
  save_ppm_image(NAME_IMG_OUT, rgb, length, width);
  // display region outlines
  // save ppm image

  // print_linkedlist_centers(centers);

  // print_clusterdata_matrix(data, length, width);

  free_linkedlist_centers(centers);

  free_float_matrix(rgb.r);
  free_float_matrix(rgb.g);
  free_float_matrix(rgb.b);
  free_float_matrix(lab.l);
  free_float_matrix(lab.a);
  free_float_matrix(lab.b);
  free_clusterdata_matrix(data);

  return 0;
}