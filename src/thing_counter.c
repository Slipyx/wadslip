/*
** thing_counter.c
**
** Functions for counting things.
*/

#include "thing_counter.h"

// Thing counts
typedef struct {
    uint16_t easy, normal, hard;
} thingCount_t;

// Thing category
typedef enum {MONSTER, POWERUP, WEAPON, AMMO, KEY} thingCat_t;

// Monster names
static const char* monsterNames[] = {
    "Arachnotrons", "Arch-Viles", "Barons of Hell", "Cacodemons",
    "Commander Keens", "Cyberdemons", "Demons", "Heavy Weapon Dudes",
    "Hell Knights", "Imps", "Lost Souls", "Mancubi", "Pain Elementals",
    "Revenants", "Shotgun Guys", "Spectres", "Spiderdemons", "Wolfenstein SS",
    "Zombiemen", "Boss Brains"
};
// Powerup names
static const char* powerupNames[] = {
    "Armor bonuses", "Backpacks", "Berserk packs", "Blue armors",
    "Computer maps", "Green armors", "Health bonuses", "Invisibilities",
    "Invulnerabilities", "Light goggles", "Medikits", "Megaspheres",
    "Radiation suits", "Soul spheres", "Stimpacks"
};

static thingCat_t thingCat = MONSTER;
static thingCount_t monsterCounts[20] = {0, 0, 0};
static thingCount_t powerupCounts[15] = {0, 0, 0};

void CountThing( int16_t type, int16_t flags ) {
    uint8_t index = 255;
    thingCat = MONSTER;

    // Monster?
    switch ( type ) {
        // Arachnotron
        case 68: index = 0; break;
        // Arch-Vile
        case 64: index = 1; break;
        // Baron of Hell
        case 3003: index = 2; break;
        // Cacodemon
        case 3005: index = 3; break;
        // Commander Keen
        case 72: index = 4; break;
        // Cyberdemon
        case 16: index = 5; break;
        // Demon
        case 3002: index = 6; break;
        // Heavy Weapon Dude
        case 65: index = 7; break;
        // Hell Knight
        case 69: index = 8; break;
        // Imp
        case 3001: index = 9; break;
        // Lost Soul
        case 3006: index = 10; break;
        // Mancubus
        case 67: index = 11; break;
        // Pain Elemental
        case 71: index = 12; break;
        // Revenant
        case 66: index = 13; break;
        // Shotgun Guy
        case 9: index = 14; break;
        // Spectre
        case 58: index = 15; break;
        // Spiderdemon
        case 7: index = 16; break;
        // Wolfenstein SS
        case 84: index = 17; break;
        // Zombieman
        case 3004: index = 18; break;
        // Boss brain
        case 88: index = 19; break;
        // Not a monster, maybe a powerup?
        default: thingCat = POWERUP; break;
    }
    // Powerup?
    if ( thingCat == POWERUP ) {
        switch ( type ) {
            // Armor bonus
            case 2015: index = 0; break;
            // Backpack
            case 8: index = 1; break;
            // Berserk pack
            case 2023: index = 2; break;
            // Blue armor
            case 2019: index = 3; break;
            // Computer map
            case 2026: index = 4; break;
            // Green armor
            case 2018: index = 5; break;
            // Health bonus
            case 2014: index = 6; break;
            // Invisibility
            case 2024: index = 7; break;
            // Invulnerability
            case 2022: index = 8; break;
            // Light goggles
            case 2045: index = 9; break;
            // Medikit
            case 2012: index = 10; break;
            // Megasphere
            case 83: index = 11; break;
            // Radiation suit
            case 2025: index = 12; break;
            // Soul sphere
            case 2013: index = 13; break;
            // Stimpacks
            case 2011: index = 14; break;
            // Not a powerup, maybe a weapon?
            default: thingCat = WEAPON; break;
        }
    }

    if ( index != 255 && thingCat == MONSTER ) {
        if ( flags & TFLAG_SK_EASY )
            ++monsterCounts[index].easy;
        if ( flags & TFLAG_SK_NORMAL )
            ++monsterCounts[index].normal;
        if ( flags & TFLAG_SK_HARD )
            ++monsterCounts[index].hard;
    } else if ( index != 255 && thingCat == POWERUP ) {
        if ( flags & TFLAG_SK_EASY )
            ++powerupCounts[index].easy;
        if ( flags & TFLAG_SK_NORMAL )
            ++powerupCounts[index].normal;
        if ( flags & TFLAG_SK_HARD )
            ++powerupCounts[index].hard;
    }
}

void PrintThingCounts( void ) {
    uint8_t i = 0;
    // Monsters
    for ( i = 0; i < 20; ++i ) {
        if ( !(monsterCounts[i].easy == 0 && monsterCounts[i].normal == 0 &&
               monsterCounts[i].hard == 0) ) {
            printf( "%s: %u, %u, %u\n", monsterNames[i], monsterCounts[i].easy,
                    monsterCounts[i].normal, monsterCounts[i].hard );
        }
    }
    printf( "\n" );
    // Powerups
    for ( i = 0; i < 15; ++i ) {
        if ( !(powerupCounts[i].easy == 0 && powerupCounts[i].normal == 0 &&
               powerupCounts[i].hard == 0) ) {
            printf( "%s: %u, %u, %u\n", powerupNames[i], powerupCounts[i].easy,
                   powerupCounts[i].normal, powerupCounts[i].hard );
        }
    }
    printf( "\n" );
}
