#ifndef EMBER_LOG_H
#define EMBER_LOG_H

#include <stdio.h>

inline void loginfos(const char* msg)
{
    printf("\x1b[37m [INFO] %s\x1b[37m\n", msg);
}
inline void loginfoi(const int* msg)
{
    printf("\x1b[37m [INFO] %d\x1b[37m\n", *msg);
}
inline void loginfof(const float* msg)
{
    printf("\x1b[37m [INFO] %f\x1b[37m\n", *msg);
}
inline void loginfod(const double* msg)
{
    printf("\x1b[37m [INFO] %lf\x1b[37m\n", *msg);
}

inline void logwarnings(const char* msg)
{
    printf("\x1b[33m [WARNING] %s\x1b[37m\n", msg);
}
inline void logwarningi(const int* msg)
{
    printf("\x1b[33m [WARNING] %d\x1b[37m\n", *msg);
}
inline void logwarningf(const float* msg)
{
    printf("\x1b[33m [WARNING] %f\x1b[37m\n", *msg);
}
inline void logwarningd(const double* msg)
{
    printf("\x1b[33m [WARNING] %lf\x1b[37m\n", *msg);
}

inline void logerrors(const char* msg)
{
    printf("\x1b[31m [ERROR] %s\x1b[37m\n", msg);
}
inline void logerrori(const int* msg)
{
    printf("\x1b[31m [ERROR] %d\x1b[37m\n", *msg);
}
inline void logerrorf(const float* msg)
{
    printf("\x1b[31m [ERROR] %f\x1b[37m\n", *msg);
}
inline void logerrord(const double* msg)
{
    printf("\x1b[31m [ERROR] %lf\x1b[37m\n", *msg);
}

#endif