#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

class Shader
{
public:
    Shader(): 
        ready_to_link(false),
        ready_to_use(false),
        vertex_shader(0),
        geometry_shader(0),
        fragment_shader(0),
        shader_program(0) {};
    ~Shader()
    {
        glDeleteShader(this->vertex_shader);
        glDeleteShader(this->geometry_shader);
        glDeleteShader(this->fragment_shader);
        glDeleteProgram(this->shader_program);
    };
    bool load_source(unsigned int type, const std::string source);
    bool load_source_file(unsigned int type, const std::string source);
    bool create_program();
    bool use();
    int get_attrib_location(const std::string name);
    int get_uniform_location(const std::string name);
    void set_mat4(const std::string name, const glm::mat4 matrix);
private:
    bool ready_to_link;
    bool ready_to_use;
    unsigned int vertex_shader, geometry_shader, fragment_shader, shader_program;

    void update_status();
};
