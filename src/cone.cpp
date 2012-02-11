/*
 * cone.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "cone.hpp"
#include "global.hpp"
#include "utils.hpp"

const Point3D Cone::CAP_CENTRE = Point3D(0, 0, 0);
const Vector3D Cone::CAP_NORMAL = Vector3D(0, -1, 0);

int cflag = 0;

const double Cone::RADIUS;
const double Cone::HEIGHT;

Cone::Cone() {
  // TODO Auto-generated constructor stub

}

Cone::~Cone() {
  // TODO Auto-generated destructor stub
}

bool Cone::intersect(const Ray & ray, Intersection & intersection, bool quick) {
  Intersection near, far;
  double t_near = DOUBLE_MAX, t_far = 0;
  double t[2];

  double x_D = ray.D[0];
  double y_D = ray.D[1];
  double z_D = ray.D[2];
  double x_O = ray.O[0];
  double y_O = ray.O[1];
  double z_O = ray.O[2];

  double fac = (RADIUS * RADIUS) / (double) (HEIGHT * HEIGHT);

  // cylinder part

  double yA = (fac) * y_D * y_D;
  double yB = (2 * fac * y_O * y_D) - (2 * (RADIUS * RADIUS) / HEIGHT) * y_D;
  double yC = (fac * y_O * y_O) - ((2 * (RADIUS * RADIUS) / HEIGHT) * y_O) + (RADIUS * RADIUS);

  // TODO clean up
  double A = (x_D * x_D) + (z_D * z_D) - yA;
  double B = (2 * x_O * x_D) + (2 * z_O * z_D) - yB;
  double C = (x_O * x_O) + (z_O * z_O) - yC;

  double d = (B * B) - (4 * A * C);

  if (d >= 0) {
    t[0] = (-B - sqrt(d)) / (2 * A);
    t[1] = (-B + sqrt(d)) / (2 * A);
    for (int i = 0; i < 2; i++) {
      if (t[i] > EPSILON) { // So it doesn't cast shadows on itself
        // find intersection
        Intersection temp;
        temp.NEAR = ray.O + t[i] * ray.D;
        if (temp.NEAR[1] <= HEIGHT && temp.NEAR[1] >= 0) { // valid intersection point

          double a = 1, d = (-1 * RADIUS * RADIUS) / (double) (HEIGHT * HEIGHT), f = 1, h = (2
              * RADIUS * RADIUS) / HEIGHT;

          double B = std::sqrt(2);

          double mag = std::sqrt(temp.NEAR[0] * temp.NEAR[0] + temp.NEAR[2] * temp.NEAR[2]);

          double costheta = temp.NEAR[0] / (double) (mag);

          temp.NORMAL[0] = 1 / B * costheta;
          temp.NORMAL[1] = 1 / B;
          temp.NORMAL[2] = 1 / B * (1 - costheta);

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

  intersection.NORMAL.normalize();

  // base
  Intersection temp;
  double t_cap;
  bool hit = intersect_circle(ray, CAP_CENTRE, RADIUS, CAP_NORMAL, temp, t_cap);

  if (hit) {
    if (t_cap > t_far) {
      t_far = t_cap;
      intersection.FAR = temp.NEAR;
      intersection.FAR_NORMAL = temp.NORMAL;
    }
    if (t_cap < t_near) {
      t_near = t_cap;
      intersection.NEAR = temp.NEAR;
      intersection.NORMAL = temp.NORMAL;
    }
  }

  if (t_near < DOUBLE_MAX) {
    return true;
  } else {
    return false;
  }
}

