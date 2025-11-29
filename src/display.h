#ifndef EMBER_DISPLAY_H
#define EMBER_DISPLAY_H

#include "audio.h"

void displayaudioinformation(audio* audio, ma_engine* engine, bool* loop);
void displayallaudios(audio* audios, audio** current_audio, int count, bool* loop);

#endif