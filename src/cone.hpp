/*
 * cone.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CONE_HPP_
#define CONE_HPP_

#include "primitive.hpp"

class Cone: public Primitive {
public:
  Cone();
  virtual ~Cone();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM

private:
  static const double RADIUS = 1;
  static const double HEIGHT = 1;
  static const Point3D CAP_CENTRE;
  static const Vector3D CAP_NORMAL;
};

#endif /* CONE_HPP_ */
