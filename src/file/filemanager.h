#ifndef EMBER_FILE_MANAGER_H
#define EMBER_FILE_MANAGER_H

#ifdef EMBER_PLATFORM_WINDOWS

bool file_manager_is_valid_audio_file(const char* path);
char* file_manager_normalize_path(const char* raw_path);
bool file_manager_file_exists(const char* path);
void file_manager_extract_filename(const char* path, char* out, size_t out_size);
void file_manager_extract_parent_directory(const char* path, char* out, size_t out_size);

#endif
#endif