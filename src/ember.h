#ifndef EMBER_H
#define EMBER_H

#include "audio.h"

ma_engine* initaudioengine(ma_engine_config* config);
void shutdownaudioengine(ma_engine* engine, audio** audios, int count);
void setvolume(ma_engine* engine, float volume);
float getvolume(ma_engine* engine);

#endif