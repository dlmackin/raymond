/*
 * CSGNode.h
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#ifndef CSGNODE_H_
#define CSGNODE_H_

#include "scene.hpp"

class CSGNode: public SceneNode {
public:
  CSGNode(const std::string& name);
  virtual ~CSGNode();
  bool intersect(Ray ray, Intersection& intersection, SceneNode*& node, bool quick);

  enum Operation {
    UNION, INTERSECTION, DIFFERENCE
  };

  void set_operation(std::string op) {
    if (op == "difference") {
      operation = DIFFERENCE;
    } else if (op == "intersection") {
      operation = INTERSECTION;
    } else {
      operation = UNION;
    }
  }

private:
  Operation operation;

  bool get_union(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
      SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node,
      bool quick);
  bool get_intersection(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
      SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node,
      bool quick);
  bool get_difference(Ray ray, Intersection& A, Intersection& B, bool A_hit, bool B_hit,
      SceneNode *A_node, SceneNode *B_node, Intersection& intersection, SceneNode*& node,
      bool quick);
};

#endif /* CSGNODE_H_ */
