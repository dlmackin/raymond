#ifndef SCENE_HPP
#define SCENE_HPP

#include <list>
#include "algebra.hpp"
#include "primitive.hpp"
#include "material.hpp"
#include "light.hpp"
#include "intersection.hpp"
#include "ray.hpp"

class SceneNode {
public:
  SceneNode(const std::string& name);
  virtual ~SceneNode();

  const Matrix4x4& get_transform() const { return m_trans; }
  const Matrix4x4& get_inverse() const { return m_invtrans; }
  
  void set_transform(const Matrix4x4& m)
  {
    m_trans = m;
    m_invtrans = m.invert();
  }

  void set_transform(const Matrix4x4& m, const Matrix4x4& i)
  {
    m_trans = m;
    m_invtrans = i;
  }

  void add_child(SceneNode* child)
  {
    m_children.push_back(child);
  }

  void remove_child(SceneNode* child)
  {
    m_children.remove(child);
  }

  // Callbacks to be implemented.
  // These will be called from Lua.
  void rotate(char axis, double angle);
  void scale(const Vector3D& amount);
  void translate(const Vector3D& amount);

  // Returns true if and only if this node is a JointNode
  virtual bool is_joint() const;
  
  virtual bool is_geo() const; // DLM
  
  virtual void walk_tree(); // DLM
  
  std::list<SceneNode*> get_children() { return m_children; } // DLM // TODO rm!!
  std::string get_name() { return m_name; }; // DLM
  std::string get_type() { return "scene node"; }; // DLM
  
  virtual bool intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick); // DLM
protected:
  
  // Useful for picking
  int m_id;
  std::string m_name;

  // Transformations
  Matrix4x4 m_trans;
  Matrix4x4 m_invtrans;

  // Hierarchy
  typedef std::list<SceneNode*> ChildList;
  ChildList m_children;
};

class JointNode : public SceneNode {
public:
  JointNode(const std::string& name);
  virtual ~JointNode();

  virtual bool is_joint() const;

  void set_joint_x(double min, double init, double max);
  void set_joint_y(double min, double init, double max);

  struct JointRange {
    double min, init, max;
  };

  
protected:

  JointRange m_joint_x, m_joint_y;
};

class GeometryNode : public SceneNode {
public:
  GeometryNode(const std::string& name,
               Primitive* primitive);
  virtual ~GeometryNode();

  const Material* get_material() const;
  Material* get_material() { return m_material; }

  void set_material(Material* material)
  {
    m_material = material;
  }
  
  // DLM
  virtual bool is_geo() const; // DLM
  bool intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick);
  Colour colour_at(Intersection intersection, const Vector3D* view, const Light *light);
  Colour get_ambient_intensity(Intersection &intersection);

  std::string get_type() { return "geometry node"; }; // DLM
protected:
  Material* m_material;
  Primitive* m_primitive;
};

#endif
