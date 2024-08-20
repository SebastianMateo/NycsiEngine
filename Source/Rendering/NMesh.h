#pragma once
#include <vector>

#include "NVertex.h"
#include "glm/vec3.hpp"


class NShader;
class NTexture;

class NMesh
{
public:
    NMesh(std::vector<NVertex> Vertices, std::vector<unsigned int> Indices, std::vector<NTexture> Textures);
    void Draw(NShader &Shader);
    
private:
    std::vector<NVertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<NTexture> Textures;

    void SetupMesh();
    
    unsigned int VAO, VBO, EBO;
};
