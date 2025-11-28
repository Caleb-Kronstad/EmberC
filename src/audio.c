#include "includes.h"
#include "audio.h"

void resizeaudios(audio** audios, int* capacity, int count)
{
    if (count >= *capacity)
    {
        *capacity *= 2;
        *audios = realloc(*audios, *capacity * sizeof(audio));
    }
}

void addaudio(audio** audios, ma_engine* engine, int* count, int* capacity, int id, char* name, char* author, char* group, char* path)
{
    if (!engine)
    {
        logerrors("Failed to find audio engine, cannot create audio");
        return;
    }

    ma_sound* sound = malloc(sizeof(ma_sound));
    ma_result result = ma_sound_init_from_file(engine, path, MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_ASYNC, NULL, NULL, sound);
    if (result != MA_SUCCESS)
    {
        logerrors("Failed to load audio file at path: ");
        logerrors(path);
        free(sound);
        return;
    }

    ma_sound_set_spatialization_enabled(sound, false);
    
    resizeaudios(audios, capacity, *count);

    audio audio;
    audio.id = id;
    strcpy(audio.name, name);
    strcpy(audio.author, author);
    strcpy(audio.group, group);
    strcpy(audio.path, path);
    audio.sound = sound;
    
    (*audios)[*count] = audio;
    (*count)++;
}

void loadaudiosfromdirectory(audio** audios, ma_engine* engine, int* count, int* capacity, const char* directory_path, const char* group_name)
{
    char search_path[1024];
    (void)snprintf(search_path, sizeof(search_path), "%s\\*", directory_path);
    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(search_path, &find_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        logerrors("Failed to open directory");
        return;
    }

    int id = 0;
    if (*count > 0)
    {
        int max_id = (*audios)[0].id;
        for (int i = 1; i < *count; i++)
        {
            if ((*audios)[i].id > max_id)
                max_id = (*audios)[i].id;
        }
        id = max_id + 1;
    }

    do
    {
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // Skip . and ..
            if (strcmp(find_data.cFileName, ".") == 0 || strcmp(find_data.cFileName, "..") == 0)
                continue;
            
            // Build subdirectory path
            char subdir_path[1024];
            snprintf(subdir_path, sizeof(subdir_path), "%s\\%s", directory_path, find_data.cFileName);
            
            // Recursively load from subdirectory, using its name as the group
            loadaudiosfromdirectory(audios, engine, count, capacity, subdir_path, find_data.cFileName);
            continue;
        }

        char* name = find_data.cFileName;
        size_t len = strlen(name);

        if (len < 4)
            continue;

        char* ext = name + len - 4;
        if (strcmp(ext, ".mp3") != 0 && strcmp(ext, ".wav") != 0 && strcmp(ext, ".ogg") != 0)
            continue;

        char full_path[1024];
        (void)snprintf(full_path, sizeof(full_path), "%s\\%s", directory_path, name);

        addaudio(audios, engine, count, capacity, id++, name, "Unknown", group_name ? group_name : "Uncategorized", full_path);
    }
    while (FindNextFileA(hFind, &find_data) != 0);

    FindClose(hFind);
}

void startaudio(audio* audio)
{
    ma_sound_start(audio->sound);
}
void restartaudio(audio* audio)
{
    ma_sound_seek_to_pcm_frame(audio->sound, 0);
    ma_sound_start(audio->sound);
}
void stopaudio(audio* audio)
{
    ma_sound_stop(audio->sound);
}

bool isaudioplaying(audio* audio)
{
    return ma_sound_is_playing(audio->sound) == MA_TRUE;
}
bool isaudioended(audio* audio)
{
    return ma_sound_at_end(audio->sound) == MA_FALSE;
}

ma_uint64 getdurationinframes(audio* audio)
{
    ma_uint64 length = 0;
    ma_sound_get_length_in_pcm_frames(audio->sound, &length);
    return length;
}

ma_uint64 getpositioninframes(audio* audio)
{
    ma_uint64 cursor = 0;
    ma_sound_get_cursor_in_pcm_frames(audio->sound, &cursor);
    return cursor;
}

float getdurationinseconds(audio* audio, ma_engine* engine)
{
    ma_uint64 length_in_frames = getdurationinframes(audio);
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    return (float)length_in_frames / (float)sample_rate;
}

float getpositioninseconds(audio* audio, ma_engine* engine)
{
    ma_uint64 cursor_in_frames = getpositioninframes(audio);
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    return (float)cursor_in_frames / (float)sample_rate;
}

bool seektoframe(audio* audio, ma_uint64 frame_index)
{
    ma_result result = ma_sound_seek_to_pcm_frame(audio->sound, frame_index);
    return result == MA_SUCCESS;
}

bool seektotime(audio* audio, ma_engine* engine, float time_in_seconds)
{
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    ma_uint64 frame_index = (ma_uint64)(time_in_seconds * (float)sample_rate);
    return seektoframe(audio, frame_index);
}