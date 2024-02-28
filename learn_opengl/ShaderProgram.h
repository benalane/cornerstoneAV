#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>

class ShaderProgram {
   public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    ShaderProgram(const char *vertexPath, const char *fragmentPath);
    ~ShaderProgram();
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif //SHADER_PROGRAM_H