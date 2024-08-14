#pragma once
#include <string>

class NTexture
{
public:
    NTexture(const std::string& Path, unsigned int TextureUnit, bool bHasAlpha = false);

    void BindAndActivate() const;
private:
    unsigned int TextureId;
    unsigned int TextureUnit;
};
