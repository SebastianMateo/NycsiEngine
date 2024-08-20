#pragma once
#include <string>

enum class ETextureType
{
    Diffuse,
    Specular
};

class NTexture
{
public:
    NTexture(const std::string& Path, unsigned int TextureUnit, ETextureType TextureType, bool bHasAlpha = false);

    ETextureType GetTextureType() const { return TextureType; }
    void BindAndActivate() const;
    
private:
    unsigned int TextureId;
    unsigned int TextureUnit;
    ETextureType TextureType;
};
