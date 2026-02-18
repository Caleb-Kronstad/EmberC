#ifndef EMBER_IMAGE_H
#define EMBER_IMAGE_H

#include "glad/glad.h"

typedef struct image
{
    GLuint id;
    int width;
    int height;
} image;

void image_load(const char* path, image* image);
void image_render(const image* image);

#endif