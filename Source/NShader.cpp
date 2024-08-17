#include "NShader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include "glm/glm.hpp"

NShader::NShader(const std::string& VertexPath, const std::string& FragmentPath)
{
    // 1. Create both Vertex and Fragment Shaders
    const unsigned Vertex = CompileShader(VertexPath, GL_VERTEX_SHADER);
    const unsigned Fragment = CompileShader(FragmentPath, GL_FRAGMENT_SHADER);
    
    // 2. Create the Shader pogram
    ShaderProgramId = glCreateProgram();
    glAttachShader(ShaderProgramId, Vertex);
    glAttachShader(ShaderProgramId, Fragment);
    glLinkProgram(ShaderProgramId);
    CheckCompileErrors(ShaderProgramId, "PROGRAM");

    // 3. Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(Vertex);
    glDeleteShader(Fragment);
}

NShader::~NShader()
{
    glDeleteProgram(ShaderProgramId);
}

void NShader::Use() const
{
    glUseProgram(ShaderProgramId);
}

void NShader::SetBool(const std::string &Name, const bool Value) const
{         
    glUniform1i(glGetUniformLocation(ShaderProgramId, Name.c_str()), static_cast<int>(Value)); 
}

void NShader::SetInt(const std::string &Name, const int Value) const
{ 
    glUniform1i(glGetUniformLocation(ShaderProgramId, Name.c_str()), Value); 
}

// ------------------------------------------------------------------------
void NShader::SetFloat(const std::string &Name, const float Value) const
{ 
    glUniform1f(glGetUniformLocation(ShaderProgramId, Name.c_str()), Value); 
}

void NShader::SetMat4(const std::string& Name, const glm::mat4& Mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ShaderProgramId, Name.c_str()), 1, GL_FALSE, &Mat[0][0]);
}

void NShader::SetVec3(const std::string& Name, const float X1, const float X2, const float X3) const
{
    glUniform3f(glGetUniformLocation(ShaderProgramId, Name.c_str()), X1, X2, X3); 
}

void NShader::SetVec3(const std::string& Name, const glm::vec3& Vec) const
{
    glUniform3fv(glGetUniformLocation(ShaderProgramId, Name.c_str()), 1, &Vec[0]); 
}

unsigned int NShader::CompileShader(const std::string& Path, const int ShaderType)
{
    //1. Open the Files and read the Shader Codes
    const std::string ShaderCode = ReadShaderFile(Path.c_str());
    if (!ShaderCode.empty())
    {
        const char* ShaderCodeStr = ShaderCode.c_str();

        // 2. compile shaders
        const unsigned int Shader = glCreateShader(ShaderType);
        glShaderSource(Shader, 1, &ShaderCodeStr, nullptr);
        glCompileShader(Shader);

        CheckCompileErrors(Shader, (ShaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT");
    
        return Shader;
    }
    else
    {
        std::cout << "FAILED TO READ SHADER " << Path << '\n';
    }
    return 0;
}

std::string NShader::ReadShaderFile(const char *ShaderFile)
{
    // no feedback is provided for stream errors / exceptions.
    std::ifstream File (ShaderFile);
    if (!File) return {};

    File.ignore(std::numeric_limits<std::streamsize>::max());
    const auto Size = File.gcount();

    if (Size > 0x10000) // 64KiB sanity check for shaders:
        return {};

    File.clear();
    File.seekg(0, std::ios_base::beg);

    std::stringstream Sstr;
    Sstr << File.rdbuf();
    File.close();

    return Sstr.str();
}

// Check for Shader compilation/linking errors
void NShader::CheckCompileErrors(const unsigned int Shader, const std::string& Type)
{
    int Success;
    char InfoLog[1024];
    if (Type != "PROGRAM")
    {
        glGetShaderiv(Shader, GL_COMPILE_STATUS, &Success);
        if (!Success)
        {
            glGetShaderInfoLog(Shader, 1024, nullptr, InfoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << Type << "\n" << InfoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(Shader, GL_LINK_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(Shader, 1024, nullptr, InfoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << Type << "\n" << InfoLog << std::endl;
        }
    }
}
