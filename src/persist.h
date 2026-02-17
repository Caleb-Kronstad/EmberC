#ifndef EMBER_CONFIG_H
#define EMBER_CONFIG_H

#include <stdbool.h>

typedef struct ember_config
{
    float volume;
    float pitch;
    bool loop;
    
    int window_width;
    int window_height;
    bool fullscreen;
} ember_config;

ember_config config_initialize_defaults(void);
bool config_load(ember_config* config, const char* filepath);
bool config_save(const ember_config* config, const char* filepath);

#endif 