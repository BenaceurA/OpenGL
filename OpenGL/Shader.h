#pragma once
#include <string>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use() const;
    // utility uniform functions
    void setUniformBool(const std::string& name, bool value) const;
    void setUniformInt(const std::string& name, int value) const;
    void setUniformFloat(const std::string& name, float value) const;
    void setVec3(const std::string& name, float r, float g, float b) const;
    void setVec3(const std::string& name, glm::vec3 vec) const;
    void setVec4(const std::string& name, float r, float g, float b, float a) const;
    void setVec4(const std::string& name, glm::vec4 vec) const;
    void setMat4(const std::string& name, glm::mat4 mat) const;
};

