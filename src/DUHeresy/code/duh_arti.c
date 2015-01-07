//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Artifact functions.
//
//-----------------------------------------------------------------------------

#include "duh_arti.h"

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DUH_ArtifactTomeReady
//
static void DUH_ArtifactTomeReady(__str ammo, int cap)
{
   ACS_SetAmmoCapacity(ammo,  cap);

   int count = ACS_CheckInventory(ammo);

   if(count > cap)
      ACS_TakeInventory(ammo, count - cap);
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_AmmoReady
//
void DUH_AmmoReady(int tic, __str ammo, int mul)
{
   int ammo_count = ACS_CheckInventory(ammo);
   int ammo_limit = ACS_GetAmmoCapacity(ammo) * mul;
   int period;

   period = ammo_count ? ammo_limit / ammo_count : ammo_limit;
   if(!period || !(tic % period)) ACS_GiveInventory(ammo, 1);
}

//
// DUH_ArtifactReady
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ArtifactReady(int frames)
{
   static int tics[MAX_PLAYERS];

   while(frames--)
   {
      int tic = ++tics[ACS_PlayerNumber()];
      int inv;

      int tid = ACS_ActivatorTID();

      if(!tid) ACS_Thing_ChangeTID(0, tid = DU_MakeTID());

      inv = ACS_CheckInventory(s"ArtiEgg");
      if(inv > 256) inv = 256;
      if(inv && !(tic % ((35 * 256) / inv)))
         ACS_Spawn(s"DUH_ChickenFriend", ACS_GetActorX(0), ACS_GetActorY(0),
            ACS_GetActorZ(0) + 56, 0, ACS_GetActorAngle(0)*256);

      inv = ACS_CheckInventory(s"ArtiFly");
      if(inv > 0x7FFF - 8) inv = 0x7FFF - 8;
      ACS_SetActorPropertyFixed(0, APROP_JumpZ, inv + 8.0k);

      inv = ACS_CheckInventory(s"ArtiHealth");
      if(inv > 35 * 4) inv = 35 * 4;
      if(inv && !(tic % ((35 * 4) / inv)))
         ACS_GiveInventory(s"Health", 1);

      inv = ACS_CheckInventory(s"ArtiInvisibility");
      if(inv > 35 * 8) inv = 35 * 8;
      if(inv && !(tic % ((35 * 8) / inv)))
         ACS_GiveInventory(s"DUH_ArmorBonus", 1);

      inv = ACS_CheckInventory(s"ArtiInvulnerability");
      if(inv > 35 * 1) inv = 35 * 1;
      if(inv && !(tic % ((35 * 1) / inv)))
         ACS_GiveInventory(s"DUH_ArmorBonus", 1);

      inv = ACS_CheckInventory(s"ArtiSuperHealth") + 4;
      if(inv > 0x7FFFFFFF / 25) inv = 0x7FFFFFFF / 25;
      ACS_SetActorProperty(0, APROP_SpawnHealth, inv * 25);

      inv = ACS_CheckInventory(s"ArtiTeleport");
      if(inv > 16) inv = 16;
      ACS_SetActorPropertyFixed(0, APROP_Speed, (inv / 10.0) + 1.0);

      inv = ACS_CheckInventory(s"ArtiTimeBomb");
      if(inv > 35 * 16) inv = 35 * 16;
      if(inv && !(tic % ((35 * 16) / inv)))
         ACS_SpawnProjectile(tid, s"DUH_TimeBombAura", 0, 0, 0, 0, 0);

      inv = ACS_CheckInventory(s"ArtiTomeOfPower") + 4;
      if(inv > 65536) inv = 65536;
      DUH_ArtifactTomeReady(DUH_ElvenWandAmmo,  inv *  50);
      DUH_ArtifactTomeReady(DUH_CrossbowAmmo,   inv *  25);
      DUH_ArtifactTomeReady(DUH_DragonClawAmmo, inv * 100);
      DUH_ArtifactTomeReady(DUH_HellstaffAmmo,  inv * 100);
      DUH_ArtifactTomeReady(DUH_PhoenixRodAmmo, inv *  10);
      DUH_ArtifactTomeReady(DUH_FiremaceAmmo,   inv *  75);

      inv = ACS_CheckInventory(s"ArtiTorch");
      if(inv > 64) inv = 64;
      if(inv && !(tic % ((35 * 64) / inv)))
         ACS_GiveInventory(s"ArtiTorch", 1);

      DUH_AmmoReady(tic, DUH_ElvenWandAmmo,   20);
      DUH_AmmoReady(tic, DUH_CrossbowAmmo,    30);
      DUH_AmmoReady(tic, DUH_DragonClawAmmo,  40);
      DUH_AmmoReady(tic, DUH_HellstaffAmmo,   50);
      DUH_AmmoReady(tic, DUH_PhoenixRodAmmo, 100);
      DUH_AmmoReady(tic, DUH_FiremaceAmmo,   200);

      if(frames) ACS_Delay(1);
   }
}

//
// DUH_ChickenFriendCountdown
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ChickenFriendCountdown(int frames)
{
   ACS_GiveInventory(s"DUH_ChickenFriendCounter", frames);

   while(ACS_CheckInventory(s"DUH_ChickenFriendCounter") >= 35)
   {
      ACS_TakeInventory(s"DUH_ChickenFriendCounter", 35);
      ACS_DamageThing(1, 0);
   }
}

//
// DUH_WeaponReady
//
[[call("ScriptS")]]
void DUH_WeaponReady(int frames, int *tics, __str ammo, int multiplier)
{
   DUH_ArtifactReady(frames);

   while(frames--)
   {
      DUH_AmmoReady(++tics[ACS_PlayerNumber()], ammo, multiplier);

      if(frames) ACS_Delay(1);
   }
}

// EOF

