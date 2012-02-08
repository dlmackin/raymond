#include "scene.hpp"

#include "global.hpp"

#include <iostream>

SceneNode::SceneNode(const std::string& name) :
  m_name(name), m_trans() {
  m_invtrans = m_trans.invert();
}

SceneNode::~SceneNode() {
}
void SceneNode::rotate(char axis, double angle) {
  // Fill me in
  Matrix4x4 r;

  double theta = angle * (M_PI / 180.0);

  if (axis == 'x') {

    r = Matrix4x4(Vector4D(1, 0, 0, 0), Vector4D(0, cos(theta), -sin(theta), 0), Vector4D(0, sin(
        theta), cos(theta), 0), Vector4D(0, 0, 0, 1));

  } else if (axis == 'y') {

    r = Matrix4x4(Vector4D(cos(theta), 0, sin(theta), 0), Vector4D(0, 1, 0, 0), Vector4D(
        -sin(theta), 0, cos(theta), 0), Vector4D(0, 0, 0, 1));

  } else if (axis == 'z') {

    r = Matrix4x4(Vector4D(cos(theta), -sin(theta), 0, 0), Vector4D(sin(theta), cos(theta), 0, 0),
        Vector4D(0, 0, 1, 0), Vector4D(0, 0, 0, 1));

  } else {
    std::cerr << "Invalid input to rotation(double angle, char axis). "
        << "Axis should be one of \'x\', \'y\', \'z\'." << std::endl;
  }

  m_trans = m_trans * r;
  m_invtrans = m_trans.invert(); // TODO move to a method and/or do this better
}

void SceneNode::scale(const Vector3D& amount) {
  // Fill me in
  Matrix4x4 s;

  s = Matrix4x4(Vector4D(amount[0], 0, 0, 0), Vector4D(0, amount[1], 0, 0), Vector4D(0, 0,
      amount[2], 0), Vector4D(0, 0, 0, 1));

  m_trans = m_trans * s;
  m_invtrans = m_trans.invert(); // TODO move to a method and/or do this better
}

void SceneNode::translate(const Vector3D& amount) {
  // Fill me in
  Matrix4x4 t;

  t = Matrix4x4(Vector4D(1, 0, 0, amount[0]), Vector4D(0, 1, 0, amount[1]), Vector4D(0, 0, 1,
      amount[2]), Vector4D(0, 0, 0, 1));

  m_trans = m_trans * t;
  m_invtrans = m_trans.invert(); // TODO move to a method and/or do this better
}

bool SceneNode::is_joint() const {
  return false;
}

bool SceneNode::is_geo() const {
  return false;
}

bool SceneNode::intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick) {
  
  Intersection local_intersection;
  SceneNode *local_node;
  bool hit = false;
  intersection.NEAR = Point3D(0, 0, DOUBLE_MIN);
  
  // Transform the ray
  ray.transform(m_invtrans);
  for (ChildList::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
    
    if ((*it)->intersect(ray, local_intersection, local_node, quick)) {
      
      if (quick) {
        node = local_node;
        return true;
      }
      
      if (intersection.NEAR[2] == DOUBLE_MIN || (local_intersection.NEAR - ray.O).length()
          < (intersection.NEAR - ray.O).length()) {
        
        hit = true;
        intersection = local_intersection;
        node = local_node;
      }
    }
    
  }

  // Un-transform the intersection
  intersection.transform(m_trans);
  
  intersection.NORMAL = m_invtrans.transpose() * intersection.NORMAL;
  
  if (hit) {
    //    std::cerr << "intersection: " << node->get_name() << " at " << intersection.NEAR << std::endl;
  }
  return hit;
}

void SceneNode::walk_tree() {
  for (ChildList::const_iterator it = m_children.begin(); it != m_children.end(); ++it) {
    (*it)->walk_tree();
  }
}

JointNode::JointNode(const std::string& name) :
  SceneNode(name) {
}

JointNode::~JointNode() {
}

bool JointNode::is_joint() const {
  return true;
}

void JointNode::set_joint_x(double min, double init, double max) {
  m_joint_x.min = min;
  m_joint_x.init = init;
  m_joint_x.max = max;
}

void JointNode::set_joint_y(double min, double init, double max) {
  m_joint_y.min = min;
  m_joint_y.init = init;
  m_joint_y.max = max;
}

GeometryNode::GeometryNode(const std::string& name, Primitive* primitive) :
  SceneNode(name), m_primitive(primitive) {
}

GeometryNode::~GeometryNode() {
}

bool GeometryNode::is_geo() const {
  return true;
}

bool GeometryNode::intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick) {
  Ray orig = ray;
  
  // Transform the ray
  ray.transform(m_invtrans);
  
  // get the intersection
  bool ret = m_primitive->intersect(ray, intersection, quick);
  
  node = this;

  if (quick)
    return ret;

  // transform the intersection
  intersection.transform(m_trans);
  
  intersection.NORMAL = m_invtrans.transpose() * intersection.NORMAL;
  
  return ret;
}

Colour GeometryNode::colour_at(Intersection intersection, const Vector3D* view, const Light *light) {
  return m_material->colour_at(intersection, view, light);
}

Colour GeometryNode::get_ambient_intensity(Intersection &intersection) {
  intersection.transform(m_invtrans);

  Colour ret = ((PhongMaterial *) m_material)->get_kd(intersection);
  intersection.transform(m_trans);

  return ret;
}

