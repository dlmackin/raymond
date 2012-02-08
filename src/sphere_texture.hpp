/*
 * sphere_texture.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef SPHERE_TEXTURE_HPP_
#define SPHERE_TEXTURE_HPP_

#include "material.hpp"

class SphereTexture: public Material {
public:
  SphereTexture();
  virtual ~SphereTexture();
  virtual Colour colour_at(const Intersection& intersection, const Vector3D& view,
      const Light *light); // DLM
protected:
  virtual Colour get_texture_for_point(Point2D uv);

private:
  double RADIUS; // TODO keep this w/ the sphere primitive...

};

#endif /* SPHERE_TEXTURE_HPP_ */
