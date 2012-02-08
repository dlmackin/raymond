/*
 * image_texture.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "image_texture.hpp"

ImageTexture::ImageTexture(const std::string& filename) :
  filename(filename), image() {
  // TODO Auto-generated constructor stub
  load_texture_file();

}

ImageTexture::~ImageTexture() {
  // TODO Auto-generated destructor stub
}

Colour ImageTexture::get_texture_for_point(Point2D uv) {
  double u = uv[0] - int(uv[0]);
  double v = uv[1] - int(uv[1]);

  double x = u * image.width();
  double y = v * image.height();

//  std::cerr << "elements = " << image.elements() << std::endl;
  double r = image(x,y,0);
  double g = image(x,y,1);
  double b = image(x,y,2);

  return Colour(r,g,b);

}

void ImageTexture::load_texture_file() {
  std::cerr << "filename is " << filename << std::endl;
  bool success = image.loadPng(filename);
  if (!success) {
    std::cerr << "unable to read " << filename << std::endl;
  }

  std::cerr << "dims: " << image.width() << " x " << image.height() << std::endl;
}
