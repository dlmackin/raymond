/*
 * photon_map.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "photon_map.hpp"
#include "ray.hpp"
#include "global.hpp"

PhotonMap::PhotonMap(SceneNode *root, const std::list<Light*>& lights, int size) :
  root(root), lights(lights), size(size), stored_photons(0), misses(0) {
  // TODO Auto-generated constructor stub

}

PhotonMap::~PhotonMap() {
  // TODO Auto-generated destructor stub
}

void PhotonMap::build() {
  std::cerr << "building photon map" << std::endl;
  map = new Photon*[size];
  int posn = 0;

  srand(0); // seed the random # generator

  int interval = (size / 10);
  int next_update = interval;

  flag = false;
  // loop through lights
  for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
    while (stored_photons < size) {
      if (flag) {
        break;
      }

      if (stored_photons >= next_update) {
        std::cerr << "mapped " << stored_photons << " photons (" << misses << " misses)"
            << std::endl;
        next_update += interval;
      }

      // Generate a ray
      Ray r;
      r.D = Vector3D(get_rand(), get_rand(), get_rand()); // random direction
      r.O = (*I)->position; // origin at light

      //      std::cerr << "emitting photon towards " << r.D << std::endl;

      int ret = emit_photon(r, (*I), 1);

    }
  }

  std::cout << "stored " << stored_photons << " photons (" << misses << " misses)" << std::endl;
}

int PhotonMap::emit_photon(Ray ray, Light* light, int depth) {
  int stored = 0;

  Intersection intersection;
  SceneNode *node;

  bool hit = root->intersect(ray, intersection, node, false);

  if (hit) {
    if (node->get_name() == "glass sphere") {
      //      std::cerr << "hit " << node->get_name() << " at " << intersection.NEAR << " (depth " << depth << ")" << std::endl;
    }

    double refraction = ((GeometryNode *) node)->get_material()->get_refraction();
    double reflection = ((GeometryNode *) node)->get_material()->get_reflection();

    if (depth > 1 && refraction <= 0 && reflection <= 0) {
      //      std::cerr << "storing hit " << node->get_name() << " at " << intersection.NEAR << " (depth " << depth << ")" << std::endl;
      // store
      Photon *photon = new Photon();
      photon->d = ray.D;
      photon->p = intersection.NEAR;
      photon->colour = new Colour(0.5, 0.5, 0.5); // TODO

//      photon->colour = &(((GeometryNode *) node)->get_ambient_intensity(intersection) * 0.9);

      map[stored_photons] = photon;
      stored_photons++;
      stored++;
    }

    if (refraction > 0 || reflection > 0) {
      double rand = drand48();

      // TODO this but properly
      double p_refract = refraction / (double) (std::max(1.0, refraction + reflection));

      p_refract = refraction;

      if (rand < p_refract) {
        // transmit
        Ray next;

        next.O = intersection.NEAR;

        double refractive_index = ((GeometryNode *) node)->get_material()->get_refractive_index();

        Vector3D normal = Vector3D(intersection.NORMAL);
        bool incoming = (ray.D.dot(normal) < 0);
        double n;
        if (incoming) {
          n = 1 / refractive_index;
        } else {
          n = refractive_index;
          normal = -normal;
        }

        Vector3D incident = Vector3D(ray.D);
        normal.normalize();
        incident.normalize();
        double cosI = normal.dot(incident);
        double sinT2 = n * n * (1 - cosI * cosI);

        double theta = std::acos(cosI / (double) (normal.length() * incident.length()));

        if (sinT2 > 1) {
          //          std::cerr << "TIR: " <<  180 * theta / M_PI << " degrees" << std::endl;
          return stored;
        }

        next.D = n * incident - (n + std::sqrt(1 - sinT2)) * normal;

        stored += emit_photon(next, light, depth + 1);

      } else { // REFLECT
//        Ray next;
//        next.O = Point3D(intersection.NEAR);
//        next.D = Vector3D(ray.D + (2 * -(intersection.NORMAL.dot(ray.D)) * intersection.NORMAL));
//
//        stored += emit_photon(next, depth + 1);
      }

    }

  } else {
    misses++;
  }

  return stored;
}

Photon * PhotonMap::get_closest(Point3D p) {
  Photon *closest = NULL;
  double dist = DOUBLE_MAX;

  for (int i = 0; i < stored_photons; i++) {
    if (std::abs((p - map[i]->p).length()) < dist) {
      closest = map[i];
    }
  }
  return closest;

}

Photon ** PhotonMap::get_closest_x(Point3D p, int x, Photon** closest) {

  int items = 1;

  closest[0] = map[0];
  closest[0]->dist = std::abs((p - map[0]->p).length());

  for (int i = 1; i < stored_photons; i++) {
    map[i]->dist = std::abs((p - map[i]->p).length());

    for (int j = 0; j <= items; j++) {
      if (j == items) {
        if (items < x) {
          closest[j] = map[i];
          items++;
        }
        break;
      }

      if (map[i]->dist < closest[j]->dist) {
        // insert
        for (int k = items; k > j; k--) {

          closest[k] = closest[k - 1];
        }

        closest[j] = map[i];
        if (items < x) {
          items++;
        }
        break;
      } else {
      }
    }
  }

  return closest;
}

float PhotonMap::get_rand() {
  return ((2) * ((float) rand() / RAND_MAX)) - 1;
}
