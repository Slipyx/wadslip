/*
** map_drawer.c
**
** Functions for drawing a map.
*/

#include "map_drawer.h"
#include <cairo/cairo-svg.h>
#include "thing_counter.h"

// Convert 255 based color to 1.0 based color
#define NORM_COLOR(c) c.r / 255.0, c.g / 255.0, c.b / 255.0

// Array of trigger linedef numbers
static const int16_t trigLineNums[] = {
    // Doors
    1, 2, 3, 4, 16, 29, 31, 42, 46, 50, 61, 63, 75, 76, 86, 90, 103, 105, 106,
    107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 175, 196,
    // Floors
    5, 18, 19, 23, 24, 30, 36, 37, 38, 45, 55, 56, 58, 59, 60, 64, 65, 69, 70,
    71, 78, 82, 83, 84, 91, 92, 93, 94, 96, 98, 101, 102, 119, 128, 129, 130,
    131, 132, 140, 142, 147, 153, 154, 158, 159, 160, 161, 176, 177, 178, 179,
    180, 189, 190, 219, 220, 221, 222, 239, 240, 241,
    // Ceilings
    40, 41, 43, 44, 72, 145, 151, 152, 166, 167, 186, 187, 199, 200, 201, 202,
    203, 204, 205, 206,
    // Platforms
    10, 14, 15, 20, 21, 22, 47, 53, 54, 62, 66, 67, 68, 87, 88, 89, 95, 120, 121,
    122, 123, 143, 144, 148, 149, 162, 163, 181, 182, 211, 212,
    // Crushers
    6, 25, 49, 57, 73, 74, 77, 141, 150, 164, 165, 168, 183, 184, 185, 188,
    // Stair builders
    7, 8, 100, 127, 256, 257, 258, 259,
    // Elevators
    227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
    // Lighting
    12, 13, 17, 35, 79, 80, 81, 104, 138, 139, 156, 157, 169, 170, 171, 172,
    173, 192, 193, 194,
    // Exits
    11, 51, 52, 124, 197, 198,
    // Teleporters
    39, 97, 125, 126, 174, 195, 207, 208, 209, 210, 243, 244, 262, 263, 264, 265,
    266, 267, 268, 269,
    // Donuts
    9, 146, 155, 191
};

void DrawPalette( color_t* pal ) {
    cairo_surface_t* surface = NULL;
    cairo_t* cr = NULL;
    uint16_t i = 0;

    surface = cairo_image_surface_create( CAIRO_FORMAT_RGB24, 16, 16 );
    cr = cairo_create( surface );
    cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE );

    for ( i = 0; i < 256; ++i ) {
        cairo_set_source_rgb( cr, NORM_COLOR(pal[i]) );
        cairo_rectangle( cr, i % 16, i / 16, 1.0, 1.0 );
        cairo_fill( cr );
    }

    // Write and cleanup
    cairo_surface_write_to_png( surface, "pal.png" );
    cairo_destroy( cr );
    cairo_surface_destroy( surface );
}

