#ifndef EMBER_LOG_H
#define EMBER_LOG_H

#include <stdio.h>

static inline void log_info_s(const char* msg)
{
    printf("\x1b[37m [INFO] %s\x1b[37m\n", msg);
}
static inline void log_info_i(const int* msg)
{
    printf("\x1b[37m [INFO] %d\x1b[37m\n", *msg);
}
static inline void log_info_f(const float* msg)
{
    printf("\x1b[37m [INFO] %f\x1b[37m\n", *msg);
}
static inline void log_info_d(const double* msg)
{
    printf("\x1b[37m [INFO] %lf\x1b[37m\n", *msg);
}

static inline void log_warning_s(const char* msg)
{
    printf("\x1b[33m [WARNING] %s\x1b[37m\n", msg);
}
static inline void log_warning_i(const int* msg)
{
    printf("\x1b[33m [WARNING] %d\x1b[37m\n", *msg);
}
static inline void log_warning_f(const float* msg)
{
    printf("\x1b[33m [WARNING] %f\x1b[37m\n", *msg);
}
static inline void log_warning_d(const double* msg)
{
    printf("\x1b[33m [WARNING] %lf\x1b[37m\n", *msg);
}

static inline void log_error_s(const char* msg)
{
    printf("\x1b[31m [ERROR] %s\x1b[37m\n", msg);
}
static inline void log_error_i(const int* msg)
{
    printf("\x1b[31m [ERROR] %d\x1b[37m\n", *msg);
}
static inline void log_error_f(const float* msg)
{
    printf("\x1b[31m [ERROR] %f\x1b[37m\n", *msg);
}
static inline void log_error_d(const double* msg)
{
    printf("\x1b[31m [ERROR] %lf\x1b[37m\n", *msg);
}

#endif