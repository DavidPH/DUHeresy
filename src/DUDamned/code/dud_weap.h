//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Weapon and ammo handling.
//
//-----------------------------------------------------------------------------

#ifndef DUD_WEAP_H__
#define DUD_WEAP_H__

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct dud_ammo_t   dud_ammo_t;
typedef struct dud_weapon_t dud_weapon_t;

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
   WEAP1_CFIS,

   WEAP2_PIST = 0,

   WEAP3_SHTG = 0,
   WEAP3_SHT2,
   WEAP3_AGUN,

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
   bool  have;
   int   ammo;
   __str desc;
   __str icon;
   __str iconDim;
   __str name;
   __str nameDec;
   __str nameIco;
};


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern dud_ammo_t DUD_Ammo[AMMOMAX];

extern dud_weapon_t  DUD_Weapon[7][WEAPMAX];
extern dud_weapon_t *DUD_WeaponSelected[MAX_PLAYERS][7];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void DUD_AddWeapon(int slot, int weap);

#endif//DUD_WEAP_H__

