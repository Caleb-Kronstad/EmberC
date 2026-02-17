#ifndef EMBER_WINDOW_H
#define EMBER_WINDOW_H

GLFWwindow* window_initialize(const char* title, int width, int height, bool fullscreen);
void window_begin(GLFWwindow* window);
void window_end(GLFWwindow* window);
void window_shutdown(GLFWwindow* window);

#endif
