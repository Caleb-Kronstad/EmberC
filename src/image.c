#include "includes.h"
#include "image.h"

void image_load(const char* path, image* image)
{
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    image->id = id;

    stbi_set_flip_vertically_on_load(0);
    int channels;
    unsigned char* pixels = stbi_load(path, &image->width, &image->height, &channels, 4);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    stbi_image_free(pixels);
}

void image_render(const image* image)
{
    igImage(
        (ImTextureRef_c){._TexData = NULL, ._TexID = (ImTextureID)(uintptr_t)image->id},
        (ImVec2_c){(float)image->width, (float)image->height},
        (ImVec2_c){0, 0},
        (ImVec2_c){1, 1}
    );
}
