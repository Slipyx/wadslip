/*
** wadreader.h
**
** Functions for reading WAD data
*/

#ifndef __WAD_READER_H
#define __WAD_READER_H

#include "shared.h"

/*
** Open a WAD file
*/
void WAD_OpenFile( const char* filename );

/*
** Close a WAD file
*/
void WAD_CloseFile( void );

/*
** Read WAD header
*/
void WAD_ReadHeader( wadinfo_t* info );

/*
** Read a lump info
*/
void WAD_ReadLump( lumpinfo_t* lump );

/*
** Read the first palette
*/
void WAD_ReadPalette( color_t* pal, lumpinfo_t* lump );

/*
** Read map THINGS
*/
void WAD_ReadMapThings( map_t* map, lumpinfo_t* lump );

/*
** Read map LINEDEFS
*/
void WAD_ReadMapLinedefs( map_t* map, lumpinfo_t* lump );

/*
** Read map SIDEDEFS
*/
void WAD_ReadMapSidedefs( map_t* map, lumpinfo_t* lump );

/*
** Read map VERTEXES
*/
void WAD_ReadMapVertexes( map_t* map, lumpinfo_t* lump );

/*
** Read map SECTORS
*/
void WAD_ReadMapSectors( map_t* map, lumpinfo_t* lump );

#endif
