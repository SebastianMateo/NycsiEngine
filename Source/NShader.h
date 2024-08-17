#pragma once

#include <string>

#include "glm/fwd.hpp"

class NShader final
{
public:
    // Constructor reads and builds the shader
    NShader(const std::string& VertexPath, const std::string& FragmentPath);
    ~NShader();
    
    // Use/activate the shader
    void Use() const;
    
    // Utility uniform functions
    void SetBool(const std::string &Name, bool Value) const;
    void SetInt(const std::string &Name, int Value) const;
    void SetFloat(const std::string &Name, float Value) const;
    void SetMat4(const std::string &Name, const glm::mat4 &Mat) const;
    void SetVec3(const std::string& Name, float X1, float X2, float X3) const;
    void SetVec3(const std::string& Name, const glm::vec3 &Vec) const;

    unsigned int GetId() const { return ShaderProgramId; }
private:
    unsigned int ShaderProgramId;

    unsigned int CompileShader(const std::string& Path, const int ShaderType );
    static std::string ReadShaderFile(const char* ShaderFile);
    void CheckCompileErrors(unsigned Shader, const std::string& Type);
};
