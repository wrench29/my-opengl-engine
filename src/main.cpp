#include <iostream>
#include <memory>

#include <glad/glad.h>

#include "scene.hpp"

int main()
{
    std::shared_ptr<Scene> scene = Scene::get_instance();

    scene->init();
    scene->loop();

    return 0;
}
