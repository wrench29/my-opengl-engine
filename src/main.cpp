#include <iostream>

#include <glad/glad.h>

#include "scene.hpp"

int main()
{
    Scene* scene = Scene::get_instance();

    scene->init();
    scene->loop();

    return 0;
}
