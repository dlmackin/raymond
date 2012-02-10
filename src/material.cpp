/*
 * material.cpp
 *
 *   CS 488: Final Project
 *   Fall 2011, University of Waterloo
 *   Diana Mackinnon (dlmackin, 20280688)
 */

#include "material.hpp"

Material::~Material() {
}
Colour Material::colour_at(const Intersection& intersection, const Vector3D* view,
    const Light *light) {
  return Colour(1); // TODO placeholder - should really just be abstract.
}

PhongMaterial::PhongMaterial(const Colour& kd, const Colour& ks, double shininess) :
  m_kd(kd), m_ks(ks), m_shininess(shininess) {
}

PhongMaterial::~PhongMaterial() {
}

Colour PhongMaterial::get_kd() {
  return m_kd;
}

Colour PhongMaterial::colour_at(const Intersection& intersection, const Vector3D* view,
    const Light *light) {
  double d_light_contrib = 1, s_light_contrib = 1;
  Colour diffuse(0, 0, 0), specular(0, 0, 0), l(0, 0, 0);
  
  Vector3D normal = get_normal(intersection);

  // calculate light direction
  Vector3D light_dir = light->position - intersection.NEAR;
  double dist = light_dir.length();
  light_dir.normalize();
  
  // calc attenuation
  double attenuation = 1 / (light->falloff[0] + (light->falloff[1] * dist) + (light->falloff[2]
      * dist * dist));
  
  // dot w/ normal
  double light_dot_normal = std::max((double) 0, light_dir.dot(normal));

  // Diffuse part
  diffuse = Colour(get_kd(intersection) * light_dot_normal);
  
  // Specular
  Vector3D r = -light_dir + 2 * (light_dot_normal) * normal;
  
  if (view == NULL || pow((r.dot(*view)), m_shininess) < 0) {
    specular = Colour(0, 0, 0);
  } else {
    specular = Colour(m_ks * pow((r.dot(*view)), m_shininess));
  }
  
  return attenuation * light->colour * (diffuse + specular);
}

RMaterial::RMaterial(const Colour & kd, const Colour & ks, double shininess, double reflection,
    double refraction, double refractive_index) :
  PhongMaterial(kd, ks, shininess), reflection(reflection), refraction(refraction),
      refractive_index(refractive_index) {
}

