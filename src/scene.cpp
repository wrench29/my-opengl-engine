#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.hpp"
#include "scene.hpp"
#include "camera.hpp"

Scene* Scene::scene = nullptr;

Scene* Scene::get_instance()
{
    if (scene == nullptr)
    {
        scene = new Scene();
    }
    return scene;
}

void Scene::on_error()
{
    if (this->glfw_initialized)
    {
        glfwTerminate();
    }
    exit(1);
}

void Scene::process_input()
{
    if (glfwGetKey(this->glfw_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(this->glfw_window, true);
    }
}

////
// Callbacks
////
void Scene::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Scene* scene = Scene::scene;
    scene->window_width = width;
    scene->window_height = height;
    scene->projection_matrix = glm::perspective(
        glm::radians(70.0f),
        (float)scene->window_width / (float)scene->window_height,
        0.1f,
        100.0f
    );
    glViewport(0, 0, width, height);
}
////

void Scene::set_callbacks()
{
    glfwSetFramebufferSizeCallback(this->glfw_window, Scene::framebuffer_size_callback);
}

void Scene::init()
{
    this->glfw_initialized = glfwInit();
    if (!this->glfw_initialized)
    {
        std::cerr << "failed to initialize GLFW" << std::endl;
        this->on_error();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->glfw_window = glfwCreateWindow(this->window_width, this->window_height, "Hello Window", nullptr, nullptr);
    if (this->glfw_window == nullptr)
    {
        std::cerr << "failed to create window" << std::endl;
        this->on_error();
    }

    glfwMakeContextCurrent(this->glfw_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "failed to initialize opengl" << std::endl;
        this->on_error();
    }

    glfwSwapInterval(1);

    this->set_callbacks();
}

void Scene::loop()
{
    // Depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Shader shader;
    shader.load_source_file(GL_VERTEX_SHADER, "shaders/vertex_shader.glsl");
    shader.load_source_file(GL_FRAGMENT_SHADER, "shaders/fragment_shader.glsl");
    shader.create_program();

    float verticies[] = 
    {
        -1.0f, -1.0f, -1.0f, // 1
        -1.0f, -1.0f, 1.0f,  // 2
        1.0f, -1.0f, 1.0f,   // 3
        1.0f, -1.0f, -1.0f,  // 4
        -1.0f, 1.0f, -1.0f,  // 5
        -1.0f, 1.0f, 1.0f,   // 6
        1.0f, 1.0f, 1.0f,    // 7
        1.0f, 1.0f, -1.0f    // 8
    };
    int indices[] =
    {
        0, 4, 7,
        0, 3, 7,

        1, 0, 4,
        1, 5, 4,

        1, 5, 6,
        1, 2, 6,

        3, 7, 6,
        3, 2, 6,

        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7
    };

    unsigned int VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    glVertexAttribPointer(shader.get_attrib_location("aPos"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    GLuint h_mvp = shader.get_uniform_location("MVP");

    this->fps_camera = FirstPersonCamera::get_instance();

    // Matrices

    // Perspective
    this->projection_matrix = glm::perspective(
        glm::radians(70.0f),
        (float)this->window_width / (float)this->window_height,
        0.1f,
        100.0f
    );

    // Model
    glm::mat4 model_matrix = glm::mat4(1.0f);

    glm::mat4 MVP;

    float current_frame = 0.0f, previous_frame = 0.0f;

    while (!glfwWindowShouldClose(this->glfw_window))
    {
        current_frame = glfwGetTime();
        this->frame_delta = current_frame - previous_frame;
        previous_frame = current_frame;

        this->process_input();
        this->fps_camera->process_input();

        // Render process
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        model_matrix = glm::rotate(model_matrix, this->frame_delta * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 1.0f));
        MVP = projection_matrix * this->fps_camera->get_view_matrix() * model_matrix;
        glUniformMatrix4fv(h_mvp, 1, GL_FALSE, &MVP[0][0]);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(this->glfw_window);
        // End of render process

        glfwPollEvents();    
    }
}
