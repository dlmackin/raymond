/*
 * primitive.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CS488_PRIMITIVE_HPP
#define CS488_PRIMITIVE_HPP

#include "algebra.hpp"
#include "intersection.hpp"
#include "ray.hpp"

class Primitive {
public:
  virtual ~Primitive();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick) = 0; // DLM
};

class Sphere : public Primitive {
public:
  Sphere();
  virtual ~Sphere();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM
  virtual void calculate_uv(Intersection &intersection);
private:
  Point3D m_pos;
  double m_radius;
  virtual Vector3D normal(Intersection& point); // DLM
};

class Cube : public Primitive {
public:
  virtual ~Cube();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM

private:
  virtual Vector3D normal(Intersection& point); // DLM
  double intersect_plane(const Ray& ray, Point3D& p0, Vector3D& n, double& d); // DLM
  // TODO move both of these to some util file
  bool in_box(Point3D point, Point3D box_min, Point3D box_max, int plane);
  Point3D closest(Point3D origin, Point3D p0, Point3D p1);
};

class NonhierSphere : public Primitive {
public:
  NonhierSphere(const Point3D& pos, double radius)
    : m_pos(pos), m_radius(radius)
  {
  }
  virtual ~NonhierSphere();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM
  virtual Vector3D normal(Intersection& point); // DLM
private:
  Point3D m_pos;
  double m_radius;
};

class NonhierBox : public Primitive {
public:
  NonhierBox(const Point3D& pos, double size)
    : m_pos(pos), m_size(size)
  {
  }
  
  virtual ~NonhierBox();
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM
  virtual Vector3D normal(Intersection& point); // DLM
private:
  Point3D m_pos;
  double m_size;
  
  double intersect_plane(const Ray& ray, Point3D& p0, Vector3D& n, double& d); // DLM
  // TODO move both of these to some util file
  bool in_box(Point3D point, Point3D box_min, Point3D box_max, int plane);
  Point3D closest(Point3D origin, Point3D p0, Point3D p1);
};

#endif
