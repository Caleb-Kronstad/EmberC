#ifndef EMBER_PLATFORM_H
#define EMBER_PLATFORM_H

enum startup_mode { MODE_DIRECTORY_SCAN, MODE_SINGLE_FILE };

#ifdef EMBER_PLATFORM_WINDOWS

void windows_initialize(int argc, char* argv[], enum startup_mode* mode, char* target_file_path, size_t target_file_path_size, bool* show_registration_prompt);
void windows_registration_prompt(bool* show_registration_prompt);

bool windows_file_manager_is_valid_audio_file(const char* path);
char* windows_file_manager_normalize_path(const char* raw_path);
bool windows_file_manager_file_exists(const char* path);
void windows_file_manager_extract_filename(const char* path, char* out, size_t out_size);
void windows_file_manager_extract_parent_directory(const char* path, char* out, size_t out_size);

bool windows_registry_is_registered(void);
bool windows_registry_register_file_associations(const char* exe_path);
bool windows_registry_unregister_file_associations(void);

#endif

#ifdef EMBER_PLATFORM_LINUX

void linux_initialize(int argc, char* argv[], enum startup_mode* mode, char* target_file_path, size_t target_file_path_size);

bool linux_file_manager_is_valid_audio_file(const char* path);
char* linux_file_manager_normalize_path(const char* raw_path);
bool linux_file_manager_file_exists(const char* path);
void linux_file_manager_extract_filename(const char* path, char* out, size_t out_size);
void linux_file_manager_extract_parent_directory(const char* path, char* out, size_t out_size);

#endif

#endif