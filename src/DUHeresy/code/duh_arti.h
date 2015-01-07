//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Artifact functions.
//
//-----------------------------------------------------------------------------

#ifndef DUH_ARTI__
#define DUH_ARTI__


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

#define DUH_ElvenWandAmmo  s"GoldWandAmmo"
#define DUH_CrossbowAmmo   s"CrossbowAmmo"
#define DUH_DragonClawAmmo s"BlasterAmmo"
#define DUH_HellstaffAmmo  s"SkullRodAmmo"
#define DUH_PhoenixRodAmmo s"PhoenixRodAmmo"
#define DUH_FiremaceAmmo   s"MaceAmmo"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

void DUH_AmmoReady(int tic, __str ammo, int mul);

[[call("ScriptS"), extern("ACS")]]
void DUH_ArtifactReady(int frames);

[[call("ScriptS")]]
void DUH_WeaponReady(int frames, int *tics, __str ammo, int multiplier);

#endif//DUH_ARTI__

