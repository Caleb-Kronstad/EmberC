#ifndef EMBER_INCLUDES_H
#define EMBER_INCLUDES_H

// UNIVERSAL INCLUDES //
#include "log.h"
//

// C INCLUDES //
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
//

// PLATFORM SPECIFIC INCLUDES //
#ifdef EMBER_PLATFORM_WINDOWS
#include <windows.h>
#endif

// DEPENDENCY INCLUDES //
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cimgui.h>
#include <cimgui_impl.h>
#include <miniaudio.h>
#include <cJSON.h>
//

#endif