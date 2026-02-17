#include "includes.h"
#include "ember.h"

#include "math.h"
#include "audio.h"

ma_engine* audio_engine_initialize(ma_engine_config* config)
{
    ma_engine* engine = malloc(sizeof(ma_engine));
    *config = ma_engine_config_init();
    ma_result result = ma_engine_init(config, engine);
    if (result != MA_SUCCESS)
    {
        log_error_s("Failed to initialize audio engine");
        free(engine);
        return NULL;
    }

    ma_engine_set_volume(engine, 0.5f);
    return engine;
}

void audio_engine_shutdown(ma_engine* engine, audio** audios, int count)
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

void volume_set(ma_engine* engine, float volume)
{
    ma_engine_set_volume(engine, volume / 100.0f);
}
float volume_get(ma_engine* engine)
{
    return ma_engine_get_volume(engine) * 100.0f;
}
void pitch_set(audio* audio, float pitch)
{
    ma_sound_set_pitch(audio->sound, pitch / 50.0f);
}
float pitch_get(audio* audio)
{
    return ma_sound_get_pitch(audio->sound) * 50.0f;
}
void loop_set(audio* audio, bool loop)
{
    ma_sound_set_looping(audio->sound, loop);
}
bool loop_get(audio* audio)
{
    return ma_sound_is_looping(audio->sound) == MA_TRUE;
}