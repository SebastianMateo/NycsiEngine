#pragma once
#include <string>
#include <unordered_set>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "NMesh.h"
#include "NTexture.h"

class NModel
{
public:
    NModel(const std::string& Path)
    {
        LoadModel(Path);
    }
    
    void Draw(const NShader &Shader);
private:
    // model data
    std::vector<NMesh> Meshes;
    std::string Directory;

    // To not repeat loading the same texture
    std::pmr::unordered_set<std::string> TexturesLoaded;
    
    void LoadModel(const std::string& Path);
    
    void ProcessNode(const aiNode *Node, const aiScene *Scene);
    NMesh ProcessMesh(aiMesh *Mesh, const aiScene *Scene);
    std::vector<NTexture> LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type);

    ETextureType ConvertTextureType(aiTextureType TextureType);
};
