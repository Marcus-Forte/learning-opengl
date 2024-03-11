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
    float x, y, z, r = 1, g = 1, b = 1;
    std::sscanf(line.c_str(), "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);

    if (x == 0.0f && y == 0.0f && z == 0.0f) continue;
    retval.push_back({x, y, z, r, g, b});
  }
  return std::move(retval);
}