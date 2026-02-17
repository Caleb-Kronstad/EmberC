#include "includes.h"
#include "audio.h"

void audio_list_resize(audio** audios, int* capacity, int count)
{
    if (count >= *capacity)
    {
        *capacity *= 2;
        *audios = realloc(*audios, *capacity * sizeof(audio));
    }
}

void audio_list_add_audio(audio** audios, ma_engine* engine, int* count, int* capacity, int id, char* name,
                          char* author, char* group, char* path)
{
    if (!engine)
    {
        log_error_s("Failed to find audio engine, cannot create audio");
        return;
    }

    ma_sound* sound = malloc(sizeof(ma_sound));
    ma_result result = ma_sound_init_from_file(engine, path, MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_ASYNC, NULL, NULL,
                                               sound);
    if (result != MA_SUCCESS)
    {
        log_error_s("Failed to load audio file at path: ");
        log_error_s(path);
        free(sound);
        return;
    }

    ma_sound_set_spatialization_enabled(sound, false);

    audio_list_resize(audios, capacity, *count);

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

void audio_list_load_from_directory(audio** audios, ma_engine* engine, int* count, int* capacity,
                                    const char* directory_path, const char* group_name)
{
    char search_path[1024];
    (void)snprintf(search_path, sizeof(search_path), "%s\\*", directory_path);
    WIN32_FIND_DATAA find_data;
    HANDLE hFind = FindFirstFileA(search_path, &find_data);

    if (hFind == INVALID_HANDLE_VALUE)
    {
        log_error_s("Failed to open directory");
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
            audio_list_load_from_directory(audios, engine, count, capacity, subdir_path, find_data.cFileName);
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

        audio_list_add_audio(audios, engine, count, capacity, id++, name, "Unknown",
                             group_name ? group_name : "Uncategorized", full_path);
    }
    while (FindNextFileA(hFind, &find_data) != 0);

    FindClose(hFind);
}

void audio_start(audio* audio)
{
    ma_sound_start(audio->sound);
}

void audio_restart(audio* audio)
{
    ma_sound_seek_to_pcm_frame(audio->sound, 0);
    ma_sound_start(audio->sound);
}

void audio_stop(audio* audio)
{
    ma_sound_stop(audio->sound);
}

bool audio_playing(audio* audio)
{
    return ma_sound_is_playing(audio->sound) == MA_TRUE;
}

bool audio_ended(audio* audio)
{
    return ma_sound_at_end(audio->sound) == MA_FALSE;
}

ma_uint64 audio_duration_in_frames(audio* audio)
{
    ma_uint64 length = 0;
    ma_sound_get_length_in_pcm_frames(audio->sound, &length);
    return length;
}

ma_uint64 audio_position_in_frames(audio* audio)
{
    ma_uint64 cursor = 0;
    ma_sound_get_cursor_in_pcm_frames(audio->sound, &cursor);
    return cursor;
}

float audio_duration_in_seconds(audio* audio, ma_engine* engine)
{
    ma_uint64 length_in_frames = audio_duration_in_frames(audio);
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    return (float)length_in_frames / (float)sample_rate;
}

float audio_position_in_seconds(audio* audio, ma_engine* engine)
{
    ma_uint64 cursor_in_frames = audio_position_in_frames(audio);
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    return (float)cursor_in_frames / (float)sample_rate;
}

bool audio_seek_to_frame(audio* audio, ma_uint64 frame_index)
{
    ma_result result = ma_sound_seek_to_pcm_frame(audio->sound, frame_index);
    return result == MA_SUCCESS;
}

bool audio_seek_to_time(audio* audio, ma_engine* engine, float time_in_seconds)
{
    ma_uint32 sample_rate = ma_engine_get_sample_rate(engine);
    ma_uint64 frame_index = (ma_uint64)(time_in_seconds * (float)sample_rate);
    return audio_seek_to_frame(audio, frame_index);
}

void audio_time_format(float seconds, char* buffer, size_t buffer_size)
{
    int total_seconds = (int)seconds;
    int minutes = total_seconds / 60;
    int secs = total_seconds % 60;
    (void)snprintf(buffer, buffer_size, "%d:%02d", minutes, secs);
}

void audio_list_load_single_with_directory(
    audio** audios,
    ma_engine* engine,
    int* count,
    int* capacity,
    const char* target_file_path,
    audio** initial_audio)
{
    if (!audios || !engine || !count || !capacity || !target_file_path)
    {
        return;
    }

    char filename[256] = {0};
    char parent_dir[1024] = {0};

#ifdef EMBER_PLATFORM_WINDOWS
#include "file/filemanager.h"
    file_manager_extract_filename(target_file_path, filename, sizeof(filename));
    file_manager_extract_parent_directory(target_file_path, parent_dir, sizeof(parent_dir));
#else
    const char* last_slash = strrchr(target_file_path, '/');
    if (last_slash)
    {
        size_t dir_len = last_slash - target_file_path;
        strncpy(parent_dir, target_file_path, dir_len);
        parent_dir[dir_len] = '\0';

        const char* name_start = last_slash + 1;
        const char* dot = strrchr(name_start, '.');
        size_t name_len = dot ? (dot - name_start) : strlen(name_start);
        strncpy(filename, name_start, name_len);
        filename[name_len] = '\0';
    }
#endif

    if (strlen(parent_dir) == 0)
    {
        log_error_s("Could not extract parent directory from file path");
        return;
    }

    int initial_id = *count;
    audio_list_add_audio(audios, engine, count, capacity,
                         initial_id, filename, "Unknown", "Current Folder",
                         (char*)target_file_path);

    if (*count > initial_id && initial_audio)
    {
        *initial_audio = &(*audios)[initial_id];
    }

    audio_list_load_from_directory(audios, engine, count, capacity,
                                   parent_dir, "Current Folder");
}
