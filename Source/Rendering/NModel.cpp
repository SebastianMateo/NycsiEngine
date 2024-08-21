#include "NModel.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>

void NModel::Draw(const NShader& Shader)
{
    for (auto& Mesh : Meshes)
    {
        Mesh.Draw(Shader);
    }
}

void NModel::LoadModel(const std::string& Path)
{
    // Read the file with ASSIMP
    Assimp::Importer Importer;
    const aiScene *Scene = Importer.ReadFile(Path, aiProcess_Triangulate | aiProcess_FlipUVs);

    // Check for errors
    if(!Scene || Scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !Scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << Importer.GetErrorString() << std::endl;
        return;
    }
    
    Directory = Path.substr(0, Path.find_last_of('/'));

    // Process ASSIMP Nodes recursively
    ProcessNode(Scene->mRootNode, Scene);
}

void NModel::ProcessNode(const aiNode* Node, const aiScene* Scene)
{
    // Process all node meshes
    for(unsigned int i = 0; i < Node->mNumMeshes; i++)
    {
        aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
        Meshes.push_back(ProcessMesh(mesh, Scene));
    }

    // Then we do the same for the children
    for(unsigned int i = 0; i < Node->mNumChildren; i++)
    {
        ProcessNode(Node->mChildren[i], Scene);
    }
}

NMesh NModel::ProcessMesh(aiMesh* Mesh, const aiScene* Scene)
{
    std::vector<NVertex> Vertices;
    // Data to fill
    std::vector<unsigned int> Indices;
    std::vector<NTexture> Textures;
    
    // Process Vertices
    for(unsigned int i = 0; i < Mesh->mNumVertices; i++)
    {
        NVertex vertex;
         
        // positions
        vertex.Position = { Mesh->mVertices[i].x, Mesh->mVertices[i].y, Mesh->mVertices[i].z };
        
        // normals
        if (Mesh->HasNormals())
        {
            vertex.Normal = {Mesh->mNormals[i].x, Mesh->mNormals[i].y, Mesh->mNormals[i].z };
        }
        // texture coordinates
        if(Mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;

            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TexCoord = { Mesh->mTextureCoords[0][i].x, Mesh->mTextureCoords[0][i].y };

            // tangent
            //vertex.Tangent = {Mesh->mTangents[i].x, Mesh->mTangents[i].y, Mesh->mTangents[i].z };
            
            // bitangent
            //vertex.Bitangent = { Mesh->mBitangents[i].x, Mesh->mBitangents[i].y, Mesh->mBitangents[i].z }; 
        }
        else
            vertex.TexCoord = glm::vec2(0.0f, 0.0f);
    
        Vertices.push_back(vertex);
    }

    // Process indices
    for(unsigned int i = 0; i < Mesh->mNumFaces; i++)
    {
        aiFace face = Mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            Indices.push_back(face.mIndices[j]);        
    }
    
    // Process materials
    aiMaterial* material = Scene->mMaterials[Mesh->mMaterialIndex];    
    // we assume a convention for sampler names in the shaders. 
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN
    // height: texture_heightN
    
    // 1. diffuse maps
    std::vector<NTexture> DiffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
    Textures.insert(Textures.end(), DiffuseMaps.begin(), DiffuseMaps.end());
    
    // 2. specular maps
    std::vector<NTexture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
    Textures.insert(Textures.end(), specularMaps.begin(), specularMaps.end());
    
    // 3. normal maps
    //std::vector<NTexture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
    //Textures.insert(Textures.end(), normalMaps.begin(), normalMaps.end());
    
    // 4. height maps
    //std::vector<NTexture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_height");
    //Textures.insert(Textures.end(), heightMaps.begin(), heightMaps.end());
    
    // return a mesh object created from the extracted mesh data
    return {Vertices, Indices, Textures};
}

std::vector<NTexture> NModel::LoadMaterialTextures(aiMaterial* Mat, aiTextureType Type)
{
    std::vector<NTexture> Textures;
    for(unsigned int i = 0; i < Mat->GetTextureCount(Type); i++)
    {
        aiString str;
        Mat->GetTexture(Type, i, &str);

        // Check if we already loaded this texture
        if (!TexturesLoaded.contains(str.C_Str()))
        {
            TexturesLoaded.emplace(str.C_Str());

            NTexture Texture(Directory + '/' + std::string(str.C_Str()), ConvertTextureType(Type));
            Textures.push_back(Texture);
        }
        
    }
    return Textures;
}

ETextureType NModel::ConvertTextureType(aiTextureType TextureType)
{
    switch (TextureType)
    {
    case aiTextureType_DIFFUSE:
        return ETextureType::Diffuse;
        
    case aiTextureType_SPECULAR:
        return ETextureType::Specular;
        
    case aiTextureType_HEIGHT:
        return ETextureType::Height;
    
    case aiTextureType_NORMALS:
        return ETextureType::Normal;
        
    default: ;
        std::cout << "ERROR::ASSIMP::Invalid texture type" << std::endl;
        return ETextureType::Diffuse;
    }
}
