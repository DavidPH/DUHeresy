//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Phoenix Rod functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_PhoenixRodAltDone
//
[[call("ScriptS"), extern("ACS")]]
void DUH_PhoenixRodAltDone(bool useammo)
{
   int ammoUsage = ACS_CheckInventory(s"DUH_PhoenixRodFireAmmo");

   if(useammo)
      ACS_TakeInventory(DUH_PhoenixRodAmmo, ammoUsage);
}

//
// DUH_PhoenixRodAltFire
//
[[call("ScriptS"), extern("ACS")]]
void DUH_PhoenixRodAltFire()
{
   ACS_TakeInventory(s"DUH_PhoenixRodFireTics", 1);
   if(!ACS_CheckInventory(s"DUH_PhoenixRodFireTics"))
   {
      ACS_GiveInventory(s"DUH_PhoenixRodFireEnd", 1);
      return;
   }

   int damage = ACS_CheckInventory(s"DUH_PhoenixRodFireDamage");

   DU_FireMissile(ACS_ActivatorTID(), s"DUH_PhoenixRodMissile2", damage,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 10);
}

//
// DUH_PhoenixRodAltInit
//
[[call("ScriptS"), extern("ACS")]]
void DUH_PhoenixRodAltInit()
{
   int ammoCount = ACS_CheckInventory(DUH_PhoenixRodAmmo);
   int ammoUsage = (ammoCount / 10) + 1;
   int damage    = (ammoCount / 10) + ammoUsage + 1;

   ACS_TakeInventory(s"DUH_PhoenixRodFireAmmo", 1000);
   ACS_TakeInventory(s"DUH_PhoenixRodFireDamage", 1000);
   ACS_TakeInventory(s"DUH_PhoenixRodFireEnd", 1);

   ACS_GiveInventory(s"DUH_PhoenixRodFireAmmo", ammoUsage);
   ACS_GiveInventory(s"DUH_PhoenixRodFireDamage", damage);
   ACS_GiveInventory(s"DUH_PhoenixRodFireTics", 350);
}

//
// DUH_PhoenixRodFlameTic
//
[[call("ScriptS"), extern("ACS")]]
void DUH_PhoenixRodFlameTic()
{
   int damage = ACS_GetActorProperty(0, APROP_Damage);
   ACS_SetActorProperty(0, APROP_Damage, --damage);

   if(!damage) ACS_SetActorState(0, s"Done", false);
}

//
// DUH_PhoenixRodReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_PhoenixRodReady(int frames)
{
   static int tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_PhoenixRodAmmo, 10);
}

// EOF