void DrawMap( map_t* map ) {
    cairo_surface_t* surface = NULL;
    cairo_t* cr = NULL;
    uint16_t surfaceW = (uint16_t)iniparser_getint( ini, "MapDrawer:maxSize", 1024 ),
             surfaceH = surfaceW;
    double scale = 1.0;
    uint32_t i = 0;
    color_t bgColor = {255, 255, 255};
    color_t wallColor = {0, 0, 0}; // Normal wall color
    color_t fdColor = {128, 128, 128}; // Floor difference
    color_t cdColor = {128, 128, 128}; // Ceiling difference
    color_t shColor = {128, 128, 128}; // Same height
    color_t trigColor = {224, 112, 0};
    // Boolean variables
    uint8_t drawThings = (uint8_t)iniparser_getboolean( ini, "MapDrawer:drawThings", 0 ),
            countThings = (uint8_t)iniparser_getboolean( ini, "MapDrawer:countThings", 0 );

    // Print info
    printf( "Name: %.8s\nDimensions: %ux%u\nThings: %d\nLinedefs: %d\n"
            "Sidedefs: %d\nVertexes: %d\nSectors: %d\n\n", map->name, map->width,
            map->height, map->numthings, map->numlinedefs, map->numsidedefs,
            map->numvertexes, map->numsectors );

    if ( map->width > map->height ) {
        surfaceH = (surfaceW * map->height) / map->width;
        scale = (double)(surfaceW - 8) / map->width;
    } else if ( map->height > map->width ) {
        surfaceW = (surfaceH * map->width) / map->height;
        scale = (double)(surfaceH - 8) / map->height;
    } else {
        scale = (double)(surfaceW - 8) / map->width;
    }
    surface = cairo_svg_surface_create( "map.svg", surfaceW, surfaceH );
    cr = cairo_create( surface );
    cairo_set_source_rgb( cr, NORM_COLOR(bgColor) );
    cairo_paint( cr );
    if ( !iniparser_getboolean( ini, "MapDrawer:antiAlias", 1 ) ) {
        cairo_set_antialias( cr, CAIRO_ANTIALIAS_NONE );
    }
    cairo_set_line_width( cr, iniparser_getdouble( ini, "MapDrawer:lineWidth", 2.0 ) );

    cairo_translate( cr, surfaceW / 2.0, surfaceH / 2.0 );
    // Draw the map's lines
    for ( i = 0; i < map->numlinedefs; ++i ) {
        vertex_t v1 = map->vertexes[map->linedefs[i].v1];
        vertex_t v2 = map->vertexes[map->linedefs[i].v2];
        linedef_t linedef = map->linedefs[i];
        uint16_t j = 0;

        // Offset to 0, 0
        v1.x += -map->centerv.x; v1.y += -map->centerv.y;
        v2.x += -map->centerv.x; v2.y += -map->centerv.y;

        // Default is regular solid wall
        cairo_set_source_rgb( cr, NORM_COLOR(wallColor) );

        // Check for two sided
        if ( linedef.sidenum[1] >= 0 ) {
            sector_t frontsector = map->sectors[map->sidedefs[linedef.sidenum[0]].sectornum];
            sector_t backsector = map->sectors[map->sidedefs[linedef.sidenum[1]].sectornum];
            if ( frontsector.floorheight != backsector.floorheight ) { // Floor difference
                cairo_set_source_rgb( cr, NORM_COLOR(fdColor) );
            } else if ( frontsector.ceilingheight != backsector.ceilingheight ) { // Ceiling difference
                cairo_set_source_rgb( cr, NORM_COLOR(cdColor) );
            } else { // Same height
                cairo_set_source_rgb( cr, NORM_COLOR(shColor) );
            }
        }

        // Check for triggers
        for ( j = 0; j < sizeof(trigLineNums) / sizeof(int16_t); ++j ) {
            if ( linedef.special == trigLineNums[j] ) {
                cairo_set_source_rgb( cr, NORM_COLOR(trigColor) );
                break;
            }
        }

        // Check for locked doors
        switch ( linedef.special ) {
            // Red door
            case 28: case 33: case 134: case 135:
                cairo_set_source_rgb( cr, 1.0, 0.0, 0.0 );
                break;
            // Blue door
            case 26: case 32: case 99: case 133:
                cairo_set_source_rgb( cr, 0.0, 0.0, 1.0 );
                break;
            // Yellow door
            case 27: case 34: case 136: case 137:
                cairo_set_source_rgb( cr, 1.0, 0.9, 0.0 );
                break;
            default:
                break;
        }

        cairo_move_to( cr, v1.x * scale, -(v1.y * scale) );
        cairo_line_to( cr, v2.x * scale, -(v2.y * scale) );
        cairo_stroke( cr );
    }
    // Draw and count the map's things
    if ( countThings || drawThings ) {
        for ( i = 0; i < map->numthings; ++i ) {
            int16_t type = map->things[i].type;
            // Count thing
            if ( countThings ) {
                CountThing( type, map->things[i].flags );
            }
            // Draw thing
            if ( drawThings ) {
                int16_t x = map->things[i].x += -map->centerv.x;
                int16_t y = map->things[i].y += -map->centerv.y;
                // Player 1 start (16 radius)
                if ( type == 1 ) {
                    cairo_set_source_rgb( cr, 0.0, 1.0, 0.0 );
                    cairo_rectangle( cr, (x * scale) - (16 * scale),
                                     -((y * scale) + (16 * scale)),
                                     32 * scale, 32 * scale );
                    cairo_fill( cr );
                // Keys (20 radius)
                } else if ( type == 5 || type == 40 ) { // Blue card / skull
                    cairo_set_source_rgb( cr, 0.0, 0.0, 1.0 );
                    cairo_rectangle( cr, (x * scale) - (20 * scale),
                                     -((y * scale) + (20 * scale)),
                                     40 * scale, 40 * scale );
                    cairo_fill( cr );
                } else if ( type == 13 || type == 38 ) { // Red card / skull
                    cairo_set_source_rgb( cr, 1.0, 0.0, 0.0 );
                    cairo_rectangle( cr, (x * scale) - (20 * scale),
                                     -((y * scale) + (20 * scale)),
                                     40 * scale, 40 * scale );
                    cairo_fill( cr );
                } else if ( type == 6 || type == 39 ) { // Yellow card / skull
                    cairo_set_source_rgb( cr, 1.0, 0.9, 0.0 );
                    cairo_rectangle( cr, (x * scale) - (20 * scale),
                                     -((y * scale) + (20 * scale)),
                                     40 * scale, 40 * scale );
                    cairo_fill( cr );
                }
            }
        }
    }

    // Print thing counts
    if ( countThings ) {
        PrintThingCounts();
    }

    // Write and cleanup
    cairo_surface_write_to_png( surface, "map.png" );
    cairo_destroy( cr );
    cairo_surface_destroy( surface );
}
