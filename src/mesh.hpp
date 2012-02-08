#ifndef CS488_MESH_HPP
#define CS488_MESH_HPP

#include <vector>
#include <iosfwd>
#include "primitive.hpp"
#include "algebra.hpp"

// A polygonal mesh.
class Mesh : public Primitive {
public:
  Mesh(const std::vector<Point3D>& verts,
       const std::vector< std::vector<int> >& faces);
  Mesh(const std::vector<Point3D>& verts,
         const std::vector< std::vector<int> >& faces,
         const std::vector<Point2D>& uv);

  typedef std::vector<int> Face;
  
private:
  std::vector<Point3D> m_verts;
  std::vector<Face> m_faces;
  std::vector<Point2D> m_uv;

  bool has_uv;

  friend std::ostream& operator<<(std::ostream& out, const Mesh& mesh);
  
  // DLM
  virtual bool intersect(const Ray& ray, Intersection& intersection, bool quick); // DLM
  bool intersect_face(const Ray& ray, const Face face, Intersection& intersection);
  bool intersect_triangle(Point2D P, Point2D V0, Point2D V1, Point2D V2);
  
  void init_boundary();
  bool intersect_boundary(const Ray& ray);
  
  NonhierSphere * boundary;
};

#endif
