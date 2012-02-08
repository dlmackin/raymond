/*
 * ray_tracer.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef RAY_TRACER_HPP_
#define RAY_TRACER_HPP_

#include "algebra.hpp"
#include "scene.hpp"
#include "light.hpp"
#include "image.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "global.hpp"
#include "photon_map.hpp"

class RayTracer {
public:
  RayTracer(// What to render
      SceneNode* root,
      // Where to output the image
      const std::string& filename,
      // Image size
      int width, int height,
      // Viewing parameters
      const Point3D& eye, const Vector3D& view,
      const Vector3D& up, double fov,
      // Lighting parameters
      const Colour& ambient,
      const std::list<Light*>& lights,
      Settings *settings);
  virtual ~RayTracer();

  bool run();
private:
  SceneNode * root;
  const std::string& filename;
  int width, height;

  const Point3D& eye;
  const Vector3D& view;
  const Vector3D& up;
  double fov;

  const Colour& ambient;
  const std::list<Light*>& lights;

  Settings *settings;

  int hits;
  int next_update;
  int x, y;
  int num_rays;

  PhotonMap *photon_map;

  Colour background(double x, double y, int width, int height);
  Colour trace_ray(Ray ray, double x, double y, int depth);
  void progress_report();

  void show_photons();
  Point2D get_pixel_for_photon(Photon *photon);
};

#endif /* RAY_TRACER_HPP_ */
