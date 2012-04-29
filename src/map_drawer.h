/*
** map_drawer.h
**
** Functions for drawing a map.
*/

#ifndef __MAP_DRAWER_H
#define __MAP_DRAWER_H

#include "shared.h"

void DrawPalette( color_t* pal );
void DrawMap( map_t* map );

// Global configuration file
extern dictionary* ini;

#endif
