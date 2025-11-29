#include "includes.h"
#include "display.h"

#include "ember.h"
#include "audio.h"

void displayaudioinformation(audio* audio, ma_engine* engine, bool* loop)
{
    ImVec2_c window_size = igGetWindowSize();
    
    if (!audio)
    {
        ImVec2_c text_size = igCalcTextSize("Select a song to play", NULL, false, -1.0f);
        igSetCursorPosX((window_size.x - text_size.x) * 0.5f);
        igText("Select a song to play");
        return;
    }
    
    ImVec2_c text_size = igCalcTextSize(audio->name, NULL, false, -1.0f);
    igSetCursorPosX((window_size.x - text_size.x) * 0.5f);
    igText(audio->name);

    // play/pause
    float button_width = 50.0f;
    igSetCursorPosX((window_size.x - button_width) * 0.5f);
    if (isaudioplaying(audio))
    {
        if (igButton("Pause", (ImVec2_c){button_width, 0}))
            stopaudio(audio);
    }
    else
    {
        if (igButton("Play", (ImVec2_c){button_width, 0}))
            startaudio(audio);
    }

    // loop
    igSetCursorPosX((window_size.x - text_size.x) * 0.5f);
    igSameLine(0, -1);
    bool looping = getloop(audio);
    if (igCheckbox("Loop", &looping))
    {
        setloop(audio, looping);
        *loop = looping;
    }

    // volume slider
    float slider_width = 200.0f;
    igSetCursorPosX((window_size.x - slider_width) * 0.5f);
    igSetNextItemWidth(slider_width);
    float volume = getvolume(engine);
    if (igSliderFloat("Volume", &volume, 0.0f, 100.0f, "%.f", 0))
        setvolume(engine, volume);

    // pitch slider
    igSetCursorPosX((window_size.x - slider_width) * 0.5f);
    igSetNextItemWidth(slider_width);
    float pitch = getpitch(audio);
    if (igSliderFloat("Pitch", &pitch, 0.0f, 100.0f, "%.f", 0))
        setpitch(audio, pitch);

    float current_position = getpositioninseconds(audio, engine);
    float duration = getdurationinseconds(audio, engine);
    float progress_bar_offset = window_size.x / 2.0f - 250.0f;
    if (duration >= 0.0f)
    {
        igIndent(progress_bar_offset);
        
        float progress = current_position / duration;

        char time_current[16], time_duration[16], time_label[64];
        formattime(current_position, time_current, sizeof(time_current));
        formattime(duration, time_duration, sizeof(time_duration));
        (void)snprintf(time_label, sizeof(time_label), "%s / %s", time_current, time_duration);
    
        float progress_width = window_size.x - (progress_bar_offset*2) - 32.0f;
        igSetCursorPosX((window_size.x - progress_width) * 0.5f);
        
        ImVec2_c cursor_pos = igGetCursorScreenPos();
        igProgressBar(progress, (ImVec2_c){progress_width, 0}, time_label);
        
        igSetCursorScreenPos(cursor_pos);
        igInvisibleButton("##ProgressBarScrub", (ImVec2_c){progress_width, igGetFrameHeight()}, 0);
        if (igIsItemActive())
        {
            ImVec2_c mouse_pos = igGetMousePos();
            float mouse_x = mouse_pos.x - cursor_pos.x;
            float new_progress = mouse_x / progress_width;
            if (new_progress < 0.0f) new_progress = 0.0f;
            if (new_progress > 1.0f) new_progress = 1.0f;
            seektotime(audio, engine, new_progress * duration);
        }
        
        float grab_size = 10.0f;
        float grab_x = cursor_pos.x + (progress * progress_width) - (grab_size * 0.5f);
        float grab_y = cursor_pos.y;
        ImDrawList* draw_list = igGetWindowDrawList();
        ImDrawList_AddRectFilled(draw_list, 
            (ImVec2_c){grab_x, grab_y}, 
            (ImVec2_c){grab_x + grab_size, grab_y + igGetFrameHeight()},
            igGetColorU32_Vec4(*(ImVec4*)&igGetStyle()->Colors[ImGuiCol_PlotHistogramHovered]), 
            0.0f, 0);
        
        igUnindent(progress_bar_offset);
    }
}

void displayallaudios(audio* audios, audio** current_audio, int count, bool* loop)
{
    char unique_groups[256][256];  // Max 256 groups
    int group_count = 0;
    
    for (int i = 0; i < count; i++)
    {
        bool found = false;
        for (int j = 0; j < group_count; j++)
        {
            if (strcmp(unique_groups[j], audios[i].group) == 0)
            {
                found = true;
                break;
            }
        }
        
        if (!found)
        {
            strcpy(unique_groups[group_count], audios[i].group);
            group_count++;
        }
    }
    
    for (int g = 0; g < group_count; g++)
    {
        igPushStyleColor_Vec4(ImGuiCol_Text, (ImVec4_c){141.0f/255.0f, 124.0f/255.0f, 192.0f/255.0f, 1.0f});
        igText(unique_groups[g]);
        igPopStyleColor(1);
        igSpacing();
        
        for (int i = 0; i < count; i++)
        {
            if (strcmp(audios[i].group, unique_groups[g]) == 0)
            {
                igPushID_Int(audios[i].id);
                
                if (igSelectable_Bool(audios[i].name, *current_audio == &audios[i], 0, (ImVec2_c){300, 15}))
                {
                    if (*current_audio)
                        stopaudio(*current_audio);
                    *current_audio = &audios[i];
                    restartaudio(*current_audio);
                    setloop(*current_audio, true);
                }
                
                igPopID();
            }
        }
        igSpacing();
        igSpacing();
        igSeparator();
        igSpacing();
        igSpacing();
    }
}