#ifndef CS488_RAY_HPP
#define CS488_RAY_HPP

#include "algebra.hpp"

// Data structure for passing around intersection info.

struct Ray {
  Point3D O; // ray origin
  Vector3D D; // ray direction
  
  void transform(Matrix4x4 M) {
    O = M * O;
    D = M * D;
  }
};

#endif // CS488_RAY_HPP
