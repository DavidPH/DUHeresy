//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Elven Wand functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_ElvenLaserTrailSpawn
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ElvenLaserTrailSpawn(bool final)
{
   accum curx = ACS_GetActorX(0);
   accum cury = ACS_GetActorY(0);
   accum curz = ACS_GetActorZ(0);

   accum oldx = ACS_GetUserVariableFixed(0, s"user_oldX");
   accum oldy = ACS_GetUserVariableFixed(0, s"user_oldY");
   accum oldz = ACS_GetUserVariableFixed(0, s"user_oldZ");

   accum difx = curx - oldx;
   accum dify = cury - oldy;
   accum difz = curz - oldz;

   int i = (absk(difx) + absk(dify) + absk(difz)) / 32;

   if(i)
   {
      difx /= i;
      dify /= i;
      difz /= i;
   }

   while(i--)
   {
      ACS_Spawn(s"DUH_ElvenLaserTrail", oldx += difx, oldy += dify, oldz += difz,
         0, ACS_GetActorAngle(0)*256);
   }

   if(final)
   {
      if(ACS_GetActorZ(0) + 4 >= ACS_GetActorCeilingZ(0) &&
         ACS_CheckActorCeilingTexture(0, s"F_SKY1"))
      {
         ACS_SetActorState(0, s"DeathSky", false);
      }
      else if(ACS_GetActorZ(0) < ACS_GetActorFloorZ(0) &&
         ACS_CheckActorFloorTexture(0, s"F_SKY1"))
      {
         ACS_SetActorState(0, s"DeathSky", false);
      }
   }
   else
   {
      ACS_SetUserVariableFixed(0, s"user_oldX", curx);
      ACS_SetUserVariableFixed(0, s"user_oldY", cury);
      ACS_SetUserVariableFixed(0, s"user_oldZ", curz);
   }
}

//
// DUH_ElvenWandDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUH_ElvenWandDamage(bool useammo)
{
   int ammoCount = ACS_CheckInventory(DUH_ElvenWandAmmo);
   int ammoUsage = ammoCount / 50 + 1;
   int dice      = ammoCount / 25 + 1;

   if(useammo)
      ACS_TakeInventory(DUH_ElvenWandAmmo, ammoUsage);

   int damage = ammoUsage;
   while(dice--)
      damage += ACS_Random(1, 8);

   return damage;
}

//
// DUH_ElvenWandFire
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ElvenWandFire(bool piercing)
{
   __str missile = piercing ? s"DUH_ElvenLaserRipper" : s"DUH_ElvenLaser";

   int tid = DU_FireMissile(ACS_ActivatorTID(), missile, 1,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 256);

   ACS_SetUserVariableFixed(tid, s"user_oldX", ACS_GetActorX(tid));
   ACS_SetUserVariableFixed(tid, s"user_oldY", ACS_GetActorY(tid));
   ACS_SetUserVariableFixed(tid, s"user_oldZ", ACS_GetActorZ(tid));
}

//
// DUH_ElvenWandReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ElvenWandReady(int frames)
{
   static int tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_ElvenWandAmmo, 35);
}

// EOF

