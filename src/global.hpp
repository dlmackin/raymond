/*
 * global.hpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CS488_GLOBAL_HPP
#define CS488_GLOBAL_HPP

#include <limits>

// CONSTANTS
#define PI 3.14159265

#define DOUBLE_MIN std::numeric_limits<double>::min()
#define DOUBLE_MAX std::numeric_limits<double>::max()

#define EPSILON 0.001

// TODO It might be nice to make this a configurable setting.
#define UPDATE_INTERVAL 5

// GLOBALS
// TODO! figure out a better way to share settings, this is pretty dirty.
#include "settings.hpp"
extern Settings *settings;

#endif
