#pragma once

#include <vector>
#include <string>

struct Vertex{
    float x, y ,z;
};

std::vector<Vertex> parseOBJ();