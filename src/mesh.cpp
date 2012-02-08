#include "mesh.hpp"
#include "global.hpp"

#include <iostream>

bool print = true;

// TODO this doesn't go here
inline Vector3D operator *(const Point3D& a, const Point3D& b) {
  return Vector3D(a[0] * b[0], a[1] * b[1], a[2] * b[2]);
}

double det(double a, double b, double c, double d) {
  return (a * d) - (b * c);
}

Mesh::Mesh(const std::vector<Point3D>& verts, const std::vector<std::vector<int> >& faces) :
  m_verts(verts), m_faces(faces), has_uv(false) {
  init_boundary();
}

Mesh::Mesh(const std::vector<Point3D>& verts, const std::vector<std::vector<int> >& faces,
    const std::vector<Point2D>& uv) :
  m_verts(verts), m_faces(faces), m_uv(uv), has_uv(true) {
  init_boundary();
}

void Mesh::init_boundary() {
  // TODO
  int x = 0, y = 1, z = 2;
  
  Point3D min(DOUBLE_MAX, DOUBLE_MAX, DOUBLE_MAX);
  Point3D max(DOUBLE_MIN, DOUBLE_MIN, DOUBLE_MIN);
  
  Point3D p;
  
  for (std::vector<Point3D>::const_iterator I = m_verts.begin(); I != m_verts.end(); ++I) {
    p = (*I);
    
    min[x] = std::min(min[x], p[x]);
    min[y] = std::min(min[y], p[y]);
    min[z] = std::min(min[z], p[z]);
    
    max[x] = std::max(max[x], p[x]);
    max[y] = std::max(max[y], p[y]);
    max[z] = std::max(max[z], p[z]);
  }

  Point3D pos = Point3D((max[x] + min[x]) / 2, (max[y] + min[y]) / 2, (max[z] + min[z]) / 2);
  double radius = std::abs((max - pos).length());
  
  std::cerr << "bounding sphere: " << pos << ", " << radius << std::endl;
  
  this->boundary = new NonhierSphere(pos, radius);
}

bool Mesh::intersect_boundary(const Ray& ray) {
  // TODO
  Intersection tmp;
  return boundary->intersect(ray, tmp, true);
}

bool Mesh::intersect(const Ray& ray, Intersection& intersection, bool quick) {
  if (m_faces.size() > 1 && false) {
    return this->boundary->intersect(ray, intersection, quick);
  }
  // First check ray against bounding volume.
  if (m_faces.size() > 1 && !intersect_boundary(ray)) {
    return false;
  }
  //  */
  int i = 0;
  Intersection tmp;
  bool any = false;
  
  for (std::vector<Face>::const_iterator I = m_faces.begin(); I != m_faces.end(); ++I) {
    //  if (i < 10) { i++; continue; }
    // if (i > 1) break;

    //    std::cerr << "ray : " << ray.O << " , " << ray.D << std::endl;
    if (intersect_face(ray, (Face) (*I), tmp)) {
      if (quick)
        return true;
      
      if (!any) {
        intersection = Intersection(tmp);
        any = true;
        if (has_uv) {
          // TODO calculate UV coordinates


        }

      } else {
        // pick closest
        if ((tmp.NEAR - ray.O).length() < (intersection.NEAR - ray.O).length()) {
          intersection = Intersection(tmp);
          if (has_uv) {
            // calculate UV coordinates


          }
        }
      }
    }
    i++;
  }

  if (any) {
    return true;
  }
  return false;
}

