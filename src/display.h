#ifndef EMBER_DISPLAY_H
#define EMBER_DISPLAY_H

#include "audio.h"
#include "style.h"

void display_audio_information(audio* audio, ma_engine* engine, bool* loop, float* pitch, style* style);
void display_all_audios(audio* audios, audio** current_audio, int count, bool* loop, float* pitch, style* style);

#endif