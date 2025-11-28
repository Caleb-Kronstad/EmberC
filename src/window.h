#ifndef EMBER_WINDOW_H
#define EMBER_WINDOW_H

GLFWwindow* initwindow(const char* title, int width, int height, bool fullscreen);
void beginwindow(GLFWwindow* window);
void endwindow(GLFWwindow* window);
void shutdownwindow(GLFWwindow* window);

#endif
