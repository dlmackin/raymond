/*
 * cylinder.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "cylinder.hpp"
#include "global.hpp"
#include "utils.hpp"

#define Y_MIN 0
#define Y_MAX 1

int ctuv = 0;

// TODO?
const Point3D Cylinder::CENTRE = Point3D(0, 0, 0);

const Point3D Cylinder::TOP_POINT = Point3D(0, Y_MAX, 0);
const Vector3D Cylinder::TOP_NORMAL = Vector3D(0, 1, 0);
const Point3D Cylinder::BOTTOM_POINT = Point3D(0, Y_MIN, 0);
const Vector3D Cylinder::BOTTOM_NORMAL = Vector3D(0, -1, 0);

Cylinder::Cylinder() {
  // TODO Auto-generated constructor stub

}

Cylinder::~Cylinder() {
  // TODO Auto-generated destructor stub
}

bool Cylinder::intersect(const Ray & ray, Intersection & intersection, bool quick) {
  Intersection near, far;
  double t_near = DOUBLE_MAX, t_far = 0;
  double t[2];

  double x_D = ray.D[0];
  double y_D = ray.D[1];
  double z_D = ray.D[2];
  double x_O = ray.O[0];
  double y_O = ray.O[1];
  double z_O = ray.O[2];

  // cylinder part
  double A = (x_D * x_D) + (z_D * z_D);
  double B = (2 * x_O * x_D) + (2 * z_O * z_D);
  double C = (x_O * x_O) + (z_O * z_O) - 1;

  double d = (B * B) - (4 * A * C);

  if (d >= 0) {
    t[0] = (-B - sqrt(d)) / (2 * A);
    t[1] = (-B + sqrt(d)) / (2 * A);

    for (int i = 0; i < 2; i++) {
      if (t[i] > EPSILON) { // So it doesn't cast shadows on itself
        // find intersection
        Intersection temp;
        temp.NEAR = ray.O + t[i] * ray.D;
        if (temp.NEAR[1] <= Y_MAX && temp.NEAR[1] >= Y_MIN) { // valid intersection point
          // calculate normal
          temp.NORMAL = temp.NEAR - CENTRE;
          temp.NORMAL[1] = temp.NEAR[1];

          if (t[i] > t_far) {
            t_far = t[i];
            intersection.FAR = temp.NEAR;
            intersection.FAR_NORMAL = temp.NORMAL;
          }
          if (t[i] < t_near) {
            t_near = t[i];
            intersection.NEAR = temp.NEAR;
            intersection.NORMAL = temp.NORMAL;

          }
        }
      }
    }
  }

  // caps (TODO clean up a bit)

  Intersection temp;
  double t_cap;
  bool hit;

  hit = intersect_circle(ray, TOP_POINT, RADIUS, TOP_NORMAL, temp, t_cap);

  if (hit && t_cap > EPSILON) {
    if (t_cap > t_far) {
      t_far = t_cap;
      intersection.FAR = temp.NEAR;
      intersection.FAR_NORMAL = TOP_NORMAL;
    }
    if (t_cap < t_near) {
      t_near = t_cap;
      intersection.NEAR = temp.NEAR;
      intersection.NORMAL =  TOP_NORMAL;
    }

  }

  hit = intersect_circle(ray, BOTTOM_POINT, RADIUS, BOTTOM_NORMAL, temp, t_cap);

  if (hit && t_cap > EPSILON) {
    if (t_cap > t_far) {
      t_far = t_cap;
      intersection.FAR = temp.NEAR;
      intersection.FAR_NORMAL = BOTTOM_NORMAL;
    }
    if (t_cap < t_near) {
      t_near = t_cap;
      intersection.NEAR = temp.NEAR;
      intersection.NORMAL = BOTTOM_NORMAL;
    }

  }

  if (t_near < DOUBLE_MAX && t_near > EPSILON) {
    intersection.uv[0] = std::acos(intersection.NEAR[0]) / (double)(2 * M_PI);
    intersection.uv[1] = intersection.NEAR[1];

    return true;
  } else {
    return false;
  }
}

