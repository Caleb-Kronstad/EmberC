#include "includes.h"
#include "persist.h"

#define EMBER_DEFAULT_VOLUME 50.0f
#define EMBER_MAX_VOLUME 100.0f
#define EMBER_MIN_VOLUME 0.0f

#define EMBER_DEFAULT_PITCH 50.0f
#define EMBER_MAX_PITCH 100.0f
#define EMBER_MIN_PITCH 0.0f

#define EMBER_DEFAULT_LOOP false

#define EMBER_DEFAULT_WINDOW_WIDTH 800
#define EMBER_DEFAULT_WINDOW_HEIGHT 600
#define EMBER_DEFAULT_FULLSCREEN false

ember_config config_initialize_defaults(void)
{
    ember_config config;
    config.volume = EMBER_DEFAULT_VOLUME;
    config.pitch = EMBER_DEFAULT_PITCH;
    config.loop = EMBER_DEFAULT_LOOP; 
    config.window_width = EMBER_DEFAULT_WINDOW_WIDTH;
    config.window_height = EMBER_DEFAULT_WINDOW_HEIGHT;
    config.fullscreen = EMBER_DEFAULT_FULLSCREEN;
    return config;
}

bool config_load(ember_config* config, const char* filepath)
{
    FILE* file = fopen(filepath, "rb");
    if (!file)
    {
        log_error_s("Failed to open file");
        return false;
    }
    
    int seek_value = 0;
    seek_value = fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    seek_value = fseek(file, 0, SEEK_SET);
    
    char* buffer = (char*)malloc(file_size + 1);
    if (!buffer)
    {
        log_error_s("Failed to allocate memory buffer for config file");
        return false;
    }
    
    size_t read_size = fread(buffer, 1, file_size, file);
    buffer[read_size] = '\0';
    int close_value = fclose(file);
    
    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    
    if (!json)
    {
        log_error_s("Failed to parse config file");
        return false;
    }
    
    cJSON* volume_item = cJSON_GetObjectItem(json, "volume");
    if (cJSON_IsNumber(volume_item))
    {
        config->volume = (float)volume_item->valuedouble;
        if (config->volume < EMBER_MIN_VOLUME) config->volume = EMBER_MIN_VOLUME;
        if (config->volume > EMBER_MAX_VOLUME) config->volume = EMBER_MAX_VOLUME;
    }
    
    cJSON* pitch_item = cJSON_GetObjectItem(json, "pitch");
    if (cJSON_IsNumber(pitch_item))
    {
        config->pitch = (float)pitch_item->valuedouble;
        if (config->pitch < EMBER_MIN_PITCH) config->pitch = EMBER_MIN_PITCH;
        if (config->pitch > EMBER_MAX_PITCH) config->pitch = EMBER_MAX_PITCH;
    }
    
    cJSON* loop_item = cJSON_GetObjectItem(json, "loop");
    if (cJSON_IsBool(loop_item))
    {
        config->loop = loop_item->valueint;
    }
    
    cJSON* window_width_item = cJSON_GetObjectItem(json, "window_width");
    if (cJSON_IsNumber(window_width_item))
    {
        config->window_width = window_width_item->valueint;
        if (config->window_width < 0) config->window_width = 0;
    }
    
    cJSON* window_height_item = cJSON_GetObjectItem(json, "window_height");
    if (cJSON_IsNumber(window_height_item))
    {
        config->window_height = window_height_item->valueint;
        if (config->window_height < 0) config->window_height = 0;
    }
    
    cJSON* fullscreen_item = cJSON_GetObjectItem(json, "fullscreen");
    if (cJSON_IsBool(fullscreen_item))
    {
        config->fullscreen = fullscreen_item->valueint;
    }
    
    cJSON_Delete(json);
    return true;
}

bool config_save(const ember_config* config, const char* filepath)
{
    cJSON* json = cJSON_CreateObject();
    if (!json)
    {
        log_error_s("Failed to create config JSON object");
        return false;
    }
    
    cJSON_AddNumberToObject(json, "volume", config->volume);
    cJSON_AddNumberToObject(json, "pitch", config->pitch);
    cJSON_AddBoolToObject(json, "loop", config->loop);
    cJSON_AddNumberToObject(json, "window_width", config->window_width);
    cJSON_AddNumberToObject(json, "window_height", config->window_height);
    cJSON_AddBoolToObject(json, "fullscreen", config->fullscreen);
    
    char* json_string = cJSON_Print(json);
    if (!json_string)
    {
        log_error_s("Failed to serialize config JSON string");
        cJSON_Delete(json);
        return false;
    }
    
    FILE* file = fopen(filepath, "wb");
    if (!file)
    {
        log_error_s("Failed to open config file for writing");
        cJSON_Delete(json);
        return false;
    }
    
    size_t json_length = strlen(json_string);
    size_t written = fwrite(json_string, 1, json_length, file);
    
    int close_value = fclose(file);
    cJSON_free(json_string);
    cJSON_Delete(json);
    
    if (written != json_length)
    {
        log_error_s("Failed to write full config file");
        return false;
    }
    
    return true;
}