#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "scene.hpp"
#include "camera.hpp"

std::shared_ptr<FirstPersonCamera> FirstPersonCamera::first_person_camera = nullptr;

float g_sensetivity = 0.0f;

std::shared_ptr<FirstPersonCamera> FirstPersonCamera::get_instance()
{
    if (FirstPersonCamera::first_person_camera == nullptr)
    {
        FirstPersonCamera::first_person_camera = std::shared_ptr<FirstPersonCamera>(new FirstPersonCamera(Scene::get_instance()->glfw_window));
    }
    return FirstPersonCamera::first_person_camera;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    std::shared_ptr<FirstPersonCamera> fpc = FirstPersonCamera::first_person_camera;

    float xoffset = xpos - fpc->lastX;
    float yoffset = fpc->lastY - ypos; 
    fpc->lastX = xpos;
    fpc->lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    fpc->yaw   += xoffset;
    fpc->pitch += yoffset;

    if(fpc->pitch > 89.0f)
        fpc->pitch = 89.0f;
    if(fpc->pitch < -89.0f)
        fpc->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(fpc->yaw)) * cos(glm::radians(fpc->pitch));
    direction.y = sin(glm::radians(fpc->pitch));
    direction.z = sin(glm::radians(fpc->yaw)) * cos(glm::radians(fpc->pitch));

    fpc->camera_front = glm::normalize(direction);
}


FirstPersonCamera::FirstPersonCamera(GLFWwindow* glfw_window) : 
    glfw_window(glfw_window), 
    sensetivity(0.1f), 
    movement_speed(10.0f),
    lastX(Scene::scene->window_width / 2),
    lastY(Scene::scene->window_height / 2),
    pitch(0.0f),
    yaw(-90.0f)
{
    this->camera_pos   = glm::vec3(0.0f, 0.0f,  3.0f);
    this->camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
    this->camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

    this->view_matrix = glm::lookAt(
        this->camera_pos, // Position of camera
        this->camera_pos + this->camera_front, // Point look at
        this->camera_up // Up-direction (+y)
    );

    g_sensetivity = this->sensetivity;

    glfwSetInputMode(this->glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(this->glfw_window, mouse_callback);
}

glm::mat4 FirstPersonCamera::get_view_matrix()
{
    this->view_matrix = glm::lookAt(
        this->camera_pos, // Position of camera
        this->camera_pos + this->camera_front, // Point look at
        this->camera_up // Up-direction (+y)
    );
    return this->view_matrix;
}
void FirstPersonCamera::set_sensetivity(float sensetivity)
{
    if (sensetivity > 0)
    {
        this->sensetivity = sensetivity;
        g_sensetivity = this->sensetivity;
    }
}
void FirstPersonCamera::set_movement_speed(float speed)
{
    if (speed > 0)
    {
        this->movement_speed = speed;
    }
}
void FirstPersonCamera::process_input()
{
    bool w = glfwGetKey(this->glfw_window, GLFW_KEY_W) == GLFW_PRESS,
         a = glfwGetKey(this->glfw_window, GLFW_KEY_A) == GLFW_PRESS,
         s = glfwGetKey(this->glfw_window, GLFW_KEY_S) == GLFW_PRESS,
         d = glfwGetKey(this->glfw_window, GLFW_KEY_D) == GLFW_PRESS;

    GLfloat camera_speed = this->movement_speed * Scene::scene->frame_delta;

    if (w || a || s || d)
    {
        if (w)
        {
            camera_pos += camera_front * camera_speed;
        }
        if (s)
        {
            camera_pos -= camera_front * camera_speed;
        }
        if (a)
        {
            this->camera_pos -= glm::normalize(
                glm::cross(this->camera_front, this->camera_up)
            ) * camera_speed;
        }
        if (d)
        {
            this->camera_pos += glm::normalize(
                glm::cross(this->camera_front, this->camera_up)
            ) * camera_speed;
        }
    }
}
