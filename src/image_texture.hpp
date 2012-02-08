/*
 * image_texture.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef IMAGE_TEXTURE_HPP_
#define IMAGE_TEXTURE_HPP_

#include "sphere_texture.hpp"
#include "image.hpp"

class ImageTexture: public SphereTexture {
public:
  ImageTexture(const std::string& filename);
  virtual ~ImageTexture();
protected:
  virtual Colour get_texture_for_point(Point2D uv);
private:
  Image image;
  const std::string& filename;

  void load_texture_file();
};

#endif /* IMAGE_TEXTURE_HPP_ */
