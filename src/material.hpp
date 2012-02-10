/*
 * material.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CS488_MATERIAL_HPP
#define CS488_MATERIAL_HPP

#include "algebra.hpp"
#include "light.hpp"
#include <list>
#include "intersection.hpp"

class Material {
public:
  virtual ~Material();
  virtual Colour colour_at(const Intersection& intersection, const Vector3D* view,
                           const Light *light); // DLM
  virtual double get_reflection() { return 0; }
  virtual double get_refraction() { return 0; }
  virtual double get_refractive_index() { return 0; }
  virtual Colour get_kd() { return Colour(0,0,0); }
  virtual Colour get_kd(const Intersection& intersection) { return get_kd(); }
protected:
  Material()
  {
  }
};

class PhongMaterial : public Material {
public:
  PhongMaterial(const Colour& kd, const Colour& ks, double shininess);
  virtual ~PhongMaterial();

  virtual Colour get_kd(); // TODO remove later
  virtual Colour get_kd(const Intersection& intersection) { return get_kd(); }
  virtual Vector3D get_normal(const Intersection& intersection) { return intersection.NORMAL; }
  virtual Colour colour_at(const Intersection& intersection, const Vector3D* view,
                           const Light *light); // DLM

private:
  Colour m_kd;
  Colour m_ks;

  double m_shininess;
};


class RMaterial : public PhongMaterial {
public:
  RMaterial(const Colour& kd, const Colour& ks, double shininess, double reflection, 
      double refraction, double refractive_index);
  virtual double get_reflection() { return reflection; }
  virtual double get_refraction() { return refraction; }
  virtual double get_refractive_index() { return refractive_index; }
private:
  double reflection;
  double refraction;
  double refractive_index;
};

#endif
