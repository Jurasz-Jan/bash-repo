#include "MathHelper.h"

#include <iostream>

using namespace std;

double MathHelper::distanceSQ(double *p1, double *p2, int dimensions) {
  double d = 0;
  double delta;

  for (int dim = 0; dim < dimensions; dim++) {
    delta = p1[dim] - p2[dim];
    d += delta * delta;
  }

  return d;
}

double MathHelper::distance(double *p1, double *p2, int dimensions) {
  return sqrt(distanceSQ(p1, p2, dimensions));
}

void MathHelper::move(double *p1, double *p2, double *p3, int dimensions,
                      double ratio) {
  double d = distance(p1, p2, dimensions);

  double r;
  if (d) {
    r = ratio / d;
  } else {
    r = 0;
  }
  for (int dim = 0; dim < dimensions; dim++) {
    p3[dim] = p1[dim] + (p2[dim] - p1[dim]) * r;
  }
}