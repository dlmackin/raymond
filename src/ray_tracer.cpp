/*
 * ray_tracer.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "ray_tracer.hpp"

#define COUNTERS 5
int counter[COUNTERS] = { 0 };

RayTracer::RayTracer(SceneNode* root, const std::string& filename, int width, int height,
    const Point3D& eye, const Vector3D& view, const Vector3D& up, double fov,
    const Colour& ambient, const std::list<Light*>& lights, Settings *settings) :

  root(root), filename(filename), width(width), height(height), eye(eye), view(view), up(up), fov(
      fov), ambient(ambient), lights(lights), settings(settings), hits(0),
      num_rays(height * width), photon_map(new PhotonMap(root, lights, settings->num_photons)) {
}

RayTracer::~RayTracer() {
  // TODO Auto-generated destructor stub
}

void RayTracer::progress_report() {
  int pct = (double) ((x * y) / (double) num_rays) * 100;
  if (pct >= next_update) {
    std::cout << pct << "% complete (" << hits << " hits)" << std::endl;
    next_update += UPDATE_INTERVAL;
  }

}

/*
 * Trace all the rays!
 */
bool RayTracer::run() {
  int samples_per_pixel = settings->samples * settings->samples;
  double weight = (1 / (double) samples_per_pixel);

  const double fovy = (height / width) * fov;
  const double tanx = tan(fov * PI / 180 * 0.5);
  const double tany = tan(fovy * PI / 180 * 0.5);

  // TODO photon map
  if (settings->show_photons) {
    photon_map->build();
    if (settings->direct_map) {
      show_photons();
      return false;
    }
  }

  Point3D pixel = Point3D(0, 0, eye[2] - 1);

  // The image to write to
  Image img(width, height, 3);

  int miny = 0, maxy = height, minx = 0, maxx = width;
  int ctr = 0;

  next_update = UPDATE_INTERVAL; // When to do the next progress update.
  int rays = 0;
  // FOR EACH PIXEL
  for (y = miny; y < maxy; y++) {
    for (x = minx; x < maxx; x++) {

      // Potentially output a progress update.
      progress_report();

      // Take samples for this pixel
      Colour *samples[samples_per_pixel];
      int idx = 0;
      double x_sample, y_sample;
      for (int i = 0; i < settings->samples; i++) {
        for (int j = 0; j < settings->samples; j++) {

          if (settings->stochastic) {
            x_sample = x + (i / (double) settings->samples) + (1 / (double) settings->samples) * drand48();
            y_sample = y + (j / (double) settings->samples) + (1 / (double) settings->samples) * drand48();
          } else {
            x_sample = x + (i / (double) settings->samples);
            y_sample = y + (j / (double) settings->samples);
          }

          pixel = eye;
          pixel[2] = pixel[2] - 1;
          rays++;
          pixel[0] = eye[0] + ((2 * x_sample - width) / (double) width) * tanx;
          pixel[1] = eye[1] + ((double) (2 * y_sample - height) / (double) height) * tany;

          Ray ray;
          ray.O = eye;
          ray.D = pixel - eye;
          ray.D.normalize();

          Colour sample_colour = trace_ray(ray, x_sample, y_sample, 0);

          samples[idx] = new Colour(sample_colour);
          idx++;
        }
      }

      // Average the sampled colours.
      Colour pixel_colour = Colour(0);
      for (int i = 0; i < samples_per_pixel; i++) {
        pixel_colour = pixel_colour + *(samples[i]);
      }
      pixel_colour = pixel_colour * weight;

      // Write the final colour for this pixel.
      img(x, height - y - 1, 0) = pixel_colour.R();
      img(x, height - y - 1, 1) = pixel_colour.G();
      img(x, height - y - 1, 2) = pixel_colour.B();

    } // end for (x)
  } // end for (y)

  std::cout << std::endl << "Rendering complete!" << std::endl;
  std::cout << hits << " hits" << ", " << " (" << rays << " rays)" << std::endl;

  // print counters:
  for (int i = 0; i < COUNTERS; i++) {
    std::cerr << "Counter " << i << " = " << counter[i] << std::endl;
  }

  img.savePng(filename);

  return false;
}

/*
 * Background generator. Returns a colour for the given position in the image.
 * Currently just a gradient from blue to black.
 * TODO: Might want to add more interesting ones, or make it user defined
 * somehow.
 */
Colour RayTracer::background(double x, double y, int width, int height) {
  return Colour(0, 0, 1 - (double) y / height);
}

