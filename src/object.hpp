#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <epoxy/gl.h>

#include "shader.hpp"
#include "scene.hpp"

class Object
{
public:
	virtual void draw() = 0;
	void move(glm::vec3 move_vector)
	{
		this->model_matrix = glm::translate(this->model_matrix, move_vector);
	}
	void set_color(glm::vec3 color)
	{
		this->color = color;
	}
protected:
	Object() : VAO(0), VBO(0), verticies(nullptr), verticies_length(0), model_matrix(1.0f), color(1.0f, 1.0f, 1.0f) {};
	void init(float* vert, unsigned int vert_len)
	{
		this->verticies_length = vert_len;
		this->verticies = std::shared_ptr<float[]>(new float[vert_len]);
		for (unsigned int i = 0; i < vert_len; i++)
		{
			this->verticies[i] = vert[i];
		}

		this->shader.load_source_file(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl");
		this->shader.load_source_file(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl");
		this->shader.create_program();

		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		glBufferData(GL_ARRAY_BUFFER, this->verticies_length * sizeof(float), this->verticies.get(), GL_STATIC_DRAW);

		glVertexAttribPointer(this->shader.get_attrib_location("aPos"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);
	}
	void low_draw()
	{
		shader.use();

		this->shader.set_vec3("color", this->color);
		this->shader.set_mat4("model", this->model_matrix);
		this->shader.set_mat4("view", Scene::scene->fps_camera->get_view_matrix());
		this->shader.set_mat4("projection", Scene::scene->projection_matrix);

		glBindVertexArray(this->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glDrawArrays(GL_TRIANGLES, 0, this->verticies_length / 3);
	}
	GLuint VAO;
	GLuint VBO;
	std::shared_ptr<float[]> verticies;
	unsigned int verticies_length;
	glm::mat4 model_matrix;
	glm::vec3 color;
	Shader shader;
};
