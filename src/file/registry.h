#ifndef EMBER_REGISTRY_H
#define EMBER_REGISTRY_H

#ifdef EMBER_PLATFORM_WINDOWS

bool registry_is_registered(void);
bool registry_register_file_associations(const char* exe_path);
bool registry_unregister_file_associations(void);

#endif
#endif