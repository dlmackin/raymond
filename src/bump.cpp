/*
 * bump.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "bump.hpp"

BumpMaterial::BumpMaterial(const std::string& filename, const Colour& kd, const Colour& ks,
    double shininess) :
  PhongMaterial(kd, ks, shininess), filename(filename) {
  bool success = bump_map.loadPng(filename);
  if (!success) {
    std::cerr << "unable to read " << filename << std::endl;
  }

}

BumpMaterial::~BumpMaterial() {
  // TODO Auto-generated destructor stub
}

Vector3D BumpMaterial::get_normal(const Intersection& intersection) {

  Point2D uv = intersection.uv;

  double u = uv[0] - int(uv[0]);
  double v = uv[1] - int(uv[1]);

  double x = u * bump_map.width();
  double y = v * bump_map.height();

  double x_gradient = bump_map(x-1, y, 0) - bump_map(x+1, y, 0);
  double y_gradient = bump_map(x, y-1, 0) - bump_map(x, y+1, 0);

  Vector3D normal = intersection.NORMAL;

  normal[0] = normal[0] + (u * x_gradient) + (v * y_gradient);
  normal[1] = normal[1] + (u * x_gradient) + (v * y_gradient);
  normal[2] = normal[2] + (u * x_gradient) + (v * y_gradient);

  //  x_gradient = pixel(x-1, y) - pixel(x+1, y)
  //    y_gradient = pixel(x, y-1) - pixel(x, y+1)


  return normal;
}
