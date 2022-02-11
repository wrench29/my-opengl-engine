#include <iostream>
#include <memory>

#include <epoxy/gl.h>

#include "scene.hpp"

int main()
{
    std::shared_ptr<Scene> scene = Scene::get_instance();

    scene->init();
    scene->loop();

    return 0;
}
