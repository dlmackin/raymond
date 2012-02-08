/*
 * Utils.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CS488_UTILS_HPP_
#define CS488_UTILS_HPP_

#include "algebra.hpp"
#include "global.hpp"
#include "ray.hpp"
#include "intersection.hpp"

bool intersect_circle(const Ray & ray, Point3D centre, double radius, Vector3D normal,
    Intersection& intersection, double & t);

#endif /* UTILS_HPP_ */
