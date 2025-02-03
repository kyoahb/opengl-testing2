#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "Useful.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
	std::string vertPath;
	std::string fragPath;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
	Shader(const char* vertexPath, const char* fragmentPath) : vertPath(vertexPath), fragPath(fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();


        // 2. compile shaders
        unsigned int vertex, fragment;

        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() const
    {
        glUseProgram(ID);
    }

    void printActiveUniforms() {
        GLint uniformCount = 0;
        glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniformCount);
        for (int i = 0; i < uniformCount; i++) {
            char name[256];
            int length;
            int size;
            GLenum type;
            glGetActiveUniform(ID, i, 256, &length, &size, &type, name);
            std::cout << "Uniform " << i << " Type: " << type << " Name: " << name << std::endl;
        }
    }

    void printActiveAttributes() {
        GLint attributeCount = 0;
        glGetProgramiv(ID, GL_ACTIVE_ATTRIBUTES, &attributeCount);
        for (int i = 0; i < attributeCount; i++) {
            char name[256];
            int length;
            int size;
            GLenum type;
            glGetActiveAttrib(ID, i, 256, &length, &size, &type, name);
            std::cout << "Attribute " << i << " Type: " << type << " Name: " << name << std::endl;
        }
    }

    bool isShaderBound() const {
		GLint currentProgram;
		glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
		
        return currentProgram == ID;
    }

	// Returns uniform location of a uniform in the shader
	GLuint getLocation(const std::string& name) const {
        if (!isShaderBound()) {
            use();
        }
		GLuint location = glGetUniformLocation(ID, name.c_str());

        // Uniform does not exist error
        ASSERT_LOG(location != -1, "Uniform " + name + " has been optimised out / has not been found in " + vertPath + " " + fragPath);
        
        return location;
	}

    // utility uniform functions.
    // All require shader to be bound.
    // ------------------------------------------------------------------------
    void setBool(const std::string& name, bool value) const
    {
		GLuint location = getLocation(name.c_str());
        glUniform1i(location, (int)value);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string& name, int value) const
    {
        GLuint location = getLocation(name.c_str());
        glUniform1i(location, value);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string& name, float value) const
    {
        GLuint location = getLocation(name.c_str());
        glUniform1f(location, value);
    }
	void setVec3(const std::string& name, glm::vec3 value) const
	{
        GLuint location = getLocation(name.c_str());
		glUniform3fv(location, 1, glm::value_ptr(value));
	}
    void setMat4(const std::string& name, glm::mat4 value) const
    {
        GLuint location = getLocation(name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};
#endif