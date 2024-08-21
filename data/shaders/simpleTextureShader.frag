#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

struct Material
{
    sampler2D diffuse1;
    sampler2D diffuse2;
    sampler2D diffuse3;
    sampler2D specular1;
    sampler2D specular2;
    sampler2D specular3;
    float shininess;
};

uniform Material material;

void main()
{    
    FragColor = texture(material.diffuse1, TexCoord);
}