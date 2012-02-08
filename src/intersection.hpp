#ifndef CS488_INTERSECTION_HPP
#define CS488_INTERSECTION_HPP

#include "algebra.hpp"

// Data structure for passing around intersection info.

struct Intersection {
  Point3D NEAR;
  Point3D FAR;
  Vector3D NORMAL;
  Vector3D FAR_NORMAL;
  
  Point2D uv;

  void transform(Matrix4x4 M) {
    NEAR = M * NEAR;
    FAR = M * FAR;
  }
};

#endif // CS488_INTERSECTION_HPP
