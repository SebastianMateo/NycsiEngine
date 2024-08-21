#include "NTexture.h"

#include <iostream>
#include <windows.h>

#include "stb_image.h"
#include "glad/glad.h"

NTexture::NTexture(const std::string& Path, ETextureType TextureType):
    TextureType(TextureType)
{
    // Texture Binding
    glGenTextures(1, &TextureId); // How many texture we want to generate, and store it in the second argument
    stbi_set_flip_vertically_on_load(true);
    
    // Loading a Texture
    int Width, Height, NrChannels;
    void* Data = stbi_load(Path.c_str(), &Width, &Height, &NrChannels, 0);

    if (Data)
    {
        // Check what we loaded. We need this later
        GLenum Format = GL_RGB;
        if (NrChannels == 1)
            Format = GL_RED;
        else if (NrChannels == 3)
            Format = GL_RGB;
        else if (NrChannels == 4)
            Format = GL_RGBA;

        // Now we can bind the texture
        glBindTexture(GL_TEXTURE_2D, TextureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, Format, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // set the texture wrapping/filtering options (on currently bound texture)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture: " << Path << '\n';
    }
    stbi_image_free(Data);
}

void NTexture::BindAndActivate(unsigned int TextureUnit) const
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, TextureId);
}
