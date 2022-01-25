#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

void Shader::update_status()
{
    int status = 0;

    // Update shader program status    
    glGetProgramiv(this->shader_program, GL_LINK_STATUS, &status);
    if (!status)
    {
        this->ready_to_use = false;
    }
    else
    {
        this->ready_to_use = true;
    }

    // Update shaders status
    glGetShaderiv(this->vertex_shader, GL_COMPILE_STATUS, &status);
    if (!status)
    {
        this->ready_to_link = false;
        return;
    }
    else
    {
        glGetShaderiv(this->fragment_shader, GL_COMPILE_STATUS, &status);
        if (!status)
        {
            this->ready_to_link = false;
            return;
        }
        this->ready_to_link = true;
    }
}

bool Shader::load_source(unsigned int type, const std::string source)
{
    if (type == GL_VERTEX_SHADER || type == GL_GEOMETRY_SHADER || type == GL_FRAGMENT_SHADER)
    {
        unsigned int __shader = glCreateShader(type);
        int success = 0;
        const char* __source = source.c_str();
        glShaderSource(__shader, 1, &__source, NULL);
        glCompileShader(__shader);
        glGetShaderiv(__shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            char info[512];
            glGetShaderInfoLog(__shader, 512, NULL, info);
            std::cout << info << std::endl;
            return false;
        }
        unsigned int* shader_ptr = nullptr;
        switch (type)
        {
        case GL_VERTEX_SHADER:
            shader_ptr = &this->vertex_shader;
            break;
        case GL_GEOMETRY_SHADER:
            shader_ptr = &this->geometry_shader;
            break;
        case GL_FRAGMENT_SHADER:
            shader_ptr = &this->fragment_shader;
            break;
        }
        glDeleteShader(*shader_ptr);
        *shader_ptr = __shader;
        this->update_status();
        return true;
    }
    return false;
}
bool Shader::load_source_file(unsigned int type, const std::string source)
{
    std::ifstream file(source);
    if (!file.is_open())
    {
        return false;
    }
    std::stringstream sstr;
    sstr << file.rdbuf();
    file.close();
    return this->load_source(type, sstr.str());
}
bool Shader::create_program()
{
    if (!this->ready_to_link)
    {
        return false;
    }
    unsigned int __program = 0;
    int status = 0;

    __program = glCreateProgram();
    glAttachShader(__program, this->vertex_shader);
    glAttachShader(__program, this->fragment_shader);
    if (this->geometry_shader != 0)
    {
        glAttachShader(__program, this->geometry_shader);
    }
    glLinkProgram(__program);
    
    glGetProgramiv(__program, GL_LINK_STATUS, &status);
    if (!status)
    { 
        char info[512];
        glGetProgramInfoLog(__program, 512, NULL, info);
        std::cout << info << std::endl;
        return false;
    }
    glDeleteProgram(this->shader_program);
    this->shader_program = __program;
    this->update_status();
    return true;
}
bool Shader::use()
{
    if (this->ready_to_use)
    {
        glUseProgram(this->shader_program);
        return true;
    }
    return false;
}
int Shader::get_attrib_location(const std::string name)
{
    if (!this->ready_to_use)
    {
        return -1;
    }
    return glGetAttribLocation(this->shader_program, name.c_str());
}
int Shader::get_uniform_location(const std::string name)
{
    if (!this->ready_to_use)
    {
        return -1;
    }
    return glGetUniformLocation(this->shader_program, name.c_str());
}

void Shader::set_mat4(const std::string name, const glm::mat4 matrix)
{
    glUniformMatrix4fv(this->get_uniform_location(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::set_vec3(const std::string name, const glm::vec3 vector)
{
    glUniform3fv(this->get_uniform_location(name), 1, &vector[0]);
}
