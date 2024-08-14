#pragma once

class NSquare
{
public:
    NSquare();
    ~NSquare();
    
    void Draw();

private:
    unsigned int VBO, VAO, EBO;
    
    // Geometry
    static constexpr float Vertices[] 
    {
        // positions      // colors            // texture coords
        0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f,        // top right
        0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,        // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,        // bottom left
        -0.5f,0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f         // top left
    };

    static constexpr unsigned int Indices[]
    {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
};
