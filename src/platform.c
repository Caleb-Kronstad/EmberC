#include "includes.h"
#include "platform.h"


// WINDOWS
#ifdef EMBER_PLATFORM_WINDOWS

#define PROGID "EmberC.AudioFile"
#define PROGID_DESC "EmberC Audio File"

void windows_initialize(int argc, char* argv[], enum startup_mode* mode, char* target_file_path, size_t target_file_path_size, bool* show_registration_prompt)
{
    if (argc > 1)
    {
        char* normalized = windows_file_manager_normalize_path(argv[1]);
        if (normalized)
        {
            if (windows_file_manager_is_valid_audio_file(normalized) &&
                windows_file_manager_file_exists(normalized))
            {
                *mode = MODE_SINGLE_FILE;
                strncpy(target_file_path, normalized, target_file_path_size - 1);
                target_file_path[target_file_path_size - 1] = '\0';
                log_info_s("Opening file from command line");
            }
            else
            {
                log_error_s("Invalid audio file provided, falling back to directory mode");
            }
            free(normalized);
        }
    }
    else
    {
        if (!windows_registry_is_registered())
        {
            *show_registration_prompt = true;
        }
    }
    
    char exe_dir[1024];
    if (GetModuleFileNameA(NULL, exe_dir, sizeof(exe_dir)) != 0)
    {
        char* last_slash = strrchr(exe_dir, '\\');
        if (last_slash)
        {
            *last_slash = '\0';
            SetCurrentDirectoryA(exe_dir);
            log_info_s("Set working directory to exe location");
        }
    }
}

void windows_registration_prompt(bool* show_registration_prompt)
{
    if (!*show_registration_prompt) return;
    
    igOpenPopup_Str("RegisterEmberC", 0);
    *show_registration_prompt = false;

    if (igBeginPopupModal("RegisterEmberC", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        igIndent(8.0f);
        
        igSpacing();
        igSpacing();
        igSpacing();
        igSpacing();
        
        igText("Would you like to register EmberC as your default audio player?");
        igText("This will associate .mp3, .wav, and .ogg files with EmberC.");
        
        igSpacing();
        igSpacing();
        igSpacing();
        igSpacing();

        if (igButton("Yes, Register", (ImVec2_c){120, 0})) {
            char exe_path[1024];
            GetModuleFileNameA(NULL, exe_path, sizeof(exe_path));

            if (windows_registry_register_file_associations(exe_path)) {
                log_info_s("EmberC registered successfully");
            } else {
                log_error_s("Failed to register EmberC - try running as administrator");
            }

            igCloseCurrentPopup();
        }

        igSameLine(0, -1);

        if (igButton("No, Skip", (ImVec2_c){120, 0})) {
            igCloseCurrentPopup();
        }
        
        igSpacing();
        igSpacing();
        
        igUnindent(8.0f);
        igEndPopup();
    }
}

bool windows_file_manager_is_valid_audio_file(const char* path)
{
    if (!path) return false;

    size_t len = strlen(path);
    if (len < 5) return false;

    const char* ext = path + len - 4;
    return (_stricmp(ext, ".mp3") == 0 ||
        _stricmp(ext, ".wav") == 0 ||
        _stricmp(ext, ".ogg") == 0);
}

char* windows_file_manager_normalize_path(const char* raw_path)
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

bool windows_file_manager_file_exists(const char* path)
{
    if (!path) return false;

    DWORD attrib = GetFileAttributesA(path);
    return (attrib != INVALID_FILE_ATTRIBUTES &&
        !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

void windows_file_manager_extract_filename(const char* path, char* out, size_t out_size)
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

void windows_file_manager_extract_parent_directory(const char* path, char* out, size_t out_size)
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

static bool windows_registry_set_key_value(HKEY root, const char* subkey, const char* value_name, const char* value_data)
{
    HKEY key;
    LONG result = RegCreateKeyExA(root, subkey, 0, NULL, 0, KEY_WRITE, NULL, &key, NULL);

    if (result != ERROR_SUCCESS)
    {
        log_error_s("Failed to create registry key");
        return false;
    }

    result = RegSetValueExA(key, value_name, 0, REG_SZ,
                            (const BYTE*)value_data, (DWORD)(strlen(value_data) + 1));

    RegCloseKey(key);

    if (result != ERROR_SUCCESS)
    {
        log_error_s("Failed to set registry value");
        return false;
    }

    return true;
}

static bool windows_registry_create_progid(const char* exe_path)
{
    char buffer[1024];

    if (!windows_registry_set_key_value(HKEY_CURRENT_USER,
                                "Software\\Classes\\" PROGID,
                                NULL,
                                PROGID_DESC))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\DefaultIcon", PROGID);
    char icon_path[1024];
    snprintf(icon_path, sizeof(icon_path), "\"%s\",0", exe_path);

    if (!windows_registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, icon_path))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\shell\\open\\command", PROGID);
    char command[1024];
    snprintf(command, sizeof(command), "\"%s\" \"%%1\"", exe_path);

    if (!windows_registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, command))
    {
        return false;
    }

    return true;
}

static bool registry_associate_extension(const char* extension)
{
    char buffer[512];

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s", extension);
    if (!windows_registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, PROGID))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\OpenWithProgids", extension);
    if (!windows_registry_set_key_value(HKEY_CURRENT_USER, buffer, PROGID, ""))
    {
        return false;
    }

    return true;
}

