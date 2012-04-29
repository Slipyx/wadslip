/*
** wadreader.c
**
** Functions for reading WAD data
*/

#include "wad_reader.h"

static FILE* wadfile = NULL;
static uint32_t i = 0, tmpPos = 0;

/*
** Open a WAD file
*/
void WAD_OpenFile( const char* filename ) {
    wadfile = fopen( filename, "rb" );
    if ( wadfile == NULL ) {
        fprintf( stderr, "Error opening WAD file: %s.\n", filename );
        exit( EXIT_FAILURE );
    }
}

/*
** Close a WAD file
*/
void WAD_CloseFile( void ) {
    if ( wadfile == NULL ) {
        fprintf( stderr, "Error closing WAD file! No open file to close!\n" );
        exit( EXIT_FAILURE );
    }
    fclose( wadfile );
}

/*
** Read WAD header
*/
void WAD_ReadHeader( wadinfo_t* info ) {
    fread( info->id, 1, 4, wadfile );            // ID
    fread( &info->numlumps, 1, 4, wadfile );     // numlumps
    fread( &info->infotableofs, 1, 4, wadfile ); // infotableofs
    // Seek to beginning of lump directory
    fseek( wadfile, info->infotableofs, SEEK_SET );
}

/*
** Read a lump info
*/
void WAD_ReadLump( lumpinfo_t* lump ) {
    fread( &lump->filepos, 1, 4, wadfile ); // filepos
    fread( &lump->size, 1, 4, wadfile );    // size
    fread( lump->name, 1, 8, wadfile );     // name
}

// Save position and seek
static void SaveAndSeek( uint32_t* pos ) {
    tmpPos = ftell( wadfile );
    fseek( wadfile, *pos, SEEK_SET );
}
// Restore saved position
static void RestorePosition( void ) {
    fseek( wadfile, tmpPos, SEEK_SET );
}

/*
** Read the first palette
*/
void WAD_ReadPalette( color_t* pal, lumpinfo_t* lump ) {
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < 256; ++i ) {
        fread( &pal[i].r, 1, 1, wadfile );
        fread( &pal[i].g, 1, 1, wadfile );
        fread( &pal[i].b, 1, 1, wadfile );
    }
    RestorePosition();
}

/*
** Read map THINGS
*/
void WAD_ReadMapThings( map_t* map, lumpinfo_t* lump ) {
    map->numthings = lump->size / sizeof(thing_t);
    map->things = (thing_t*)malloc( lump->size );
    // Save current position and seek to lump data
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < map->numthings; ++i ) {
        fread( &map->things[i].x, 1, 2, wadfile );
        fread( &map->things[i].y, 1, 2, wadfile );
        fread( &map->things[i].angle, 1, 2, wadfile );
        fread( &map->things[i].type, 1, 2, wadfile );
        fread( &map->things[i].flags, 1, 2, wadfile );
    }
    // Done reading so jump back to where we were
    RestorePosition();
}

/*
** Read map LINEDEFS
*/
void WAD_ReadMapLinedefs( map_t* map, lumpinfo_t* lump ) {
    map->numlinedefs = lump->size / sizeof(linedef_t);
    map->linedefs = (linedef_t*)malloc( lump->size );
    // Save current position and seek to lump data
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < map->numlinedefs; ++i ) {
        fread( &map->linedefs[i].v1, 1, 2, wadfile );
        fread( &map->linedefs[i].v2, 1, 2, wadfile );
        fread( &map->linedefs[i].flags, 1, 2, wadfile );
        fread( &map->linedefs[i].special, 1, 2, wadfile );
        fread( &map->linedefs[i].tag, 1, 2, wadfile );
        fread( &map->linedefs[i].sidenum[0], 1, 2, wadfile );
        fread( &map->linedefs[i].sidenum[1], 1, 2, wadfile );
    }
    // Done reading so jump back to where we were
    RestorePosition();
}

/*
** Read map SIDEDEFS
*/
void WAD_ReadMapSidedefs( map_t* map, lumpinfo_t* lump ) {
    map->numsidedefs = lump->size / sizeof(sidedef_t);
    map->sidedefs = (sidedef_t*)malloc( lump->size );
    // Save current position and seek to lump data
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < map->numsidedefs; ++i ) {
        fread( &map->sidedefs[i].xofs, 1, 2, wadfile );
        fread( &map->sidedefs[i].yofs, 1, 2, wadfile );
        fread( map->sidedefs[i].uppertexture, 1, 8, wadfile );
        fread( map->sidedefs[i].lowertexture, 1, 8, wadfile );
        fread( map->sidedefs[i].middletexture, 1, 8, wadfile );
        fread( &map->sidedefs[i].sectornum, 1, 2, wadfile );
    }
    // Done reading so jump back to where we were
    RestorePosition();
}

/*
** Read map VERTEXES
*/
void WAD_ReadMapVertexes( map_t* map, lumpinfo_t* lump ) {
    vertex_t minv = {INT16_MAX, INT16_MAX};
    vertex_t maxv = {INT16_MIN, INT16_MIN};
    map->numvertexes = lump->size / sizeof(vertex_t);
    map->vertexes = (vertex_t*)malloc( lump->size );
    // Save current position and seek to lump data
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < map->numvertexes; ++i ) {
        fread( &map->vertexes[i].x, 1, 2, wadfile );
        fread( &map->vertexes[i].y, 1, 2, wadfile );
        // Determine minimum and maximum points
        minv.x = (map->vertexes[i].x < minv.x)
               ? map->vertexes[i].x : minv.x;
        minv.y = (map->vertexes[i].y < minv.y)
               ? map->vertexes[i].y : minv.y;
        maxv.x = (map->vertexes[i].x > maxv.x)
               ? map->vertexes[i].x : maxv.x;
        maxv.y = (map->vertexes[i].y > maxv.y)
               ? map->vertexes[i].y : maxv.y;
    }
    // Determine map dimensions
    map->width = maxv.x - minv.x;
    map->height = maxv.y - minv.y;
    // Determine center point
    map->centerv.x = minv.x + map->width / 2;
    map->centerv.y = minv.y + map->height / 2;
    // Done reading so jump back to where we were
    RestorePosition();
}

/*
** Read map SECTORS
*/
void WAD_ReadMapSectors( map_t* map, lumpinfo_t* lump ) {
    map->numsectors = lump->size / sizeof(sector_t);
    map->sectors = (sector_t*)malloc( lump->size );
    // Save current position and seek to lump data
    SaveAndSeek( &lump->filepos );
    for ( i = 0; i < map->numsectors; ++i ) {
        fread( &map->sectors[i].floorheight, 1, 2, wadfile );
        fread( &map->sectors[i].ceilingheight, 1, 2, wadfile );
        fread( map->sectors[i].floorflat, 1, 8, wadfile );
        fread( map->sectors[i].ceilingflat, 1, 8, wadfile );
        fread( &map->sectors[i].lightlevel, 1, 2, wadfile );
        fread( &map->sectors[i].type, 1, 2, wadfile );
        fread( &map->sectors[i].tag, 1, 2, wadfile );
    }
    // Done reading so jump back to where we were
    RestorePosition();
}
