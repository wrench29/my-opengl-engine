#include "cube.hpp"

Cube::Cube()
{
    float vert[] = {
        -1.0f, -1.0f, -1.0f,  // 0
        -1.0f,  1.0f, -1.0f,  // 4
         1.0f,  1.0f, -1.0f,  // 7

        -1.0f, -1.0f, -1.0f,  // 0
         1.0f, -1.0f, -1.0f,  // 3
         1.0f,  1.0f, -1.0f,  // 7

        -1.0f, -1.0f,  1.0f,  // 1
        -1.0f, -1.0f, -1.0f,  // 0
        -1.0f,  1.0f, -1.0f,  // 4

        -1.0f, -1.0f,  1.0f,  // 1
        -1.0f,  1.0f,  1.0f,  // 5
        -1.0f,  1.0f, -1.0f,  // 4

        -1.0f, -1.0f,  1.0f,  // 1
        -1.0f,  1.0f,  1.0f,  // 5
         1.0f,  1.0f,  1.0f,  // 6

        -1.0f, -1.0f,  1.0f,  // 1
         1.0f, -1.0f,  1.0f,  // 2
         1.0f,  1.0f,  1.0f,  // 6

         1.0f, -1.0f, -1.0f,  // 3
         1.0f,  1.0f, -1.0f,  // 7
         1.0f,  1.0f,  1.0f,  // 6

         1.0f, -1.0f, -1.0f,  // 3
         1.0f, -1.0f,  1.0f,  // 2
         1.0f,  1.0f,  1.0f,  // 6

        -1.0f, -1.0f, -1.0f,  // 0
        -1.0f, -1.0f,  1.0f,  // 1
         1.0f, -1.0f,  1.0f,  // 2

        -1.0f, -1.0f, -1.0f,  // 0
         1.0f, -1.0f,  1.0f,  // 2
         1.0f, -1.0f, -1.0f,  // 3

        -1.0f,  1.0f, -1.0f,  // 4
        -1.0f,  1.0f,  1.0f,  // 5
         1.0f,  1.0f,  1.0f,  // 6

        -1.0f,  1.0f, -1.0f,  // 4
         1.0f,  1.0f,  1.0f,  // 6
         1.0f,  1.0f, -1.0f   // 7
    };
    this->init(vert, sizeof(vert) / sizeof(float));
}

void Cube::draw()
{
    this->low_draw();
}