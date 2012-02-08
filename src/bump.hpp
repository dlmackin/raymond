/*
 * bump.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef BUMP_HPP_
#define BUMP_HPP_

#include "material.hpp"
#include "image.hpp"

class BumpMaterial: public PhongMaterial {
public:
  BumpMaterial(const std::string& filename, const Colour& kd, const Colour& ks, double shininess);
  virtual ~BumpMaterial();
  virtual Vector3D get_normal(const Intersection& intersection);
private:
  const std::string& filename;
  Image bump_map;
};

#endif /* BUMP_HPP_ */
