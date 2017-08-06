//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Dragon Claw functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUH_DragonClawDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUH_DragonClawDamage(bool useammo)
{
   int ammoCount = ACS_CheckInventory(DUH_DragonClawAmmo);
   int ammoUsage = ammoCount / 10 + 1;
   int dice      = ammoCount / 20 + 1;

   if(useammo)
      ACS_TakeInventory(DUH_DragonClawAmmo, ammoUsage);

   int damage = 0;
   while(dice--)
      damage += ACS_Random(1, 8);

   return damage;
}

//
// DUH_DragonClawReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_DragonClawReady(unsigned frames)
{
   static unsigned tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_DragonClawAmmo, 1);
}

// EOF

