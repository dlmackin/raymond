/*
 * sphere_texture.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "sphere_texture.hpp"

SphereTexture::SphereTexture() {
  RADIUS = 1;
  // TODO Auto-generated constructor stub

}

SphereTexture::~SphereTexture() {
  // TODO Auto-generated destructor stub
}

Colour SphereTexture::colour_at(const Intersection& intersection, const Vector3D& view,
    const Light *light) {
  return get_texture_for_point(intersection.uv);
}

Colour SphereTexture::get_texture_for_point(Point2D uv) {

  // Procedural texture for horizontal stripes

  Colour ret(0, 0, 0);

  double u = uv[0] - int(uv[0]);
  double v = uv[1] - int(uv[1]);

  double val = std::sin((double) (v * 2 * M_PI) * 10.0);

  if (val < 0) {
    ret = Colour(0, 0, 0);
  } else {
    ret = Colour(1, 1, 1);
  }

  return ret;
}

