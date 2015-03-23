//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Weapon and ammo handling.
//
//-----------------------------------------------------------------------------

#include "dud_weap.h"

#include "dud_abil.h"

#include <stddef.h>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// DUD_Ammo
//
dud_ammo_t DUD_Ammo[AMMOMAX] =
{
   // AMMO_CLIP
   {
      AMMO_CLIP,
      180,
      350,
      s"Clip",
      s"Clip",
   },

   // AMMO_SHEL
   {
      AMMO_SHEL,
      60,
      1050,
      s"Shell",
      s"Shell",
   },

   // AMMO_RCKT
   {
      AMMO_RCKT,
      20,
      700,
      s"Rocket",
      s"RocketAmmo",
   },

   // AMMO_CELL
   {
      AMMO_CELL,
      240,
      350,
      s"Cell",
      s"Cell",
   },
};

//
// DUD_Weapon
//
dud_weapon_t DUD_Weapon[7][WEAPMAX] =
{
   // WEAP1
   {
      // WEAP1_FIST
      {
         true,
         AMMOMAX,
         s"Lets you punch zombies with your bare hands like real\n"
         s"men do. Heck yeah!",
         NULL,
         NULL,
         s"Fist",
         s"DUD_Fist",
         s"FIST",
      },

      // WEAP1_CSAW
      {
         true,
         AMMOMAX,
         s"Chops up demons real quick-like.",
         NULL,
         NULL,
         s"Chainsaw",
         s"DUD_Chainsaw",
         s"C. SAW",
      },

      // WEAP1_CFIST
      {
         true,
         AMMOMAX,
         s"Smash and chop with the finest of them.",
         NULL,
         NULL,
         s"Chainfist",
         s"DUD_Chainfist",
         s"C. FIST",
      },
   },

   // WEAP2
   {
      // WEAP2_PIST
      {
         true,
         AMMO_CLIP,
         s"Shoots bullets.",
         NULL,
         NULL,
         s"Pistol",
         s"DUD_Pistol",
         s"PISTOL",
      },
   },

   // WEAP3
   {
      // WEAP3_SHTG
      {
         true,
         AMMO_SHEL,
         s"Shoots shells.",
         NULL,
         NULL,
         s"Shotgun",
         s"DUD_Shotgun",
         s"SHTGUN",
      },

      // WEAP3_SHT2
      {
         true,
         AMMO_SHEL,
         s"Shoots two shells. Slower than the shotgun, but more\npowerful.",
         NULL,
         NULL,
         s"Super Shotgun",
         s"DUD_SuperShotgun",
         s"SHT2",
      },
   },

   // WEAP4
   {
      // WEAP4_CGUN
      {
         true,
         AMMO_CLIP,
         s"Shoots bullets fast.",
         NULL,
         NULL,
         s"Chaingun",
         s"DUD_Chaingun",
         s"C. GUN",
      },

      // WEAP4_DGUN
      {
         true,
         AMMO_CLIP,
         s"Shoots dakka.",
         NULL,
         NULL,
         s"Dakkagun",
         s"DUD_Dakkagun",
         s"DAKKA",
      },
   },

   // WEAP5
   {
      // WEAP5_RCKT
      {
         true,
         AMMO_RCKT,
         s"Shoots rockets.",
         NULL,
         NULL,
         s"Rocket Launcher",
         s"DUD_RocketLauncher",
         s"ROCKET",
      },
   },

   // WEAP6
   {
      // WEAP6_PLAS
      {
         true,
         AMMO_CELL,
         s"Shoots plasma fast.",
         NULL,
         NULL,
         s"Plasma Rifle",
         s"DUD_PlasmaRifle",
         s"PLASMA",
      },

      // WEAP6_LASG
      {
         true,
         AMMO_CELL,
         s"Shoots plasma hitscans. No affiliation with 40K. Nope.",
         NULL,
         NULL,
         s"Lasgun",
         s"DUD_Lasgun",
         s"LASGUN",
      },
   },

   // WEAP7
   {
      // WEAP7_BFG9
      {
         true,
         AMMO_CELL,
         s"Shoots plasma.",
         NULL,
         NULL,
         s"B.F.G. 9000",
         s"DUD_BFG9000",
         s"B.F.G.",
      },

      // WEAP7_RAIL
      {
         true,
         AMMO_CELL,
         s"Shoots piercing plasma.",
         NULL,
         NULL,
         s"Rail Rifle",
         s"DUD_RailRifle",
         s"RAIL",
      },
   },
};

dud_weapon_t *DUD_WeaponSelected[MAX_PLAYERS][7];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUD_AddWeapon
//
void DUD_AddWeapon(int slot, int weap)
{
   DUD_Weapon[slot][weap].used = true;
}

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
   }

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

