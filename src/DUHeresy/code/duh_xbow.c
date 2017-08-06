//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2017 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Crossbow functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUH_CrossbowAltFire
//
[[call("ScriptS"), extern("ACS")]]
void DUH_CrossbowAltFire(bool useammo)
{
   int ammoCount = ACS_CheckInventory(DUH_CrossbowAmmo);
   int ammoUsage = (ammoCount / 10) + 1;
   int damage    = (ammoCount /  5) + 1;

   accum angle = ACS_GetActorAngle(0);
   accum pitch = ACS_GetActorPitch(0);

   accum spreadStep = 1.0k/80.0k; // 4.5 degrees
   accum spread = 0;

   DU_FireMissile(ACS_ActivatorTID(), s"DUH_CrossbowMissile3", damage, angle,
      pitch, 30);

   while(damage >>= 1)
   {
      spread += spreadStep;
      DU_FireMissile(ACS_ActivatorTID(), s"DUH_CrossbowMissile2", damage,
         angle + spread, pitch, 30);
      DU_FireMissile(ACS_ActivatorTID(), s"DUH_CrossbowMissile2", damage,
         angle - spread, pitch, 30);
   }

   if(useammo)
      ACS_TakeInventory(DUH_CrossbowAmmo, ammoUsage);
}

//
// DUH_CrossbowReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_CrossbowReady(unsigned frames)
{
   static unsigned tics[MAX_PLAYERS];

   DUH_WeaponReady(frames, tics, DUH_CrossbowAmmo, 1);
}

// EOF

