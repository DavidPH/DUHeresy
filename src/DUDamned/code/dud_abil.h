//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Ability functions.
//
//-----------------------------------------------------------------------------

#ifndef DUD_ABIL_H__
#define DUD_ABIL_H__

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct dud_ability_t dud_ability_t;
typedef struct dud_ammo_t    dud_ammo_t;
typedef struct dud_weapon_t  dud_weapon_t;

enum
{
   ABIL1_AMMO = 0,
   ABIL1_DAMAGE_CLIP,
   ABIL1_DAMAGE_SHEL,
   ABIL1_DAMAGE_RCKT,
   ABIL1_DAMAGE_CELL,
   ABIL1_BERSERK,

   ABIL2_AMMO = 0,
   ABIL2_ARMOR,
   ABIL2_RADIUS,
   ABIL2_PSPEED,

   ABIL3_AMMO = 0,
   ABIL3_HEALTH,
   ABIL3_DAMAGE,
   ABIL3_SPEED,
   ABIL3_JUMP,

   ABILMAX = 7
};

enum
{
   AMMO_CLIP,
   AMMO_SHEL,
   AMMO_RCKT,
   AMMO_CELL,

   AMMOMAX
};

enum
{
   WEAP1_FIST = 0,
   WEAP1_CSAW,
   WEAP1_CFIST,

   WEAP2_PIST = 0,

   WEAP3_SHTG = 0,
   WEAP3_SHT2,

   WEAP4_CGUN = 0,
   WEAP4_DGUN,

   WEAP5_RCKT = 0,

   WEAP6_PLAS = 0,
   WEAP6_LASG,

   WEAP7_BFG9 = 0,
   WEAP7_RAIL,

   WEAPMAX = 3
};

//
// dud_ability_t
//
struct dud_ability_t
{
   bool  used;
   accum ammoCap[AMMOMAX];
   int   ammoRegen[AMMOMAX];
   accum damage[AMMOMAX];
   accum radius[AMMOMAX];
   accum pspeed[AMMOMAX];
   int   armorRegen, healthRegen;
   accum speed, jumpz;
   __str desc;
   __str icon;
   __str iconDim;
   __str name;
   __str nameIco;
};

//
// dud_ammo_t
//
struct dud_ammo_t
{
   int   ammo;
   int   cap;
   int   regen;
   __str name;
   __str nameDec;
};

//
// dud_weapon_t
//
struct dud_weapon_t
{
   bool  used;
   int   ammo;
   __str desc;
   __str icon;
   __str iconDim;
   __str name;
   __str nameDec;
   __str nameIco;
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

extern dud_ammo_t DUD_Ammo[AMMOMAX];

extern dud_ability_t  DUD_Ability[3][ABILMAX];
extern dud_ability_t *DUD_AbilitySelected[MAX_PLAYERS][3];

extern dud_weapon_t  DUD_Weapon[7][WEAPMAX];
extern dud_weapon_t *DUD_WeaponSelected[MAX_PLAYERS][7];


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

void DUD_AddWeapon(int slot, int weap);

accum DUD_GetDamageFactor(int pnum, int ammo);
accum DUD_GetPSpeedFactor(int pnum, int ammo);
accum DUD_GetRadiusFactor(int pnum, int ammo);

#endif//DUD_ABIL_H__

