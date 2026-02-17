#include "includes.h"
#include "registry.h"

#ifdef EMBER_PLATFORM_WINDOWS

#define PROGID "EmberC.AudioFile"
#define PROGID_DESC "EmberC Audio File"

static bool registry_set_key_value(HKEY root, const char* subkey, const char* value_name, const char* value_data)
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

static bool registry_create_progid(const char* exe_path)
{
    char buffer[1024];

    if (!registry_set_key_value(HKEY_CURRENT_USER,
                                "Software\\Classes\\" PROGID,
                                NULL,
                                PROGID_DESC))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\DefaultIcon", PROGID);
    char icon_path[1024];
    snprintf(icon_path, sizeof(icon_path), "\"%s\",0", exe_path);

    if (!registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, icon_path))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\shell\\open\\command", PROGID);
    char command[1024];
    snprintf(command, sizeof(command), "\"%s\" \"%%1\"", exe_path);

    if (!registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, command))
    {
        return false;
    }

    return true;
}

static bool registry_associate_extension(const char* extension)
{
    char buffer[512];

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s", extension);
    if (!registry_set_key_value(HKEY_CURRENT_USER, buffer, NULL, PROGID))
    {
        return false;
    }

    snprintf(buffer, sizeof(buffer), "Software\\Classes\\%s\\OpenWithProgids", extension);
    if (!registry_set_key_value(HKEY_CURRENT_USER, buffer, PROGID, ""))
    {
        return false;
    }

    return true;
}

bool registry_is_registered(void)
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

bool registry_register_file_associations(const char* exe_path)
{
    if (!exe_path)
    {
        log_error_s("Invalid exe path for registration");
        return false;
    }

    log_info_s("Registering EmberC file associations...");

    // Create ProgID
    if (!registry_create_progid(exe_path))
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