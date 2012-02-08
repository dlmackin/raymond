/*
 * sphere.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "primitive.hpp"
#include "global.hpp"

Sphere::Sphere() : m_radius(1), m_pos(Point3D(0,0,0)) {

}

Sphere::~Sphere() {
}

bool Sphere::intersect(const Ray& ray, Intersection& intersection, bool quick) {

  // TODO this is pretty dirty...
  Vector3D v = m_pos - ray.O;
  if (ray.D.dot(v) <= 0) {
    return false;
  }

  double A = ray.D.dot(ray.D);
  double B = 2 * ray.D.dot(ray.O - m_pos);
  double C = (ray.O - m_pos).dot(ray.O - m_pos) - (m_radius * m_radius);

  double d = (B * B) - (4 * A * C);

  if (d < 0) {
    return false;
  }

  // HIT! find intersection point & return
  if (quick)
    return true;

  double t1 = (-B - sqrt(d)) / (2 * A);
  double t2 = (-B + sqrt(d)) / (2 * A);

  if (t1 < EPSILON) {
    t1 = t2;
  }

  if (t1 < EPSILON)
    return false;

  intersection.NEAR = ray.O + t1 * ray.D;
  intersection.FAR = ray.O + t2 * ray.D;
  // get normal too!
  normal(intersection);

  // caculate uv coordinates
  calculate_uv(intersection);

  return true;
}

void Sphere::calculate_uv(Intersection &intersection) {
  double theta = std::acos(-intersection.NEAR[1] / (double) m_radius);
  double phi = std::atan2(intersection.NEAR[2], -intersection.NEAR[0]);

  while (phi < 0) {
    phi += 2 * M_PI;
  }

  Point2D uv;
  uv[0] = phi / (double) (2.0 * M_PI);
  uv[1] = ((double) M_PI - theta) / (double) M_PI;

  intersection.uv = uv;
}

// TODO
Vector3D Sphere::normal(Intersection& point) {
  point.NORMAL = (point.NEAR - Point3D(0, 0, 0));
  point.FAR_NORMAL = (point.FAR - Point3D(0, 0, 0));
  return (point.NEAR - Point3D(0, 0, 0));
}
