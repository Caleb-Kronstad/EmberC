#include "includes.h"
#include "window.h"

GLFWwindow* window_initialize(const char* title, int width, int height, bool fullscreen)
{
    if (!glfwInit())
    {
        log_error_s("Failed to initialize GLFW");
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
        log_error_s("Failed to create GLFW window");
        return NULL;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        log_error_s("Failed to initialize GLAD");
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
    
    GLFWimage images[1];
    stbi_set_flip_vertically_on_load(0);
    int channels = 4;
    images[0].pixels = stbi_load("assets/icons/ember-logo.png", &images[0].width, &images[0].height, &channels, 0);
    if (images[0].pixels)
    {
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);
    }

    igCreateContext(NULL);

#ifdef EMBER_PLATFORM_LINUX
    {
        static char ini_path[PATH_MAX];
        char exe_path[PATH_MAX];
        ssize_t exe_len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
        if (exe_len != -1)
        {
            exe_path[exe_len] = '\0';
            char* last_slash = strrchr(exe_path, '/');
            if (last_slash)
            {
                *last_slash = '\0';
                snprintf(ini_path, sizeof(ini_path), "%s/imgui.ini", exe_path);
                igGetIO_Nil()->IniFilename = ini_path;
            }
        }
    }
#endif

#ifdef EMBER_PLATFORM_WINDOWS
    {
        static char ini_path[MAX_PATH];
        char exe_path[MAX_PATH];
        if (GetModuleFileNameA(NULL, exe_path, sizeof(exe_path)) != 0)
        {
            char* last_slash = strrchr(exe_path, '\\');
            if (last_slash)
            {
                *last_slash = '\0';
                snprintf(ini_path, sizeof(ini_path), "%s\\imgui.ini", exe_path);
                igGetIO_Nil()->IniFilename = ini_path;
            }
        }
    }
#endif

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    return window;
}

void window_begin(GLFWwindow* window)
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

void window_end(GLFWwindow* window)
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

void window_shutdown(GLFWwindow* window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    igDestroyContext(NULL);
    glfwDestroyWindow(window);
    glfwTerminate();
}