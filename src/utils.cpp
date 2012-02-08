/*
 * utils.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "utils.hpp"

bool intersect_circle(const Ray & ray, Point3D centre, double radius, Vector3D normal, Intersection& intersection, double & t) {
  double denom = ray.D.dot(normal);
  double num = (centre - ray.O).dot(normal);

  if (denom != 0 && num != 0) {
    t = num / denom;
    if (t > EPSILON) {
      // find intersection
      intersection.NEAR = ray.O + t * ray.D;
      if ((intersection.NEAR[0] * intersection.NEAR[0]) + (intersection.NEAR[2] * intersection.NEAR[2]) <= (radius * radius)) {
        intersection.NORMAL = normal;
        return true;
      }

    }
  }

  return false;
}
