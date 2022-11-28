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

void erasePreviousCenters(ClusterData **d, int i, int j, int length, int width) {
  d[i][j].isCenter = 0;

  if (isWithinImageBouds(i-1, j, length, width)) {
    d[i-1][j].isCenter = 0;
    //todo region label && distance
  }

  if (isWithinImageBouds(i-1, j+1, length, width)) {
    d[i-1][j+1].isCenter = 0;
  }

  if (isWithinImageBouds(i, j+1, length, width)) {
    d[i][j+1].isCenter = 0;
  }

  if (isWithinImageBouds(i+1, j+1, length, width)) {
    d[i+1][j+1].isCenter = 0;
  }

  if (isWithinImageBouds(i+1, j, length, width)) {
    d[i+1][j].isCenter = 0;
  }

  if (isWithinImageBouds(i+1, j-1, length, width)) {
    d[i+1][j-1].isCenter = 0;
  }

  if (isWithinImageBouds(i, j-1, length, width)) {
    d[i][j-1].isCenter = 0;
  }

  if (isWithinImageBouds(i-1, j-1, length, width)) {
    d[i-1][j-1].isCenter = 0;
  }
}

void updateCenter(ClusterData *newCenter, ClusterData *previousCenter) {
  newCenter->isCenter = 1;
  newCenter->regionLabel = previousCenter->regionLabel;
  newCenter->distanceToLocalCenter = 0;
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
      // todo: x++, y--
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

// void adjustClusterCenters(ClusterData **d, LAB lab, int length, int width) {
//   // gradient values for adjacent pixels
//   float current, top, topRight, right, bottomRight, bottom, bottomLeft, left, topLeft;

//   for (int i = 0; i < length; i += 2) {
//     for (int j = 0; j < width; j += 2) {
//       if (!d[i][j].isCenter) continue;

//       // look at 3x3 neighbours
//       current = computeGradient(lab, i, j, length, width);
//       top = computeGradient(lab, i-1, j, length, width);
//       if (current > top) {
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i-1][j], &d[i][j]);
//       }

//       topRight = computeGradient(lab, i-1, j+1, length, width);
//       if (current > topRight) {
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i-1][j+1], &d[i][j]);
//         current = topRight;
//       }

//       right = computeGradient(lab, i, j+1, length, width);
//       if (current > right) {
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i][j+1], &d[i][j]);
//         current = right;
//       }

//       bottomRight = computeGradient(lab, i+1, j+1, length, width);
//       if (current > bottomRight) {
//         printf("bottomright\n");
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i+1][j+1], &d[i][j]);
//         current = bottomRight;
//       }

//       bottom = computeGradient(lab, i+1, j, length, width);
//       if (current > bottom) {
//         printf("bottom\n");
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i+1][j], &d[i][j]);
//         current = bottom;
//       }

//       bottomLeft = computeGradient(lab, i+1, j-1, length, width);
//       if (current > bottomLeft) {
//         printf("bottomleft\n");
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i+1][j-1], &d[i][j]);
//         current = bottomLeft;
//       }

//       left = computeGradient(lab, i, j-1, length, width);
//       if (current > left) {
//         printf("left\n");
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i][j-1], &d[i][j]);
//         current = left;
//       }

//       topLeft = computeGradient(lab, i-1, j-1, length, width);
//       if (current > topLeft) {
//         printf("topleft\n");
//         erasePreviousCenters(d, i, j, length, width);
//         updateCenter(&d[i-1][j-1], &d[i][j]);
//       }
//     }
//   }
// }

void initializeClusterCenters(ClusterData **d, LAB lab, LinkedListCenters *centers, int nSuperpixels, int length, int width) {
  int gap = (int) sqrt(length * width / (float) nSuperpixels);
  int regionCounter = 0;

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      d[i][j].x = i;
      d[i][j].y = j;
      d[i][j].isCenter = 0;
      d[i][j].regionLabel = 0;
      d[i][j].distanceToLocalCenter = length * width;

      if (i % gap == 2 && j % gap == 2) {
        Center *center = (Center *)malloc(sizeof(Center));
        if (center == NULL) printf("Memory error\n");

        d[i][j].isCenter = 1;
        d[i][j].regionLabel = regionCounter++;

        center->x = i;
        center->y = j;
        center->region = regionCounter;
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

  print_linkedlist_centers(centers);
  adjustClusterCenters(d, lab, centers, length, width);
}

float computeDistance(int x1, int y1, int x2, int y2) {
  return sqrt(SQUARE(x1 - x2) + SQUARE(y1 - y2));
}

int isPixelAlreadyVisitedByCurrentRegion(ClusterData *currentCenter, ClusterData *currentPixel) {
  return currentCenter->regionLabel == currentPixel->lastRegionVisited;
}

void propagateCenter(ClusterData **d, ClusterData *currentCenter, int i, int j, int distanceThreshold, int length, int width) {
  if (!isWithinImageBouds(i, j, length, width)) return;

  if (isPixelAlreadyVisitedByCurrentRegion(currentCenter, &d[i][j])) return;
  d[i][j].lastRegionVisited = currentCenter->regionLabel;

  float distanceToCenter = computeDistance(currentCenter->x, currentCenter->y, i, j);
  if (distanceToCenter > distanceThreshold) return;

  if (d[i][j].distanceToLocalCenter > distanceToCenter && !d[i][j].isCenter) {
    d[i][j].regionLabel = currentCenter->regionLabel;
    d[i][j].distanceToLocalCenter = distanceToCenter;
  }

  // recursively propagate to the 4 nearest pixels
  propagateCenter(d, currentCenter, i+1, j, distanceThreshold, length, width);
  propagateCenter(d, currentCenter, i, j+1, distanceThreshold, length, width);
  propagateCenter(d, currentCenter, i, j-1, distanceThreshold, length, width);
  propagateCenter(d, currentCenter, i-1, j, distanceThreshold, length, width);
}

void associatePixelsToNearestClusterCenter(ClusterData **d, int nSuperpixels, int length, int width) {
  int distanceThreshold = 2 * sqrt(length * width / nSuperpixels);

  for (int i = 0; i < length; i++) {
    for (int j = 0; j < width; j++) {
      if (!d[i][j].isCenter) continue;

      d[i][j].distanceToLocalCenter = 0;
      propagateCenter(d, &d[i][j], i, j, distanceThreshold, length, width);
    }
  }
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
  associatePixelsToNearestClusterCenter(data, nSuperpixels, length, width);

  print_linkedlist_centers(centers);

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