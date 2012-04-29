/*
** main.c
**
** Main entry point for the program
*/

#include "shared.h"
#include "wad_reader.h"
#include "map_drawer.h"

// Global configuration file
dictionary* ini = NULL;

int32_t main( void ) {
    uint16_t     l = 0; // Counter
    char*        wadfilename = NULL; // WAD filename specified
    char*        mapname = NULL; // Map name to find
    wadfile_t    wad; // Contains WAD info and lump info table
    map_t        map; // The map object
    color_t      pal[256] = {0}; // Palette
    uint16_t     nextMapLump = 0; // Number of next map lump to read

    strcpy( map.name, "" ); // If this never changes, we didn't find the map
    // Load config.ini file
    ini = iniparser_load( "config.ini" );
    if ( ini == NULL ) {
        fprintf( stderr, "Failed to load config.ini!\n" );
        exit( EXIT_FAILURE );
    }
    //iniparser_dump(ini, stdout);
    // Get filename and map name from config file
    wadfilename = iniparser_getstring( ini, "Main:file", NULL );
    if ( wadfilename == NULL ) {
        fprintf( stderr, "Error! Must specify a WAD file!" );
        exit( EXIT_FAILURE );
    }

    // Open the WAD file for reading
    WAD_OpenFile( wadfilename );

    // Read WAD header
    printf( "Loading WAD file: %s...\n", wadfilename );
    WAD_ReadHeader( &wad.info );

    // Get name of map to find from config file
    mapname = iniparser_getstring( ini, "Main:map", NULL );
    // Allocate memory for lump info array
    wad.lumps = (lumpinfo_t*)malloc( sizeof(lumpinfo_t) * wad.info.numlumps );
    // Read all the lump entries in the directory
    for ( l = 0; l < wad.info.numlumps; ++l ) {
        WAD_ReadLump( &wad.lumps[l] );

        // Find palette
        if ( !strcmp( wad.lumps[l].name, "PLAYPAL" ) ) {
            WAD_ReadPalette( pal, &wad.lumps[l] );
        }

        // Find specified map
        if ( mapname != NULL && !strcmp( wad.lumps[l].name, mapname ) ) {
            strcpy( map.name, wad.lumps[l].name );
            printf( "    Found %.8s!\n", map.name );
            nextMapLump = l + 1; // THINGS
        }
    }

    // Load map
    if ( strcmp( map.name, "" ) ) {
        WAD_ReadMapThings( &map, &wad.lumps[nextMapLump++] );
        WAD_ReadMapLinedefs( &map, &wad.lumps[nextMapLump++] );
        WAD_ReadMapSidedefs( &map, &wad.lumps[nextMapLump++] );
        WAD_ReadMapVertexes( &map, &wad.lumps[nextMapLump] ); nextMapLump += 4;
        WAD_ReadMapSectors( &map, &wad.lumps[nextMapLump] );
    }

    // Close the WAD file
    WAD_CloseFile();
    printf( "Done loading WAD file.\n\n" );

    // Draw the map
    if ( strcmp( map.name, "" ) ) {
        DrawMap( &map );
    } else {
        printf( "Map not found!\n\n" );
    }
    // Palette?
    DrawPalette( pal );

    // Done with config file
    iniparser_freedict( ini );
    // Output WAD information
    printf( "Dump of WAD file: %s\n\n", wadfilename );
    printf( "WAD INFO:\n"
            "    ID: %.4s\n"
            "    numlumps: %d\n"
            "    infotableofs: 0x%08X\n\n",
            wad.info.id, wad.info.numlumps, wad.info.infotableofs );
    // Output the lump directory
    for ( l = 0; l < wad.info.numlumps; ++l) {
        printf( "LUMP %d:\n"
                "    name: %.8s\n"
                "    filepos: 0x%08X\n"
                "    size: %d bytes\n\n", l + 1,
                wad.lumps[l].name, wad.lumps[l].filepos, wad.lumps[l].size);
    }

    // Cleanup
    if ( strcmp( map.name, "" ) ) {
        free( map.sectors );
        free( map.vertexes );
        free( map.sidedefs );
        free( map.linedefs );
        free( map.things );
    }
    free( wad.lumps );

    //system( "PAUSE" );
    exit( EXIT_SUCCESS );
}