// DLM
bool Mesh::intersect_face(const Ray& ray, const Face face, Intersection& intersection) {
  Point3D V0 = m_verts[face[0]], V1 = m_verts[face[1]], V2 = m_verts[face[2]];
  
  // Intersect the plane
  Vector3D N = (V1 - V0).cross(V2 - V1);
  //  N.normalize();

  double N_dot_dir = N.dot(ray.D);
  if (N_dot_dir == 0)
    return false;

  double N_dot_eye = N[0] * ray.O[0] + N[1] * ray.O[1] + N[2] * ray.O[2];
  
  double d = (Vector3D(Point3D(0, 0, 0) - V0)).dot(N);
  
  double t = -(d + N_dot_eye) / N_dot_dir;
  
  if (t <= EPSILON) {
    return false;
  }
  // std::cerr << "B " << std::endl;
  intersection.NEAR = ray.O + (t * ray.D);
  intersection.NORMAL = N;

  //  std::cerr << "ray : " << ray.O << " , " << ray.D << std::endl;
  //  std::cerr << "t : " << t << std::endl;

  if ((intersection.NEAR - ray.O).length() < EPSILON)
    return false;

  // Intersect the actual polygon

  // 1 - determine the dominant axis
  int x, y, z;
  double dom_term = std::max(std::abs(N[0]), std::max(std::abs(N[1]), std::abs(N[2])));
  if (dom_term == std::abs(N[0])) {
    x = 1;
    y = 2;
    z = 0;
  } else if (dom_term == std::abs(N[1])) {
    x = 0;
    y = 2;
    z = 1;
  } else {
    x = 0;
    y = 1;
    z = 2;
  }

  Point2D P(intersection.NEAR[x], intersection.NEAR[y]);
  Vector3D cross = (V1 - V0).cross(V2 - V1);
  
  double orientation = ((V1[x] - V0[x]) * (V2[y] - V1[y])) - ((V1[y] - V0[y]) * (V2[x] - V1[x]));
  
  if (face.size() == 3) {
    return intersect_triangle(P, Point2D(V0[x], V0[y]), Point2D(V1[x], V1[y]),
        Point2D(V2[x], V2[y]));
  }

  for (unsigned int i = 0; i < face.size(); i++) {
    Point2D v0(m_verts[face[i]][x], m_verts[face[i]][y]);
    Point2D
        v1(m_verts[face[(i + 1) % (face.size())]][x], m_verts[face[(i + 1) % (face.size())]][y]);
    
    Point2D norm(v1[1] - v0[1], v0[0] - v1[0]);
    
    double dist = (P[0] - v0[0]) * (v1[1] - v0[1]) - (P[1] - v0[1]) * (v1[0] - v0[0]);
    
    // vertex order
    if (orientation > 0) {
      if (dist > 0)
        return false;
    } else {
      if (dist < 0)
        return false;
    }
  }

  //  std::cerr << "!!! near : " << intersection.NEAR << std::endl;
  return true;
}

bool Mesh::intersect_triangle(Point2D P, Point2D V0, Point2D V1, Point2D V2) {
  double u0 = P[0] - V0[0];
  double u1 = V1[0] - V0[0];
  double u2 = V2[0] - V0[0];
  
  double v0 = P[1] - V0[1];
  double v1 = V1[1] - V0[1];
  double v2 = V2[1] - V0[1];
  
  double alpha = det(u0, u2, v0, v2) / det(u1, u2, v1, v2);
  double beta = det(u1, u0, v1, v0) / det(u1, u2, v1, v2);
  
  if (alpha < 0 || beta < 0 || alpha + beta > 1)
    return false;

  return true;
}

std::ostream& operator<<(std::ostream& out, const Mesh& mesh) {
  std::cerr << "mesh({";
  for (std::vector<Point3D>::const_iterator I = mesh.m_verts.begin(); I != mesh.m_verts.end(); ++I) {
    if (I != mesh.m_verts.begin())
      std::cerr << ",\n      ";
    std::cerr << *I;
  }
  std::cerr << "},\n\n     {";
  
  for (std::vector<Mesh::Face>::const_iterator I = mesh.m_faces.begin(); I != mesh.m_faces.end(); ++I) {
    if (I != mesh.m_faces.begin())
      std::cerr << ",\n      ";
    std::cerr << "[";
    for (Mesh::Face::const_iterator J = I->begin(); J != I->end(); ++J) {
      if (J != I->begin())
        std::cerr << ", ";
      std::cerr << *J;
    }
    std::cerr << "]";
  }
  std::cerr << "});" << std::endl;
  return out;
}

