#include <iostream>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "utils.hpp"
#include "scene.hpp"
#include "camera.hpp"
#include "cube.hpp"

std::shared_ptr<Scene> Scene::scene = nullptr;

std::shared_ptr<Scene> Scene::get_instance()
{
    if (scene == nullptr)
    {
        scene = std::shared_ptr<Scene>(new Scene());
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
    std::shared_ptr<Scene> scene = Scene::get_instance();
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
    
    this->fps_camera = FirstPersonCamera::get_instance();

    // Matrices

    // Perspective
    this->projection_matrix = glm::perspective(
        glm::radians(70.0f),
        (float)this->window_width / (float)this->window_height,
        0.1f,
        100.0f
    );

    Cube cube;

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

        cube.draw();

        glfwSwapBuffers(this->glfw_window);
        // End of render process

        glfwPollEvents();    
    }
}
