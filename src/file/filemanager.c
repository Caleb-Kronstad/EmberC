#include "includes.h"
#include "filemanager.h"

#ifdef EMBER_PLATFORM_WINDOWS

bool file_manager_is_valid_audio_file(const char* path)
{
    if (!path) return false;

    size_t len = strlen(path);
    if (len < 5) return false;

    const char* ext = path + len - 4;
    return (_stricmp(ext, ".mp3") == 0 ||
        _stricmp(ext, ".wav") == 0 ||
        _stricmp(ext, ".ogg") == 0);
}

char* file_manager_normalize_path(const char* raw_path)
{
    if (!raw_path) return NULL;
    size_t len = strlen(raw_path);
    if (len == 0) return NULL;

    char* temp = (char*)malloc(len + 1);
    if (!temp) return NULL;

    strcpy(temp, raw_path);

    if (temp[0] == '"' && temp[len - 1] == '"')
    {
        temp[len - 1] = '\0';
        memmove(temp, temp + 1, len - 1);
        len -= 2;
    }

    for (size_t i = 0; i < len; i++)
    {
        if (temp[i] == '/')
        {
            temp[i] = '\\';
        }
    }

    char* absolute = (char*)malloc(MAX_PATH);
    if (!absolute)
    {
        free(temp);
        return NULL;
    }

    DWORD result = GetFullPathNameA(temp, MAX_PATH, absolute, NULL);
    free(temp);

    if (result == 0 || result >= MAX_PATH)
    {
        free(absolute);
        return NULL;
    }

    return absolute;
}

bool file_manager_file_exists(const char* path)
{
    if (!path) return false;

    DWORD attrib = GetFileAttributesA(path);
    return (attrib != INVALID_FILE_ATTRIBUTES &&
        !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

void file_manager_extract_filename(const char* path, char* out, size_t out_size)
{
    if (!path || !out || out_size == 0) return;

    const char* last_slash = strrchr(path, '\\');
    if (!last_slash)
    {
        last_slash = strrchr(path, '/');
    }

    const char* filename = last_slash ? last_slash + 1 : path;

    const char* dot = strrchr(filename, '.');
    size_t name_len = dot ? (size_t)(dot - filename) : strlen(filename);

    size_t copy_len = name_len < (out_size - 1) ? name_len : (out_size - 1);
    strncpy(out, filename, copy_len);
    out[copy_len] = '\0';
}

void file_manager_extract_parent_directory(const char* path, char* out, size_t out_size)
{
    if (!path || !out || out_size == 0) return;

    const char* last_slash = strrchr(path, '\\');
    if (!last_slash)
    {
        last_slash = strrchr(path, '/');
    }

    if (!last_slash)
    {
        out[0] = '\0';
        return;
    }

    size_t dir_len = (size_t)(last_slash - path);
    size_t copy_len = dir_len < (out_size - 1) ? dir_len : (out_size - 1);

    strncpy(out, path, copy_len);
    out[copy_len] = '\0';
}

#endif