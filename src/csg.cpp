/*
 * CSGNode.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "csg.hpp"
#include "global.hpp"

CSGNode::CSGNode(const std::string& name) :
  SceneNode(name), operation(UNION) {
  // TODO Auto-generated constructor stub

}

CSGNode::~CSGNode() {
  // TODO Auto-generated destructor stub
}

bool CSGNode::get_union(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
    SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node, bool quick) {
  if (!(A_hit || B_hit)) {
    return false;
  }

  if (A_hit && B_hit) {
    if ((A.NEAR - ray.O).length() < (B.NEAR - ray.O).length()) {
      intersection = A;
      node = A_node;
    } else {
      intersection = B;
      node = B_node;
    }
    return true;
  }

  if (A_hit) {
    intersection = A;
    node = A_node;
    return true;
  }

  intersection = B;
  node = B_node;
  return true;
}

bool CSGNode::get_intersection(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
    SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node, bool quick) {
  if (!(A_hit && B_hit)) {
    return false;
  }

  double A_min = (A.NEAR - ray.O).length();
  double B_min = (B.NEAR - ray.O).length();
  double A_max = (A.FAR - ray.O).length();
  double B_max = (B.FAR - ray.O).length();

  Intersection *a, *b;
  a = &A;
  b = &B;
  double *a_min = &A_min, *b_min = &B_min, *a_max = &A_max, *b_max = &B_max;
  SceneNode *a_node = A_node, *b_node = B_node;

  // TODO if B < A => swap...
  if (B_min < A_min) {
    a = &B;
    b = &A;
    a_min = &B_min;
    a_max = &B_max;
    b_min = &A_min;
    b_max = &A_max;

    a_node = B_node;
    b_node = A_node;
  }

  if (*b_min > *a_max) {
    return false;
  }

  node = b_node;
  intersection = *b;
  if (*a_max < *b_max) {
    intersection.FAR = a->FAR;
  }
  return true;

}

bool CSGNode::get_difference(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
    SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node, bool quick) {
  if (!A_hit) {
    return false;
  }

  node = A_node;

  if (!B_hit) {
    intersection = A;
    return true;
  }

  double A_min = (A.NEAR - ray.O).length();
  double B_min = (B.NEAR - ray.O).length();
  double A_max = (A.FAR - ray.O).length();
  double B_max = (B.FAR - ray.O).length();

  if (std::abs(A_max - A_min) < EPSILON && std::abs(B_max - B_min) < EPSILON) {
    if (A_min < B_min) {
      return false;
    } else {
//      std::cerr << "D" << std::endl;
//      std::cerr << "A_min: " << A.NEAR << ", B_min: " << B.NEAR << std::endl;
//      std::cerr << "ray: " << ray.O << std::endl;
      intersection.NEAR = B.NEAR;
      intersection.NORMAL = -B.NORMAL;
      intersection.FAR = A.NEAR;
      intersection.FAR_NORMAL = A.NORMAL;
      intersection.uv = B.uv;
      return true;
    }
  }

  if (B_min < A_min && B_max > A_max) {
    return false;
  }

  if (A_min < B_min) {
    intersection = A;
    if (B_min < A_max) {
      intersection.FAR = B.FAR;
      intersection.FAR_NORMAL = -B.FAR_NORMAL;
    }
    return true;
  }

  if (B_max < A_min) {
    intersection = A;
    return true;
  }

  if (B_max < A_max) {
    intersection = B;
    intersection.NEAR = B.FAR;
    intersection.FAR = A.FAR;
    intersection.NORMAL = -B.FAR_NORMAL;
    intersection.FAR_NORMAL = -A.FAR_NORMAL;
    node = B_node;
    return true;
  }

  std::cerr << "I" << std::endl;
  return false;
}

bool CSGNode::intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick) {

  if (m_children.size() == 0) {
    return false;
  }
  quick = false;

  Intersection local_intersection;
  SceneNode *local_node;
  bool hit = false;
  intersection.NEAR = Point3D(0, 0, DOUBLE_MIN);

  Intersection A, B;
  SceneNode *A_node;
  SceneNode *B_node;
  bool A_hit, B_hit = false;

  // Transform the ray
  ray.transform(m_invtrans);
  ChildList::const_iterator it = m_children.begin();

  //  std::cerr << "0" << std::endl;
  A_hit = (*it)->intersect(ray, A, A_node, quick);

  //  std::cerr << "a" << std::endl;

  if (m_children.size() == 1) {
    //    std::cerr << "b" << std::endl;
    hit = A_hit;
    local_intersection = A;
    local_node = A_node;

  } else {
    //    std::cerr << "bad" << std::endl;
    ++it;
    B_hit = (*it)->intersect(ray, B, B_node, quick);

    if (operation == UNION) {
      hit = get_union(ray, A, B, A_hit, B_hit, A_node, B_node, local_intersection, local_node,
          quick);

    } else if (operation == INTERSECTION) {
      hit = get_intersection(ray, A, B, A_hit, B_hit, A_node, B_node, local_intersection,
          local_node, quick);

    } else {
      hit = get_difference(ray, A, B, A_hit, B_hit, A_node, B_node, local_intersection, local_node,
          quick);

    }
  }

  //  std::cerr << "c" << std::endl;
  if (hit) {
    intersection = local_intersection;
    node = local_node;

    // Un-transform the intersection
    intersection.transform(m_trans);

    intersection.NORMAL = m_invtrans.transpose() * intersection.NORMAL;
  }
  return hit;
}
