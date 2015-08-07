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

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DU_AverageAngle
//
accum DU_AverageAngle(accum angle1, accum angle2)
{
   if(absk(angle1 - angle2) > 0.5k)
      return ((angle1 + angle2) / 2 + 0.5k) % 1.0k;
   else
      return (angle1 + angle2) / 2;
}

//
// DU_FireMissile
//
int DU_FireMissile(int shooter, __str name, int damage, accum angle,
   accum pitch, accum speed)
{
   int tid = DU_MakeTID();

   accum velx = ACS_Cos(angle) * ACS_Cos(pitch) * speed;
   accum vely = ACS_Sin(angle) * ACS_Cos(pitch) * speed;
   accum velz = ACS_Sin(1.0k - pitch) * speed;

   accum x = ACS_GetActorX(0);
   accum y = ACS_GetActorY(0);
   accum z = ACS_GetActorZ(0);

   if(ACS_GetActorViewHeight(ACS_ActivatorTID()))
      z += ACS_GetActorViewHeight(ACS_ActivatorTID()) - 5;

   // Hack to not kill projectile if shooting from water.
   // Cache original position because the position change could fail in which
   // case saying z-16 would lower the mobj!
   accum shooterx = ACS_GetActorX(shooter);
   accum shootery = ACS_GetActorY(shooter);
   accum shooterz = ACS_GetActorZ(shooter);

   ACS_SetActorPosition(shooter, shooterx, shootery, shooterz+16, false);

   // SpawnProjectile is used to set the newly created missile's target.
   // Other than that, it's pretty useless because it always spawns at the
   // shooter's feet, so we just set all the positioning and velocity manually.
   ACS_SpawnProjectile(shooter, name, 0, 0, 0, 0, tid);

   ACS_SetActorPosition(shooter, shooterx, shootery, shooterz, false);

   // And here's the entire point of this function, custom damage.
   ACS_SetActorProperty(tid, APROP_Damage, damage);

   // Set twice for hacky avoidance of the floor.
   ACS_SetActorPosition(tid, x, y, z+16, false);
   ACS_SetActorPosition(tid, x, y, z, false);
   ACS_SetActorVelocity(tid, velx, vely, velz, false, false);

   ACS_SetActorAngle(tid, angle);
   ACS_SetActorPitch(tid, pitch);

   return tid;
}

//
// DU_GetInventoryFixed
//
accum DU_GetInventoryFixed(int tid, __str inv)
{
   union {unsigned i; accum x;} u;

   if(tid)
      u.i = ACS_CheckActorInventory(tid, inv);
   else
      u.i = ACS_CheckInventory(inv);

   u.i <<= 1;

   return u.x;
}

//
// DU_GetTargetPosition
//
[[call("ScriptS")]]
DU_Position DU_GetTargetPosition(int tid)
{
   ACS_SetActivatorToTarget(tid);

   DU_Position pos =
   {
      .x = ACS_GetActorX(0),
      .y = ACS_GetActorY(0),
      .z = ACS_GetActorZ(0),
   };

   return pos;
}

//
// DU_MakeTID
//
int DU_MakeTID(void)
{
   // Might be worth using a static object to store the last result?
   return ACS_UniqueTID();
}

//
// DU_SetInventoryFixed
//
void DU_SetInventoryFixed(int tid, __str inv, accum val)
{
   union {unsigned i; accum x;} u = {val};

   u.i >>= 1;

   if(tid)
   {
      ACS_TakeActorInventory(tid, inv, 0x7FFFFFFF);
      ACS_GiveActorInventory(tid, inv, u.i);
   }
   else
   {
      ACS_TakeInventory(inv, 0x7FFFFFFF);
      ACS_GiveInventory(inv, u.i);
   }
}

// EOF

