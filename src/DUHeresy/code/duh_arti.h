//-----------------------------------------------------------------------------
//
// Copyright (C) 2015-2017 David Hill
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

void DUH_AmmoReady(unsigned tic, __str ammo, unsigned mul);

[[call("ScriptS"), extern("ACS")]]
void DUH_ArtifactReady(unsigned frames);

[[call("ScriptS"), extern("ACS")]]
void DUH_ArtifactReadyTID(int tid, unsigned frames);

[[call("ScriptS")]]
void DUH_WeaponReady(unsigned frames, unsigned *tics, __str ammo, unsigned mul);

#endif//DUH_ARTI__

