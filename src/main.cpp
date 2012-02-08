#include <iostream>
#include "scene_lua.hpp"
#include "global.hpp"

extern Settings *settings;

int main(int argc, char** argv)
{ 
  settings = new Settings();
  
  // Parse the command line args
  std::string filename = "scene.lua";
  if (argc >= 2) {
    filename = argv[argc - 1];
  }
  
  int opt;
  while ((opt = getopt (argc, argv, "a:dsp:")) != -1) {
    switch(opt) {
      case 'a':
        if (*optarg == 's') {
          settings->stochastic = true;
        } else {
          settings->samples = std::atoi(optarg);
          if (settings->samples < 1) {
            std::cerr << "samples must be a number >= 1" << std::endl;
            abort();
          }
          
        }
        break;
      case 's':
        settings->stochastic = true;
        break;
      case 'p':
        settings->show_photons = true;
        settings->num_photons = std::atoi(optarg);
        if (settings->num_photons < 1) {
          std::cerr << "photons must be a number >= 1" << std::endl;
          abort();
        }
        break;
      case 'd':
        settings->direct_map = true;
        break;
      case '?':
        break;
      default:
        // ruh-roh
        break;
    }
  }


  if (!run_lua(filename)) {
    std::cerr << "Could not open " << filename << std::endl;
    return 1;
  }
//*/
}