bool windows_registry_is_registered(void)
{
    HKEY key;
    LONG result = RegOpenKeyExA(HKEY_CURRENT_USER,
                                "Software\\Classes\\" PROGID,
                                0,
                                KEY_READ,
                                &key);

    if (result == ERROR_SUCCESS)
    {
        RegCloseKey(key);
        return true;
    }

    return false;
}

bool windows_registry_register_file_associations(const char* exe_path)
{
    if (!exe_path)
    {
        log_error_s("Invalid exe path for registration");
        return false;
    }

    log_info_s("Registering EmberC file associations...");

    // Create ProgID
    if (!windows_registry_create_progid(exe_path))
    {
        log_error_s("Failed to create ProgID");
        return false;
    }

    const char* extensions[] = {".mp3", ".wav", ".ogg"};
    for (int i = 0; i < 3; i++)
    {
        if (!registry_associate_extension(extensions[i]))
        {
            log_error_s("Failed to associate extension");
            return false;
        }
    }

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    log_info_s("EmberC registered successfully");
    return true;
}

bool registry_unregister_file_associations(void)
{
    log_info_s("Unregistering EmberC file associations...");

    RegDeleteTreeA(HKEY_CURRENT_USER, "Software\\Classes\\" PROGID);

    const char* extensions[] = {".mp3", ".wav", ".ogg"};
    for (int i = 0; i < 3; i++)
    {
        char buffer[512];
        snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s", extensions[i]);

        HKEY key;
        if (RegOpenKeyExA(HKEY_CURRENT_USER, buffer, 0, KEY_READ | KEY_WRITE, &key) == ERROR_SUCCESS)
        {
            char value[256];
            DWORD value_size = sizeof(value);

            if (RegQueryValueExA(key, NULL, NULL, NULL, (BYTE*)value, &value_size) == ERROR_SUCCESS)
            {
                if (strcmp(value, PROGID) == 0)
                {
                    RegDeleteValueA(key, NULL);
                }
            }

            RegCloseKey(key);
        }

        snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\OpenWithProgids", extensions[i]);
        if (RegOpenKeyExA(HKEY_CURRENT_USER, buffer, 0, KEY_WRITE, &key) == ERROR_SUCCESS)
        {
            RegDeleteValueA(key, PROGID);
            RegCloseKey(key);
        }
    }

    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);

    log_info_s("EmberC unregistered");
    return true;
}

#endif


// LINUX
#ifdef EMBER_PLATFORM_LINUX

void linux_initialize(int argc, char* argv[], enum startup_mode* mode, char* target_file_path, size_t target_file_path_size)
{
    if (argc > 1)
    {
        char* normalized = linux_file_manager_normalize_path(argv[1]);
        if (normalized)
        {
            if (linux_file_manager_is_valid_audio_file(normalized) &&
                linux_file_manager_file_exists(normalized))
            {
                *mode = MODE_SINGLE_FILE;
                strncpy(target_file_path, normalized, target_file_path_size - 1);
                target_file_path[target_file_path_size - 1] = '\0';
                log_info_s("Opening file from command line");
            }
            else
            {
                log_error_s("Invalid audio file provided, falling back to directory mode");
            }
            free(normalized);
        }
    }

    char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
        char* last_slash = strrchr(exe_path, '/');
        if (last_slash)
        {
            *last_slash = '\0';

            char assets_path[PATH_MAX];
            snprintf(assets_path, sizeof(assets_path), "%s/assets", exe_path);
            struct stat st;
            if (stat(assets_path, &st) == 0 && S_ISDIR(st.st_mode))
            {
                if (chdir(exe_path) == 0)
                {
                    log_info_s("Set working directory to exe location");
                }
            }
        }
    }
}

bool linux_file_manager_is_valid_audio_file(const char* path)
{
    if (!path) return false;

    size_t len = strlen(path);
    if (len < 5) return false;

    const char* ext = path + len - 4;
    return (strcasecmp(ext, ".mp3") == 0 ||
        strcasecmp(ext, ".wav") == 0 ||
        strcasecmp(ext, ".ogg") == 0);
}

char* linux_file_manager_normalize_path(const char* raw_path)
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

    char* absolute = (char*)malloc(PATH_MAX);
    if (!absolute)
    {
        free(temp);
        return NULL;
    }

    if (!realpath(temp, absolute))
    {
        free(temp);
        free(absolute);
        return NULL;
    }

    free(temp);
    return absolute;
}

bool linux_file_manager_file_exists(const char* path)
{
    if (!path) return false;

    struct stat st;
    return (stat(path, &st) == 0 && S_ISREG(st.st_mode));
}

void linux_file_manager_extract_filename(const char* path, char* out, size_t out_size)
{
    if (!path || !out || out_size == 0) return;

    const char* last_slash = strrchr(path, '/');
    const char* filename = last_slash ? last_slash + 1 : path;

    const char* dot = strrchr(filename, '.');
    size_t name_len = dot ? (size_t)(dot - filename) : strlen(filename);

    size_t copy_len = name_len < (out_size - 1) ? name_len : (out_size - 1);
    strncpy(out, filename, copy_len);
    out[copy_len] = '\0';
}

void linux_file_manager_extract_parent_directory(const char* path, char* out, size_t out_size)
{
    if (!path || !out || out_size == 0) return;

    const char* last_slash = strrchr(path, '/');

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