#ifndef EMBER_DISPLAY_H
#define EMBER_DISPLAY_H

#include "audio.h"
#include "image.h"
#include "style.h"

void display_audio_information(audio* audio, ma_engine* engine, bool* loop, float* pitch, style* style, image* logo);
void display_all_audios(audio* audios, audio** current_audio, int count, bool* loop, float* pitch, style* style);

#endif