#ifndef CS488_BLAH_HPP
#define CS488_BLAH_HPP

struct Settings {

  bool stochastic;
  int samples;
  bool simple;
  bool show_photons;
  int num_photons;
  bool direct_map;

  Settings() {
    this->stochastic = false;
    this->samples = 1;
    this->simple = false;
    this->show_photons = false;
    this->num_photons = 1000;
    this->direct_map = false;
  }
};

#endif // CS488_SETTINGS_HPP
