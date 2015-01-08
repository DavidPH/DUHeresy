//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Weapon functions.
//
//-----------------------------------------------------------------------------

#include "dud_abil.h"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUD_ChaingunDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUD_ChaingunDamage(void)
{
   return 5 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CLIP);
}

//
// DUD_FireBFG
//
[[call("ScriptS"), extern("ACS")]]
void DUD_FireBFG(void)
{
   int damage = 100 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CELL);
   int pspeed =  25 * DUD_GetPSpeedFactor(ACS_PlayerNumber(), AMMO_CELL);

   DU_FireMissile(0, s"DUD_BFGBall", damage, ACS_GetActorAngle(0),
      ACS_GetActorPitch(0), pspeed);

   ACS_TakeInventory(s"Cell", 40);
}

//
// DUD_FireDGun
//
[[call("ScriptS"), extern("ACS")]]
void DUD_FireDGun(void)
{
   static int counts[MAX_PLAYERS];

   int *count = &counts[ACS_PlayerNumber()];

   int damage =  1 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CLIP);
   int pspeed = 80 * DUD_GetPSpeedFactor(ACS_PlayerNumber(), AMMO_CLIP);

   accum angle = ACS_GetActorAngle(0) + ACS_RandomFixed(-1.0k/256, 1.0k/256);
   accum pitch = ACS_GetActorPitch(0) + ACS_RandomFixed(-1.0k/256, 1.0k/256);

   int tid = DU_FireMissile(0, s"DUD_Dakka", damage, angle, pitch, pspeed);

   switch((*count)++)
   {
   default:
   case 0: case 1: ACS_SetActorState(tid, s"SpawnD"); break;
   case 2: case 3: ACS_SetActorState(tid, s"SpawnA"); break;
   case 4: case 5: ACS_SetActorState(tid, s"SpawnK"); break;
   case 6: case 7: ACS_SetActorState(tid, s"SpawnK"); break;
   case 8: case 9: ACS_SetActorState(tid, s"SpawnA"); break;
   };

   if(*count == 10) *count = 0;

   if(*count & 1)
      ACS_TakeInventory(s"Clip", 1);
}

//
// DUD_FirePlasma
//
[[call("ScriptS"), extern("ACS")]]
void DUD_FirePlasma(void)
{
   int damage =  5 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CELL);
   int pspeed = 25 * DUD_GetPSpeedFactor(ACS_PlayerNumber(), AMMO_CELL);

   DU_FireMissile(0, s"DUD_PlasmaBall", damage, ACS_GetActorAngle(0),
      ACS_GetActorPitch(0), pspeed);

   ACS_TakeInventory(s"Cell", 1);
}

//
// DUD_FistDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUD_FistDamage(void)
{
   int base = ACS_CheckInventory(s"DUD_Berserk") * 5 + 2;

   if(DUD_AbilitySelected[ACS_PlayerNumber()][0] == &DUD_Ability[0][ABIL1_BERSERK])
      base += 5;

   return ACS_Random(base, base * 10);
}

//
// DUD_LasgunDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUD_LasgunDamage(void)
{
   return 50 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CELL);
}

//
// DUD_PistolDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUD_PistolDamage(void)
{
   return 10 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CLIP);
}

//
// DUD_RailRifleDamage
//
[[call("ScriptS"), extern("ACS")]]
int DUD_RailRifleDamage(void)
{
   return 100 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_CELL);
}

//
// DUD_RocketLauncherFire
//
[[call("ScriptS"), extern("ACS")]]
void DUD_RocketLauncherFire(void)
{
   int damage =  20 * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_RCKT);
   int radius = 128 * DUD_GetRadiusFactor(ACS_PlayerNumber(), AMMO_RCKT);
   int pspeed =  20 * DUD_GetPSpeedFactor(ACS_PlayerNumber(), AMMO_RCKT);

   int tid = DU_FireMissile(0, s"DUD_Rocket", damage, ACS_GetActorAngle(0),
      ACS_GetActorPitch(0), pspeed);

   ACS_SetThingSpecial(tid, 0, radius, 0, 0, 0, 0);

   ACS_TakeInventory(s"RocketAmmo", 1);
}

//
// DUD_ShotgunPellets
//
[[call("ScriptS"), extern("ACS")]]
int DUD_ShotgunPellets(int base)
{
   return base * DUD_GetDamageFactor(ACS_PlayerNumber(), AMMO_SHEL);
}

// EOF

