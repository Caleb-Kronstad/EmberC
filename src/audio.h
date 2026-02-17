#ifndef EMBER_AUDIO_H
#define EMBER_AUDIO_H

typedef struct audio
{
    int id;
    char name[256];
    char author[256];
    char group[256];
    char path[512];
    ma_sound* sound;
} audio;

void audio_list_resize(audio** audios, int* capacity, int count);
void audio_list_add_audio(audio** audios, ma_engine* engine, int* count, int* capacity, int id, char* name, char* author, char* group, char* path);
void audio_list_load_from_directory(audio** audios, ma_engine* engine, int* count, int* capacity, const char* directory_path, const char* group_name);
void audio_list_load_single_with_directory(audio** audios, ma_engine* engine, int* count, int* capacity, const char* target_file_path, audio** initial_audio);

ma_uint64 audio_duration_in_frames(audio* audio);
ma_uint64 audio_position_in_frames(audio* audio);
float audio_duration_in_seconds(audio* audio, ma_engine* engine);
float audio_position_in_seconds(audio* audio, ma_engine* engine);
bool audio_seek_to_frame(audio* audio, ma_uint64 frame_index);
bool audio_seek_to_time(audio* audio, ma_engine* engine, float time_in_seconds);

void audio_start(audio* audio);
void audio_restart(audio* audio);
void audio_stop(audio* audio);
bool audio_playing(audio* audio);
bool audio_ended(audio* audio);

void audio_time_format(float seconds, char* buffer, size_t buffer_size);

#endif