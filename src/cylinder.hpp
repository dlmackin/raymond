/*
 * cylinder.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CYLINDER_HPP_
#define CYLINDER_HPP_

#include "primitive.hpp"

class Cylinder: public Primitive {
public:
  Cylinder();
  virtual ~Cylinder();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM
private:
  static const int RADIUS = 1;
  static const Point3D CENTRE;
  static const Point3D TOP_POINT;
  static const Vector3D TOP_NORMAL;
  static const Point3D BOTTOM_POINT;
  static const Vector3D BOTTOM_NORMAL;
};

#endif /* CYLINDER_HPP_ */
