/*
 * project.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "project.hpp"

#include <math.h>

#include "image.hpp"
#include "intersection.hpp"
#include "ray.hpp"
#include "global.hpp"
#include "ray_tracer.hpp"

void project_render(// What to render
    SceneNode* root,
    // Where to output the image
    const std::string& filename,
    // Image size
    int width, int height,
    // Viewing parameters
    const Point3D& eye, const Vector3D& view, const Vector3D& up, double fov,
    // Lighting parameters
    const Colour& ambient, const std::list<Light*>& lights) {
  // Fill in raytracing code here.

  RayTracer *ray_tracer = new RayTracer(root, filename, width, height, eye, view, up, fov, ambient,
      lights, settings);

  int samples_per_pixel = settings->samples * settings->samples;
  double weight = (1 / (double) samples_per_pixel);

  // TODO this causes a crash in OSX...
  // std::cerr.imbue(std::locale("")); // use thousands separator
  // std::cout.imbue(std::locale("")); // use thousands separator

  std::cerr << "Rendering scene...\n     " << "root: " << root << "\n     " << filename << ", "
      << width << "x" << height << "\n     " << "eye: " << eye << ", " << "direction: " << view
      << ", " << "up: " << up << ", " << "fov: " << fov << "\n     " << "ambient: " << ambient
      << "\n     lights:";

  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    std::cerr << "\n          " << **I;
  }
  std::cerr << "" << std::endl;
  std::cerr << std::endl;

  std::cerr << "settings: " << std::endl << "  stochastic? " << settings->stochastic << std::endl
      << "  samples: " << samples_per_pixel << std::endl << "  weight: " << weight << std::endl;

  ray_tracer->run();
}
