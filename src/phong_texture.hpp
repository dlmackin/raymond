/*
 * phong_texture.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef PHONG_TEXTURE_HPP_
#define PHONG_TEXTURE_HPP_

#include "material.hpp"
#include "image.hpp"

class PhongTexture: public RMaterial {
public:
  PhongTexture(const std::string& filename, const Colour& ks, double shininess, double reflection);
  virtual ~PhongTexture();
  virtual Colour get_kd(const Intersection& intersection);
private:
  Image image;
  const std::string& filename;
  bool success;

  void load_texture_file();
  Colour get_texture_for_point(Point2D uv);
};

#endif /* PHONG_TEXTURE_HPP_ */
