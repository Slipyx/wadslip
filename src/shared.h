/*
** shared.h
**
** Functions and types that are shared by all.
*/

#ifndef __SHARED_H
#define __SHARED_H

#include <stdlib.h>
#include <stdio.h>
#include "wad_types.h" // stdint.h is here so we wont include it again
#include "iniparser/iniparser.h"

// General color object
typedef struct {
    uint8_t r, g, b;
} color_t;

#endif
