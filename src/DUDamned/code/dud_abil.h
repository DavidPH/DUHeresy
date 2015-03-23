//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Ability handling.
//
//-----------------------------------------------------------------------------

#ifndef DUD_ABIL_H__
#define DUD_ABIL_H__

#include "dud_weap.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct dud_ability_t dud_ability_t;

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


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern dud_ability_t  DUD_Ability[3][ABILMAX];
extern dud_ability_t *DUD_AbilitySelected[MAX_PLAYERS][3];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void DUD_DoAbilities(int pnum, int tics);

void DUD_DoAmmoCap(int pnum, int tics);
void DUD_DoAmmoRegen(int pnum, int tics);
void DUD_DoArmorRegen(int pnum, int tics);
void DUD_DoHealthRegen(int pnum, int tics);
void DUD_DoJumpHeight(int pnum, int tics);
void DUD_DoRunSpeed(int pnum, int tics);
void DUD_DoWeaponSelect(int pnum);

accum DUD_GetDamageFactor(int pnum, int ammo);
accum DUD_GetPSpeedFactor(int pnum, int ammo);
accum DUD_GetRadiusFactor(int pnum, int ammo);

#endif//DUD_ABIL_H__

