#pragma once

#include <memory>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class FirstPersonCamera
{
public:
    FirstPersonCamera(FirstPersonCamera &other) = delete;
    void operator=(const FirstPersonCamera &) = delete;

    static std::shared_ptr<FirstPersonCamera> get_instance();

    glm::mat4 get_view_matrix();
    void set_sensetivity(float sensetivity);
    void set_movement_speed(float speed);
    void process_input();
private:
    FirstPersonCamera(GLFWwindow* glfw_window);
    static std::shared_ptr<FirstPersonCamera> first_person_camera;

    GLFWwindow* glfw_window;
    float sensetivity;
    float movement_speed;
    glm::vec3 camera_pos;
    glm::vec3 camera_front;
    glm::vec3 camera_up;
    glm::mat4 view_matrix;
    float lastX;
    float lastY;
    float pitch;
    float yaw;

    friend void mouse_callback(GLFWwindow* window, double xpos, double ypos);
};
