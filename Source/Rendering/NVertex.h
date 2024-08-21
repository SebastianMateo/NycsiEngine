#pragma once
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct NVertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};
