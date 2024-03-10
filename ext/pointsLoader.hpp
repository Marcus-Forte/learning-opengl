#pragma once

#include <fstream>

#include "layouts/pointAttribute.hpp"

std::vector<GLPointData> loadFile(const std::string& path) {
  std::fstream file(path);

  if (!file.is_open()) throw std::runtime_error("Unable to load cloud file.");

  // Header
  //   x,y,z,r,g,b
  std::string line;
  std::vector<GLPointData> retval;

  while (std::getline(file, line)) {
    float x, y, z, r =0, g =0, b =0;
    std::sscanf(line.c_str(), "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

    // r = r / 255.0;
    // g = g / 255.0;
    // b = b / 255.0;
    if (x == 0.0f && y == 0.0f && z == 0.0f) continue;
    retval.push_back({x, y, z, r, g, b});
    // std::cout << x << "," << y << "," << z << " : " << r << "," << g << "," << b << std::endl;
    // exit(0);
  }
  return std::move(retval);
}