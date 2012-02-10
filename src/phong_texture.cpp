/*
 * phong_texture.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "phong_texture.hpp"
#include "global.hpp"

#define RADIUS 1

bool flag = false;

PhongTexture::PhongTexture(const std::string& filename, const Colour& ks, double shininess,
    double reflection) :
  RMaterial(Colour(0), ks, shininess, reflection, 0, 0), filename(filename), success(false) {
    load_texture_file();
  }

PhongTexture::~PhongTexture() {
  // TODO Auto-generated destructor stub
}

void PhongTexture::load_texture_file() {
  std::cerr << "filename is " << filename << std::endl;
  success = image.loadPng(filename);
  if (!success) {
    std::cerr << "unable to read " << filename << std::endl;
  } else {
    std::cerr << "dims: " << image.width() << " x " << image.height() << std::endl;
  }
}

Colour PhongTexture::get_kd(const Intersection& intersection) {

  Colour colour = get_texture_for_point(intersection.uv);
  return colour;
}

Colour PhongTexture::get_texture_for_point(Point2D uv) {
  if (success) {
    double u = uv[0] - int(uv[0]);
    double v = uv[1] - int(uv[1]);

    double x = u * image.width();
    double y = v * image.height();

    double r = image(x, y, 0);
    double g = image(x, y, 1);
    double b = image(x, y, 2);

    return Colour(r, g, b);
  } else {
    // Procedural texture for horizontal stripes

    Colour ret(0, 0, 0);

    double u = uv[0] - int(uv[0]);
    double v = uv[1] - int(uv[1]);

    double val = std::sin((double) (v * 2 * M_PI) * 10.0);

    if (val < -EPSILON) {
      ret = Colour(1, 1, 1);
    } else {
      ret = Colour(0, 0, 0);
    }

    return ret;
  }

}
