#include "includes.h"
#include "window.h"

GLFWwindow* initwindow(const char* title, int width, int height, bool fullscreen)
{
    if (!glfwInit())
    {
        logerrors("Failed to initialize GLFW");
        return NULL;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    int user_monitor_width = mode->width, user_monitor_height = mode->height;

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);

    if (!window)
    {
        logerrors("Failed to create GLFW window");
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logerrors("Failed to initialize GLAD");
        return NULL;
    }

    if (fullscreen)
    {
        glfwMaximizeWindow(window);
        glViewport(0, 0, user_monitor_width, user_monitor_height);
    }
    else
    {
        glViewport(0, 0, width, height);
    }

    igCreateContext(NULL);
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return window;
}

void beginwindow(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    igNewFrame();

    ImGuiViewport* viewport = igGetMainViewport();
    igSetNextWindowPos(viewport->Pos, 0, (ImVec2){0,0});
    igSetNextWindowSize(viewport->Size, 0);
    igSetNextWindowViewport(viewport->ID);

    igBegin("Buffer", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
    igEnd();
}

void endwindow(GLFWwindow* window)
{
    igRender();
    igEndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(igGetDrawData());

    ImGuiIO* io = igGetIO_Nil();
    if (io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        igUpdatePlatformWindows();
        igRenderPlatformWindowsDefault(NULL, NULL);
        glfwMakeContextCurrent(backup_current_context);
    }
        
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void shutdownwindow(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
}