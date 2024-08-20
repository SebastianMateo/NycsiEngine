#include "NMesh.h"

#include <utility>

#include "glad/glad.h"

NMesh::NMesh(std::vector<NVertex> Vertices, std::vector<unsigned> Indices, std::vector<NTexture> Textures) :
    Vertices(std::move(Vertices)),
    Indices(std::move(Indices)),
    Textures(std::move(Textures))
{
    SetupMesh();
}

void NMesh::Draw(NShader& Shader)
{
    unsigned int DiffuseNr = 1;
    unsigned int SpecularNr = 1;

    // Bind all the textures
    for(unsigned int i = 0; i < Textures.size(); i++)
    {
        
        glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);
        shader.setFloat(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
    
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void NMesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind our VAO
    glBindVertexArray(VAO);

    // Bind our VBO, and copy our vertices for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(NVertex), &Vertices[0], GL_STATIC_DRAW);

    // Bind our EBO, and copy our indices for OpenGL to use
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), &Indices[0], GL_STATIC_DRAW);

    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NVertex), (void*)0);

    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NVertex), (void*)offsetof(NVertex, Normal));

    // Texture Coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(NVertex), (void*)offsetof(NVertex, TexCoord));

    // Unbind it
    glBindVertexArray(0);
}
