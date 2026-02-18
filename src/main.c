#include "includes.h"

#include "math.h"
#include "window.h"
#include "style.h"
#include "ember.h"
#include "audio.h"
#include "persist.h"
#include "display.h"

#ifdef EMBER_PLATFORM_WINDOWS
#include "file/registry.h"
#include "file/filemanager.h"
#endif

#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

int main(int argc, char* argv[])
{
    enum startup_mode { MODE_DIRECTORY_SCAN, MODE_SINGLE_FILE };
    enum startup_mode mode = MODE_DIRECTORY_SCAN;
    char target_file_path[1024] = {0};
    bool show_registration_prompt = false;

#ifdef EMBER_PLATFORM_WINDOWS
    if (argc > 1)
    {
        char* normalized = file_manager_normalize_path(argv[1]);
        if (normalized)
        {
            if (file_manager_is_valid_audio_file(normalized) &&
                file_manager_file_exists(normalized))
            {
                mode = MODE_SINGLE_FILE;
                strncpy(target_file_path, normalized, sizeof(target_file_path) - 1);
                log_info_s("Opening file from command line");
            }
            else
            {
                log_error_s("Invalid audio file provided, falling back to directory mode");
            }
            free(normalized);
        }
    }
    else
    {
        if (!registry_is_registered())
        {
            show_registration_prompt = true;
        }
    }
    
    char exe_dir[1024];
    if (GetModuleFileNameA(NULL, exe_dir, sizeof(exe_dir)) != 0)
    {
        char* last_slash = strrchr(exe_dir, '\\');
        if (last_slash)
        {
            *last_slash = '\0';
            SetCurrentDirectoryA(exe_dir);
            log_info_s("Set working directory to exe location");
        }
    }
#endif

    ember_config load_config = config_initialize_defaults();
    if (!config_load(&load_config, "data/config/config.ember"))
    {
        log_error_s("Config file not found, using defaults");
    }

    GLFWwindow* window = window_initialize("Ember", load_config.window_width, load_config.window_height, false);
    float last_frame = 0.0f;

    ma_engine_config engine_config;
    ma_engine* audio_engine = audio_engine_initialize(&engine_config);

    bool loop = load_config.loop;
    float pitch = load_config.pitch;
    volume_set(audio_engine, load_config.volume);

    int audio_capacity = 64;
    int audio_count = 0;
    audio* audio_list = malloc(audio_capacity * sizeof(audio));
    audio* current_audio = NULL;
    audio* initial_audio_to_play = NULL;

    if (mode == MODE_SINGLE_FILE)
    {
        audio_list_load_single_with_directory(
            &audio_list, audio_engine, &audio_count, &audio_capacity,
            target_file_path, &initial_audio_to_play);
    }
    else
    {
        audio_list_load_from_directory(
            &audio_list, audio_engine, &audio_count, &audio_capacity,
            "data/audios", NULL);
    }

    if (initial_audio_to_play)
    {
        current_audio = initial_audio_to_play;
        audio_start(current_audio);
        loop_set(current_audio, loop);
        pitch_set(current_audio, pitch);
    }
    
    image logo;
    image_load("assets/icons/ember-logo.png", &logo);

    style style = {
        (ImVec4_c){255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 1.0f},
        (ImVec4_c){38.0f / 255.0f, 38.0f / 255.0f, 38.0f / 255.0f, 1.0f},
        (ImVec4_c){23.0f / 255.0f, 23.0f / 255.0f, 23.0f / 255.0f, 1.0f},
        (ImVec4_c){56.0f / 255.0f, 56.0f / 255.0f, 56.0f / 255.0f, 1.0f},
        (ImVec4_c){225.0f / 255.0f, 82.0f / 255.0f, 35.0f / 255.0f, 1.0f},
        (ImVec4_c){173.0f / 255.0f, 82.0f / 255.0f, 35.0f / 255.0f, 1.0f}
    };
    style_set(&style);

    bool quit = false;
    while (!quit)
    {
        window_begin(window);

        float delta_time = calculate_delta_time(&last_frame);
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

        display_audio_information(current_audio, audio_engine, &loop, &pitch, &style, &logo);

        igSpacing();
        igSpacing();
        igSeparator();
        igSpacing();

        display_all_audios(audio_list, &current_audio, audio_count, &loop, &pitch, &style);
        //

        // end imgui window
        igUnindent(16.0f);
        
#ifdef EMBER_PLATFORM_WINDOWS
        if (show_registration_prompt) {
            igOpenPopup_Str("RegisterEmberC", 0);
            show_registration_prompt = false;
        }

        if (igBeginPopupModal("RegisterEmberC", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            igText("Would you like to register EmberC as your default audio player?");
            igText("This will associate .mp3, .wav, and .ogg files with EmberC.");
            igSpacing();

            if (igButton("Yes, Register", (ImVec2_c){120, 0})) {
                char exe_path[1024];
                GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));

                if (registry_register_file_associations(exe_path)) {
                    log_info_s("EmberC registered successfully");
                } else {
                    log_error_s("Failed to register EmberC - try running as administrator");
                }

                igCloseCurrentPopup();
            }

            igSameLine(0, -1);

            if (igButton("No, Skip", (ImVec2_c){120, 0})) {
                igCloseCurrentPopup();
            }

            igEndPopup();
        }
#endif
        
        igEnd();

        window_end(window);
        if (glfwWindowShouldClose(window))
            quit = true;
    }

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    ember_config save_config;
    save_config.volume = volume_get(audio_engine);
    save_config.pitch = pitch;
    save_config.loop = loop;
    save_config.window_width = width;
    save_config.window_height = height;
    save_config.fullscreen = false;

    if (!config_save(&save_config, "data/config/config.ember"))
    {
        log_error_s("Failed to save config");
    }

    window_shutdown(window);
    audio_engine_shutdown(audio_engine, &audio_list, audio_count);
    free(audio_list);
    return 0;
}
