#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "shader.hpp"
#include "camera.hpp"

class Scene
{
public:
    Scene(Scene &other) = delete;
    void operator=(const Scene &) = delete;

    void init();
    void loop();

    static std::shared_ptr<Scene> get_instance();
private:
    Scene(): 
        glfw_initialized(false), 
        glfw_window(nullptr),
        window_width(1280),
        window_height(720),
        frame_delta(0.0f),
        projection_matrix(0.0f),
        view_matrix(0.0f) {};
    static std::shared_ptr<Scene> scene;

    // Scene variables
    bool glfw_initialized;
    GLFWwindow* glfw_window;
    int window_width, window_height;
    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    float frame_delta;
    std::shared_ptr<FirstPersonCamera> fps_camera;

    // Scene functions
    void on_error();
    void process_input();
    void set_callbacks();

    // Callbacks
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    friend class FirstPersonCamera;
    friend class Object;
};
