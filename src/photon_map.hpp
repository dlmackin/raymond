/*
 * photon_map.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef PHOTON_MAP_HPP_
#define PHOTON_MAP_HPP_

#include "photon.hpp"
#include "scene.hpp"


class PhotonMap {
public:
  PhotonMap(SceneNode *root, const std::list<Light*>& lights, int size);
  virtual ~PhotonMap();

  void build();
  Photon * get_closest(Point3D p);
  Photon ** get_closest_x(Point3D p, int x, Photon** closest);
  Photon **get_map() { return map; }
  int get_size() { return stored_photons; }
private:
  SceneNode * root;
  const std::list<Light*>& lights;
  Photon **map;
  int size;
  int stored_photons;
  int misses;
  bool flag;
  float get_rand();
  int emit_photon(Ray ray, Light* light, int depth);
};

#endif /* PHOTON_MAP_HPP_ */
