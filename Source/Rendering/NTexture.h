#pragma once
#include <string>

enum class ETextureType
{
    Diffuse,
    Specular,
    Normal,
    Height
};

class NTexture
{
public:
    NTexture(const std::string& Path, ETextureType TextureType);

    ETextureType GetTextureType() const { return TextureType; }
    unsigned int GetTextureId() const { return TextureId; }
    
    void BindAndActivate(unsigned int TextureUnit) const;
    
private:
    unsigned int TextureId;
    ETextureType TextureType;
};
