#include "NTexture.h"

#include <iostream>
#include <windows.h>

#include "stb_image.h"
#include "glad/glad.h"

NTexture::NTexture(const std::string& Path, const unsigned int TextureUnit, const bool bHasAlpha)
{
    // Save the texture unity to bind and activate later
    this->TextureUnit = TextureUnit;
    
    // Texture Binding
    glGenTextures(1, &TextureId); // How many texture we want to generate, and store it in the second argument
    glBindTexture(GL_TEXTURE_2D, TextureId);

    // set the texture wrapping/filtering options (on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Loading a Texture
    int Width, Height, NrChannels;
    stbi_set_flip_vertically_on_load(true);
    void* Data = stbi_load(Path.c_str(), &Width, &Height, &NrChannels, 0);

    if (Data)
    {
        const unsigned int Format = bHasAlpha ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0,Format,GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << Path << '\n';
    }
    stbi_image_free(Data);
}

void NTexture::BindAndActivate() const
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, TextureId);
}
