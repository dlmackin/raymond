/*
 * photon.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef PHOTON_HPP_
#define PHOTON_HPP_

#include "algebra.hpp"

struct Photon {
  Point3D p;
  Colour *colour;
  Vector3D d;
  bool flag; // maybe later
  double dist; // handy
};


#endif /* PHOTON_HPP_ */
