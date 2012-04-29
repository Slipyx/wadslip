/*
** wadtypes.h
**
** Custom type definitions that are found in WAD files
*/

#ifndef __WADTYPES_H
#define __WADTYPES_H

#include <stdint.h>

// THING flags
#define TFLAG_SK_EASY   0x0001 // On skill levels 1 & 2
#define TFLAG_SK_NORMAL 0x0002 // On skill level 3
#define TFLAG_SK_HARD   0x0004 // On skill level 4 & 5
#define TFLAG_SK_ALL    0x0007 // On all the skill levels
#define TFLAG_DEAF      0x0008 // Doesn't react to sound
#define TFLAG_MULT      0x0010 // Only in multiplayer

// WAD info struct
typedef struct {
    char     id[4];        // Type of WAD; IWAD or PWAD
    uint32_t numlumps;     // Number of lumps
    uint32_t infotableofs; // Offset to beginning of lump directory
} wadinfo_t;

// Lump info struct
typedef struct {
    uint32_t filepos; // Offset to beginning of lump data
    uint32_t size;    // Size of lump in bytes
    char     name[8]; // Name of lump
} lumpinfo_t;

// THINGS lump struct
typedef struct {
    int16_t x, y;  // x, y position
    int16_t angle; // Facing angle
    int16_t type;  // DoomEd thing type
    int16_t flags; // Flags
} thing_t;

// LINEDEF lump struct
typedef struct {
    uint16_t v1, v2;     // Start and end vertex
    int16_t  flags;      // Flags
    int16_t  special;    // Special type
    int16_t  tag;        // Sector tag
    int16_t  sidenum[2]; // sidenum[1] is back side, -1 if one-sided
} linedef_t;

// SIDEDEF lump struct
typedef struct {
    int16_t  xofs; // Texture x offset
    int16_t  yofs; // Texture y offset
    char     uppertexture[8];
    char     lowertexture[8];
    char     middletexture[8];
    uint16_t sectornum; // Sector number this sidedef faces
} sidedef_t;

// VERTEX lump struct
typedef struct {
    int16_t x, y; // x, y position
} vertex_t;

// SECTOR lump struct
typedef struct {
    int16_t floorheight;
    int16_t ceilingheight;
    char    floorflat[8];
    char    ceilingflat[8];
    int16_t lightlevel;
    int16_t type;
    int16_t tag;
} sector_t;

// MAP lump struct
typedef struct {
    char       name[8];       // Name of map
    uint16_t   width, height; // Dimensions of map
    vertex_t   centerv;       // Center point

    uint32_t   numthings;     // Total number of THINGS
    thing_t*   things;        // Array of all the THINGS

    uint32_t   numlinedefs;   // Total number of LINEDEFS
    linedef_t* linedefs;      // Array of all the LINEDEFS

    uint32_t   numsidedefs;   // Total number of SIDEDEFS
    sidedef_t* sidedefs;      // Array of all the SIDEDEFS

    uint32_t   numvertexes;   // Total number of VERTEXES
    vertex_t*  vertexes;      // Array of all the VERTEXES

    uint32_t   numsectors;    // Total number of SECTORS
    sector_t*  sectors;       // Array of all the SECTORS
} map_t;

// WAD file struct
typedef struct {
    wadinfo_t   info;  // WAD file header information
    lumpinfo_t* lumps; // Array of all the lump definitions
} wadfile_t;

#endif
