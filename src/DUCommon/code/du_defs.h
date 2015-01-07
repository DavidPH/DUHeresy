//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Common defintions and general functions.
//
//-----------------------------------------------------------------------------

#ifndef DU_DEFS_H__
#define DU_DEFS_H__

#include <ACS_ZDoom.h>
#include <stdbool.h>
#include <stdfix.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define MAX_PLAYERS 255


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

int DU_FireMissile(int shooter, __str name, int damage, accum angle,
   accum pitch, accum speed);

accum DU_GetInventoryFixed(int tid, __str inv);

int DU_MakeTID();

void DU_SetInventoryFixed(int tid, __str inv, accum val);

#endif//DU_DEFS_H__

