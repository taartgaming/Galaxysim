#pragma once
#include "../Vertex.hpp"
#include <vector>
#include <string>

namespace ModelLoader {
    struct ModelData {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
    };

    ModelData loadOBJ(const std::string& path);
}