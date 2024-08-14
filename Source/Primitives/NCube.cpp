#include "NCube.h"

#include "glad/glad.h"

NCube::NCube()
{
    // Generate the Vertex Buffer Object and Vertex Array Object 
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // 1. bind Vertex Array Object.
    glBindVertexArray(VAO);

    // 2. Copy VBO and EBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    // 3. then set our vertex attributes pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(2);
}

void NCube::Draw()
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

NCube::~NCube()
{
    // De-allocate all resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}