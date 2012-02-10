/*
 * primitive.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "primitive.hpp"
#include "global.hpp"

// TODO Split each primitive into its own file.
// TODO Merge some of the Cube and NonHierBox methods (if possible)
Primitive::~Primitive() {
}

Cube::~Cube() {
}

// TODO clean this up like whoa.
bool Cube::intersect(const Ray& ray, Intersection& intersection, bool quick) {
  Point3D B1 = Point3D(0, 0, 0);
  Point3D Bh = Point3D(1, 1, 1);
  
  double t_near = DOUBLE_MAX, t_far = 0;

  Point3D local_intersection;
  bool any_intersection = false;
  double d;
  Point3D p0;
  Vector3D n;
  
  for (int i = 0; i < 3; i++) {
    p0 = Point3D(0, 0, 0);
    n = Vector3D(0, 0, 0);
    
    p0[i] = Bh[i];
    n[i] = 1;
    
    if (!intersect_plane(ray, p0, n, d)) {
      continue;
    }

    if (d <= 0)
      continue;

    local_intersection = ray.O + (d * ray.D);
    if ((local_intersection - ray.O).length() < EPSILON)
      continue;
    
    if (in_box(local_intersection, B1, Bh, i)) {
      if (quick)
        return true;

      if (d > t_far) {
        t_far = d;
        intersection.FAR = local_intersection;
        intersection.FAR_NORMAL = Vector3D(0, 0, 0);
        intersection.FAR_NORMAL[i] = 1;
      }
      if (d < t_near) {
        t_near = d;
        intersection.NEAR = local_intersection;
        intersection.NORMAL = Vector3D(0, 0, 0);
        intersection.NORMAL[i] = 1;
        any_intersection = true;

        Point2D uv = Point2D(0,0);
        int tmp = 0;
        for (int j=0; j<3; j++) {
          if (j != i) {


            if (j == 1) {
              uv[tmp] = 1 - intersection.NEAR[j];
            } else {
              uv[tmp] = intersection.NEAR[j];
            }

            tmp++;
          }
        }
        intersection.uv = uv;
      }

    }
  }

  for (int i = 0; i < 3; i++) {
    p0 = Point3D(0, 0, 0);
    n = Vector3D(0, 0, 0);
    
    p0[i] = B1[i];
    n[i] = -1;
    
    if (!intersect_plane(ray, p0, n, d))
      continue; // TODO maybe?

    if (d < 0)
      continue;

    local_intersection = ray.O + (d * ray.D);
    if ((local_intersection - ray.O).length() < EPSILON)
      continue;
    
    if (in_box(local_intersection, B1, Bh, i)) {
      if (quick)
        return true;

      if (d > t_far) {
        t_far = d;
        intersection.FAR = local_intersection;
        intersection.FAR_NORMAL = Vector3D(0, 0, 0);
        intersection.FAR_NORMAL[i] = -1;
      }
      if (d < t_near) {
        t_near = d;
        intersection.NEAR = local_intersection;
        intersection.NORMAL = Vector3D(0, 0, 0);
        intersection.NORMAL[i] = -1;
        any_intersection = true;

        Point2D uv = Point2D(0,0);
        int tmp = 0;
        for (int j=0; j<3; j++) {
          if (j == 1) {
            uv[tmp] = 1 - intersection.NEAR[j];
          } else {
            uv[tmp] = intersection.NEAR[j];
          }
        }
        intersection.uv = uv;
      }

    }
  }

  if (!any_intersection)
    return false;

  if ((intersection.NEAR - ray.O).length() < EPSILON) {
    return false;
  }

  if (intersection.NORMAL.length() != 1)
    std::cerr << intersection.NORMAL << std::endl;
  
  return true;
}

double Cube::intersect_plane(const Ray& ray, Point3D& p0, Vector3D& n, double& d) {
  double num = (p0 - ray.O).dot(n);
  double denom = ray.D.dot(n);
  
  if (denom == 0) {
    return false;
  }

  d = num / denom;
  return true;
}

bool Cube::in_box(Point3D point, Point3D box_min, Point3D box_max, int plane) {
  for (int i = 0; i < 3; i++) {
    if (i == plane)
      continue;
    if (point[i] < box_min[i] || point[i] > box_max[i])
      return false;
  }
  return true;
}

// TODO move out of here
Point3D Cube::closest(Point3D origin, Point3D p0, Point3D p1) {
  if ((p0 - origin).length() < (p1 - origin).length()) {
    return p0;
  }
  return p1;
}

// TODO check how this works for corners/edges. It should be ok but maybe not
// super consistent?
Vector3D Cube::normal(Intersection& point) {
  Vector3D ret(0, 0, 0);
  
  Vector3D diff1 = point.NEAR - Point3D(0, 0, 0);
  Vector3D diff2 = point.NEAR - (Point3D(0, 0, 0) + Vector3D(1, 1, 1));
  
  for (int i = 0; i < 3; i++) {
    if (std::abs(diff1[i]) < EPSILON) {
      ret[i] = -1;
      return ret;
    } else if (std::abs(diff2[i]) < EPSILON) {
      ret[i] = 1;
      return ret;
    }
  }
  point.NORMAL = ret;
  return ret;
}

NonhierSphere::~NonhierSphere() {
}

bool NonhierSphere::intersect(const Ray& ray, Intersection& intersection, bool quick) {
  // TODO this is pretty dirty...
  Vector3D v = m_pos - ray.O;
  if (ray.D.dot(v) <= 0) {
    return false;
  }

  double A = ray.D.dot(ray.D);
  double B = 2 * ray.D.dot(ray.O - m_pos);
  double C = (ray.O - m_pos).dot(ray.O - m_pos) - (m_radius * m_radius);
  
  double d = (B * B) - (4 * A * C);
  
  if (d < 0) {
    return false;
  }

  // HIT! find intersection point & return
  if (quick)
    return true;

  double t = (-B - sqrt(d)) / (2 * A);
  if (!t > 0) {
    t = (-B + sqrt(d)) / (2 * A);
  }
  intersection.NEAR = ray.O + t * ray.D;
  
  // get normal too
  normal(intersection);
  
  return true;
}

Vector3D NonhierSphere::normal(Intersection& point) {
  point.NORMAL = (1 / m_radius) * (point.NEAR - m_pos);
  return (1 / m_radius) * (point.NEAR - m_pos);
}

NonhierBox::~NonhierBox() {
}

// TODO clean this up like whoa.
bool NonhierBox::intersect(const Ray& ray, Intersection& intersection, bool quick) {
  Point3D B1 = Point3D(m_pos[0], m_pos[1], m_pos[2]);
  Point3D Bh = Point3D(m_pos[0] + m_size, m_pos[1] + m_size, m_pos[2] + m_size);
  
  Point3D local_intersection;
  bool any_intersection = false;
  double d;
  Point3D p0;
  Vector3D n;
  
  for (int i = 0; i < 3; i++) {
    p0 = Point3D(0, 0, 0);
    n = Vector3D(0, 0, 0);
    
    p0[i] = Bh[i];
    n[i] = 1;
    
    if (!intersect_plane(ray, p0, n, d)) {
      continue;
    }

    if (d < 0)
      continue;

    local_intersection = ray.O + (d * ray.D);
    if ((local_intersection - ray.O).length() < EPSILON_TEMP)
      continue;
    
    if (in_box(local_intersection, B1, Bh, i)) {
      if (quick)
        return true;
      
      if (!any_intersection) {
        intersection.NEAR = local_intersection;
        intersection.NORMAL = Vector3D(0, 0, 0);
        intersection.NORMAL[i] = 1;
        any_intersection = true;
      } else {
        if ((local_intersection - ray.O).length() < (intersection.NEAR - ray.O).length()) {
          intersection.NEAR = local_intersection;
          intersection.NORMAL = Vector3D(0, 0, 0);
          intersection.NORMAL[i] = 1;
        }
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    p0 = Point3D(0, 0, 0);
    n = Vector3D(0, 0, 0);
    
    p0[i] = B1[i];
    n[i] = -1;
    
    if (!intersect_plane(ray, p0, n, d))
      continue; // TODO maybe?

    if (d < 0)
      continue;

    local_intersection = ray.O + (d * ray.D);
    if ((local_intersection - ray.O).length() < EPSILON_TEMP)
      continue;
    
    if (in_box(local_intersection, B1, Bh, i)) {
      if (quick)
        return true;
      
      if (!any_intersection) {
        intersection.NEAR = local_intersection;
        intersection.NORMAL = Vector3D(0, 0, 0);
        intersection.NORMAL[i] = -1;
        any_intersection = true;
      } else {
        if ((local_intersection - ray.O).length() < (intersection.NEAR - ray.O).length()) {
          intersection.NEAR = local_intersection;
          intersection.NORMAL = Vector3D(0, 0, 0);
          intersection.NORMAL[i] = -1;
        }
      }
    }
  }

  if (!any_intersection)
    return false;

  if ((intersection.NEAR - ray.O).length() < EPSILON)
    return false;

  return true;
}

double NonhierBox::intersect_plane(const Ray& ray, Point3D& p0, Vector3D& n, double& d) {
  double num = (p0 - ray.O).dot(n);
  double denom = ray.D.dot(n);
  
  if (denom == 0)
    return false;
  d = num / denom;
  
  return true;
}

bool NonhierBox::in_box(Point3D point, Point3D box_min, Point3D box_max, int plane) {
  for (int i = 0; i < 3; i++) {
    if (i == plane)
      continue;
    if (point[i] < box_min[i] || point[i] > box_max[i])
      return false;
  }
  return true;
}

Point3D NonhierBox::closest(Point3D origin, Point3D p0, Point3D p1) {
  if ((p0 - origin).length() < (p1 - origin).length()) {
    return p0;
  }
  return p1;
}

// TODO check how this works for edges and corners.
Vector3D NonhierBox::normal(Intersection& point) {
  Vector3D ret(0, 0, 0);
  
  Vector3D diff1 = point.NEAR - m_pos;
  Vector3D diff2 = point.NEAR - (m_pos + Vector3D(m_size, m_size, m_size));
  
  for (int i = 0; i < 3; i++) {
    if (std::abs(diff1[i]) < EPSILON) {
      ret[i] = -1;
    } else if (std::abs(diff2[i]) < EPSILON) {
      ret[i] = 1;
    }
  }
  point.NORMAL = ret;
  return ret;
}