Colour RayTracer::trace_ray(Ray ray, double x, double y, int depth) {
  if (depth > 10) {
    return Colour(0, 0, 0);
  }

  Colour sample_colour(0);

  Intersection intersection, tmp;
  GeometryNode *node;
  SceneNode *tmp_node;

  bool hit = root->intersect(ray, intersection, (SceneNode *&) node, settings->simple);
  if (hit) {
    hits++;

    Colour photon_colour = Colour(0);

    if (settings->show_photons) {

      int n = 5;
      Photon *closest[n];
      photon_map->get_closest_x(intersection.NEAR, n, closest);

      double dist = closest[n - 1]->dist;
      double tmp = 1 / (double) (dist);
      for (int i = 0; i < n; i++) {
        photon_colour = photon_colour + *(closest[i]->colour);
      }

      photon_colour = photon_colour * tmp;

      if (settings->direct_map) {
        return photon_colour;
      }
    }

    intersection.NORMAL.normalize();
    if (depth > 0) {
      counter[2]++;
    }

    if (settings->simple) {
      sample_colour = Colour(1, 0, 0);
      return sample_colour;
    }

    Vector3D view_dir = eye - intersection.NEAR;
    view_dir.normalize();
    sample_colour = node->get_ambient_intensity(intersection) * ambient;

    for (std::list<Light*>::const_iterator I = lights.begin(); I != lights.end(); ++I) {
      Ray *shadow_ray = new Ray();
      shadow_ray->O = Point3D(intersection.NEAR);
      shadow_ray->D = Vector3D((*I)->position - Point3D(intersection.NEAR));

      Point3D p = intersection.NEAR;

      Intersection i_copy = Intersection(intersection);

      if (intersection.NORMAL.dot(shadow_ray->D) < 0) {
        continue;
      }

      // TODO (maybe) fix the shadow intersection thing for realsies
      if (root->intersect(*shadow_ray, tmp, tmp_node, false) && (tmp.NEAR - shadow_ray->O).length()
          < shadow_ray->D.length()) {
        continue; // skip lights that are blocked
      } else {
        Colour add = node->colour_at(i_copy, &view_dir, (*I));
        if (add.R() != add.R()) {
          // Something's gone terribly wrong
          std::cerr << "something went wrong..." << std::endl;
        } else {
          sample_colour = sample_colour + add;
        }
      }
    }

    double reflection = node->get_material()->get_reflection();
    double refraction = node->get_material()->get_refraction();
    double refractive_index = node->get_material()->get_refractive_index();

    Colour reflected_colour(0, 0, 0), refracted_colour(0, 0, 0);

    // if (reflective)
    if (reflection > 0) {
      counter[0]++;
      // generate secondary ray
      Ray secondary_ray;
      secondary_ray.O = Point3D(intersection.NEAR);
      secondary_ray.D = Vector3D(ray.D + (2 * -(intersection.NORMAL.dot(ray.D))
          * intersection.NORMAL));

      // intersect it and get the colour
      reflected_colour = trace_ray(secondary_ray, x, y, depth + 1);
      if (!(reflected_colour.R() == 0 && reflected_colour.G() == 0 && reflected_colour.B() == 0)) {
        counter[1]++;
      }
    }

    // if refractive
    if (refraction > 0) {
      // generate secondary ray
      Ray secondary_ray;
      secondary_ray.O = Point3D(intersection.NEAR);
      Vector3D normal = Vector3D(intersection.NORMAL);
      Vector3D incident = Vector3D(ray.D);

      normal.normalize();
      incident.normalize();

      bool incoming = (incident.dot(normal) < 0);
      double n;
      if (incoming) {
        n = 1 / refractive_index;
      } else {
        n = 1 / refractive_index;
        normal = -normal;
      }

      double cosI = normal.dot(incident);
      double sinT2 = n * n * (1 - cosI * cosI);

      double theta = std::acos(cosI / (double) (normal.length() * incident.length()));

      if (sinT2 > 1) {
        counter[3]++;
      } else {
        counter[4]++;
        secondary_ray.D = n * incident - (n + std::sqrt(1 - sinT2)) * normal;

        // intersect it and get the colour
        refracted_colour = trace_ray(secondary_ray, x, y, depth + 1);
      }

    }

    sample_colour = sample_colour + (reflection * reflected_colour) + (refraction
        * refracted_colour) + photon_colour;

  } else if (depth == 0) {
    sample_colour = background(x, y, width, height);
  } else {
    sample_colour = Colour(0, 0, 0);
  }

  return sample_colour;

}

void RayTracer::show_photons() {

  Image img(width, height, 3);

  // foreach photon
  Photon **map = photon_map->get_map();
  int size = photon_map->get_size();

  for (int i = 0; i < size; i++) {
    // get closest pixel
    Point2D p = get_pixel_for_photon(map[i]);

    if (p[0] < 1 || p[0] > width || p[1] < 1 || p[1] > height) {
      continue;
    }

    // colour it white
    // TODO: Obviously this isn't always what we want, it does look right for
    // completely transparent stuff like water and clear glass. In order to have
    // photons with proper colours the way materials are defined really needs
    // some work.
    img(p[0], height - p[1] - 1, 0) = 1;
    img(p[0], height - p[1] - 1, 1) = 1;
    img(p[0], height - p[1] - 1, 2) = 1;
  }

  img.savePng(filename);
}

Point2D RayTracer::get_pixel_for_photon(Photon *photon) {
  const double fovy = (height / width) * fov;
  const double tanx = tan(fov * PI / 180 * 0.5);
  const double tany = tan(fovy * PI / 180 * 0.5);

  Point3D P = Point3D(0, 0, eye[2] - 1);
  Vector3D N = Vector3D(0, 0, -1);

  Vector3D dir = eye - photon->p;

  double denom = dir.dot(N);
  double num = (P - photon->p).dot(N);

  if (denom != 0 && num != 0) {
    double t = num / denom;
    Point3D point_on_grid = photon->p + t * dir;

    double X = 0.5 * width * ((point_on_grid[0] - eye[0]) / tanx + 1);
    double Y = 0.5 * height * ((point_on_grid[1] - eye[1]) / tany + 1);

    return Point2D(X, Y);
  }

  return Point2D(-1, -1);
}

