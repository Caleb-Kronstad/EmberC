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

void resizeaudios(audio** audios, int* capacity, int count);
void addaudio(audio** audios, ma_engine* engine, int* count, int* capacity, int id, char* name, char* author, char* group, char* path);
void loadaudiosfromdirectory(audio** audios, ma_engine* engine, int* count, int* capacity, const char* directory_path, const char* group_name);

ma_uint64 getdurationinframes(audio* audio);
ma_uint64 getpositioninframes(audio* audio);
float getdurationinseconds(audio* audio, ma_engine* engine);
float getpositioninseconds(audio* audio, ma_engine* engine);
bool seektoframe(audio* audio, ma_uint64 frame_index);
bool seektotime(audio* audio, ma_engine* engine, float time_in_seconds);

void startaudio(audio* audio);
void restartaudio(audio* audio);
void stopaudio(audio* audio);
bool isaudioplaying(audio* audio);
bool isaudioended(audio* audio);

void formattime(float seconds, char* buffer, size_t buffer_size);

#endif