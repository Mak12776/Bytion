#ifndef _DISPLAY_HEADER_
#define _DISPLAY_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "units.h"

#include "error_macros.h"
#include "terminal_color_mac.h"
#include "terminal_color_if.h"

extern void ReadDisplayFile(const char *filename);
extern void ReadDisplayStrings(const char *filename);

#endif
