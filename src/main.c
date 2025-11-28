#include "includes.h"

#include "math.h"
#include "window.h"
#include "style.h"
#include "ember.h"
#include "audio.h"
#include "display.h"

int main(int argc, char *argv[])
{
    GLFWwindow* window = initwindow("Ember", 1280, 720, false);
    float last_frame = 0.0f;

    ma_engine_config engine_config;
    ma_engine* audio_engine = initaudioengine(&engine_config);
    
    int audio_capacity = 64;
    int audio_count = 0;
    audio* audios = malloc(audio_capacity * sizeof(audio));
    audio* current_audio = NULL;

    loadaudiosfromdirectory(&audios, audio_engine, &audio_count, &audio_capacity, "data/audios", NULL);

    setimguistyle(
        (ImVec4_c){255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f, 1.0f}, (ImVec4_c){38.0f/255.0f, 38.0f/255.0f, 38.0f/255.0f, 1.0f},
        (ImVec4_c){23.0f/255.0f, 23.0f/255.0f, 23.0f/255.0f, 1.0f}, (ImVec4_c){56.0f/255.0f, 56.0f/255.0f, 56.0f/255.0f, 1.0f},
        (ImVec4_c){116.0f/255.0f, 77.0f/255.0f, 169.0f/255.0f, 1.0f}, (ImVec4_c){141.0f/255.0f, 124.0f/255.0f, 192.0f/255.0f, 1.0f}
    );

    bool quit = false;
    while (!quit)
    {
        beginwindow(window);
        
        float delta_time = calcdt(&last_frame);
        int window_width, window_height;
        glfwGetWindowSize(window, &window_width, &window_height);

        // begin imgui window
        igSetNextWindowPos((ImVec2_c){0, 0}, 0, (ImVec2_c){0, 0});
        igSetNextWindowSize((ImVec2_c){(float)window_width, (float)window_height}, 0);
        igBegin("Ember", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        igIndent(16.0f);

        // window content
        igSpacing();
        igSpacing();
        igSpacing();
        igSpacing();

        displayaudioinformation(current_audio, audio_engine);

        igSpacing();
        igSpacing();
        igSeparator();
        igSpacing();

        displayallaudios(audios, &current_audio, audio_count);
        //

        // end imgui window
        igUnindent(16.0f);
        igEnd();

        endwindow(window);
        if (glfwWindowShouldClose(window))
            quit = true;
    }

    shutdownwindow(window);
    shutdownaudioengine(audio_engine, &audios, audio_count);
    free(audios);
    return 0;
}