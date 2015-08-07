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
// Types                                                                      |
//

//
// DU_Position
//
typedef struct DU_Position DU_Position;
struct DU_Position
{
   accum x, y, z;
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

accum DU_AverageAngle(accum angle1, accum angle2);

int DU_FireMissile(int shooter, __str name, int damage, accum angle,
   accum pitch, accum speed);

accum DU_GetInventoryFixed(int tid, __str inv);

[[call("ScriptS")]]
DU_Position DU_GetTargetPosition(int tid);

int DU_MakeTID(void);

void DU_SetInventoryFixed(int tid, __str inv, accum val);

#endif//DU_DEFS_H__

