#ifndef EMBER_H
#define EMBER_H

#include "audio.h"

ma_engine* audio_engine_initialize(ma_engine_config* config);
void audio_engine_shutdown(ma_engine* engine, audio** audios, int count);
void volume_set(ma_engine* engine, float volume);
float volume_get(ma_engine* engine);
void pitch_set(audio* audio, float pitch);
float pitch_get(audio* audio);
void loop_set(audio* audio, bool loop);
bool loop_get(audio* audio);

#endif