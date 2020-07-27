#include "ScriptPCH.h"
#include <cstring>

uint32 skin;
uint32 face;
uint32 hairstyle;
uint32 haircolor;
uint32 facialfeature;
bool helmetShown;

typedef struct maxStyles_struct {
        uint8 maxMale;
        uint8 maxFemale;
} maxStyles_t;

uint8 maxSkinColor[MAX_RACES] =
{
    0,  //                        0
    9,  // RACE_HUMAN           = 1,
    8,  //  RACE_ORC            = 2,
    8,  // RACE_DWARF           = 3,
    9,  // RACE_NIGHTELF        = 4,
    5,  // RACE_UNDEAD_PLAYER   = 5,
    8,  //  RACE_TAUREN         = 6,
    8,  // RACE_GNOME           = 7,
    5,  // RACE_TROLL           = 8,
    0,  // RACE_GOBLIN          = 9,
    9,  //  RACE_BLOODELF       = 10,
    11,  //  RACE_DRAENEI        = 11
};

maxStyles_t maxFaceStyles[MAX_RACES] =
{
    {0,0},  //                        0
    {12,14},// RACE_HUMAN           = 1,
    {8,8},  //  RACE_ORC            = 2,
    {9,9},// RACE_DWARF           = 3,
    {8,8},  // RACE_NIGHTELF        = 4,
    {9,9}, // RACE_UNDEAD_PLAYER   = 5,
    {5,4},  //  RACE_TAUREN         = 6, готово
    {6,6},  // RACE_GNOME           = 7,
    {4,5},  // RACE_TROLL           = 8,
    {0,0},  // RACE_GOBLIN          = 9,
    {9,9}, //  RACE_BLOODELF       = 10,
    {9,9}, //  RACE_DRAENEI        = 11
};

maxStyles_t maxHairStyles[MAX_RACES] =
{
    {0,0},  //                        0
    {11,18},// RACE_HUMAN           = 1,
    {6,6},  //  RACE_ORC            = 2,
    {10,13},// RACE_DWARF           = 3,
    {6,6},  // RACE_NIGHTELF        = 4,
    {10,9}, // RACE_UNDEAD_PLAYER   = 5,
    {7,6},  //  RACE_TAUREN         = 6,
    {6,6},  // RACE_GNOME           = 7,
    {5,4},  // RACE_TROLL           = 8,
    {0,0},  // RACE_GOBLIN          = 9,
    {9,13}, //  RACE_BLOODELF       = 10,
    {7,10}, //  RACE_DRAENEI        = 11
};

uint8 maxHairColor[MAX_RACES] =
{
	0,  //                        0
    9,  // RACE_HUMAN           = 1,
    7,  //  RACE_ORC            = 2,
    9,  // RACE_DWARF           = 3,
    7,  // RACE_NIGHTELF        = 4,
    9,  // RACE_UNDEAD_PLAYER   = 5,
    2,  //  RACE_TAUREN         = 6,
    4,  // RACE_GNOME           = 7,
    9,  // RACE_TROLL           = 8,
    0,  // RACE_GOBLIN          = 9,
    9,  //  RACE_BLOODELF       = 10,
    6,  //  RACE_DRAENEI        = 11
};

maxStyles_t maxFacialFeatures[MAX_RACES] =
{
    {0,0},  //                        0
    {8,6},  // RACE_HUMAN           = 1,
    {10,6}, // RACE_ORC             = 2,
    {10,5}, // RACE_DWARF           = 3,
    {5,9},  // RACE_NIGHTELF        = 4,
    {16,7}, // RACE_UNDEAD_PLAYER   = 5,
    {6,4},  // RACE_TAUREN          = 6,
    {7,6},  // RACE_GNOME           = 7,
    {10,5}, // RACE_TROLL           = 8,
    {0,0},  // RACE_GOBLIN          = 9,
    {10,9}, // RACE_BLOODELF        = 10,
    {7,6},  // RACE_DRAENEI         = 11
};


void SelectSkin(Player* player, uint8 change);
void SelectFace(Player* player, uint8 change);
void SelectHairStyle(Player* player, uint8 change);
void SelectHairColor(Player* player, uint8 change);
void SelectFacialFeature(Player* player, uint8 change);
void SetRace(Player* player, uint8 oldrace, uint8 race);
void ReplaceSpells(Player* player, uint8 oldrace, uint8 newrace);