#include "includes.h"
#include "ember.h"

#include "math.h"
#include "audio.h"

ma_engine* initaudioengine(ma_engine_config* config)
{
    ma_engine* engine = malloc(sizeof(ma_engine));
    *config = ma_engine_config_init();
    ma_result result = ma_engine_init(config, engine);
    if (result != MA_SUCCESS)
    {
        logerrors("Failed to initialize audio engine");
        free(engine);
        return NULL;
    }

    ma_engine_set_volume(engine, 0.5f);
    return engine;
}

void shutdownaudioengine(ma_engine* engine, audio** audios, int count)
{
    for (int i = 0; i < count; i++)
    {
        if ((*audios)[i].sound)
        {
            ma_sound_uninit((*audios)[i].sound);
            free((*audios)[i].sound);
        }
    }
    
    ma_engine_uninit(engine);
    free(engine);
}

void setvolume(ma_engine* engine, float volume)
{
    ma_engine_set_volume(engine, volume / 100.0f);
}
float getvolume(ma_engine* engine)
{
    return ma_engine_get_volume(engine) * 100.0f;
}

void formattime(float seconds, char* buffer, size_t buffer_size)
{
    int total_seconds = (int)seconds;
    int minutes = total_seconds / 60;
    int secs = total_seconds % 60;
    (void)snprintf(buffer, buffer_size, "%d:%02d", minutes, secs);
}