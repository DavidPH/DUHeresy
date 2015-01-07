//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Firemace functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_FiremaceAltFire
//
[[call("ScriptS"), extern("ACS")]]
void DUH_FiremaceAltFire(bool useammo)
{
   int ammoCount = ACS_CheckInventory(DUH_FiremaceAmmo);
   int ammoUsage = (ammoCount / 75) + 1;
   int damage    = (ammoCount / 25) + ammoUsage + 7;

   int tid;

   tid = DU_FireMissile(ACS_ActivatorTID(), s"DUH_FiremaceMissile4", damage,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 25);

   ACS_SetUserVariable(tid, s"user_shooter", ACS_ActivatorTID());

   if(useammo)
      ACS_TakeInventory(DUH_FiremaceAmmo, ammoUsage);
}

//
// DUH_FiremaceDeathRing
//
[[call("ScriptS"), extern("ACS")]]
void DUH_FiremaceDeathRing(void)
{
   int damage  = (ACS_GetActorProperty(0, APROP_Damage) + 7) / 2;
   int shooter = ACS_GetUserVariable(0, s"user_shooter");

   for(accum i = damage; i--;) for (accum j = damage; j--;)
   {
      accum angle = i / damage;
      accum pitch = j / damage;

      DU_FireMissile(shooter, s"DUH_FiremaceMissile5", 1, angle, pitch, 15);
   }
}

//
// DUH_FiremaceMissileSound
//
[[call("ScriptS"), extern("ACS")]]
void DUH_FiremaceMissileSound(void)
{
   ACS_SetActorPropertyString(0, APROP_SeeSound, s"weapons/macebounce");
}

//
// DUH_FiremaceReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_FiremaceReady(int frames)
{
   static int tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_FiremaceAmmo, 20);
}

// EOF

