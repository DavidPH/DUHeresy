//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Hellstaff functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_HellstaffAltFire
//
[[call("ScriptS"), extern("ACS")]]
void DUH_HellstaffAltFire(bool useammo)
{
   int ammoCount = ACS_CheckInventory(DUH_HellstaffAmmo);
   int ammoUsage = (ammoCount / 100) + 1;
   int damage    = (ammoCount / 25) + ammoUsage + 1;

   int tid;

   tid = DU_FireMissile(ACS_ActivatorTID(), s"DUH_HellstaffMissile2", damage,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 25);

   ACS_GiveActorInventory(tid, s"DUH_HellstaffMissileStormTics",
      (ammoCount / 10) + 10);
   ACS_SetUserVariable(tid, s"user_shooter", ACS_ActivatorTID());

   if(useammo)
      ACS_TakeInventory(DUH_HellstaffAmmo, ammoUsage);
}

//
// DUH_HellstaffReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_HellstaffReady(int frames)
{
   static int tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_HellstaffAmmo, 5);
}

//
// DUH_HellstaffStorm
//
[[call("ScriptS"), extern("ACS")]]
void DUH_HellstaffStorm(void)
{
   accum basex = ACS_GetActorX(0);
   accum basey = ACS_GetActorY(0);
   accum basez = ACS_GetActorCeilingZ(0) - 12;

   int shooter = ACS_GetUserVariable(0, s"user_shooter");
   int tics    = ACS_CheckInventory(s"DUH_HellstaffMissileStormtics");

   while(tics)
   {
      accum x = basex + ACS_RandomFixed(-64, 64);
      accum y = basey + ACS_RandomFixed(-64, 64);
      accum z = basez;

      int tid = DU_MakeTID();

      // Hack to not kill projectile if shooting from water.
      accum shooterx = ACS_GetActorX(shooter);
      accum shootery = ACS_GetActorY(shooter);
      accum shooterz = ACS_GetActorZ(shooter);

      ACS_SetActorPosition(shooter, shooterx, shootery, shooterz+16, false);

      ACS_SpawnProjectile(shooter, s"DUH_HellstaffMissile3", 0, 0, 0, 0, tid);
      if(ACS_SetActorPosition(tid, x, y, z, false))
      {
         ACS_SetActorVelocity(tid, 0, 0, -5, false, false);
         --tics;
      }
      else
         ACS_Thing_Remove(tid);

      ACS_SetActorPosition(shooter, shooterx, shootery, shooterz, false);

      ACS_Delay(1);
   }
}

// EOF

