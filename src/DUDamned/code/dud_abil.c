//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Ability handling.
//
//-----------------------------------------------------------------------------

#include "dud_abil.h"

#include <stddef.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define DUD_AbilityDefaults \
   false, \
   {0, 0, 0, 0}, \
   {0, 0, 0, 0}, \
   {1, 1, 1, 1}, \
   {1, 1, 1, 1}, \
   {1, 1, 1, 1}, \
   0, 0, \
   1, 1, \
   NULL, \
   NULL, \
   NULL, \
   NULL, \
   NULL


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

//
// DUD_Ability
//
dud_ability_t DUD_Ability[3][ABILMAX] =
{
   // ABIL1
   {
      // ABIL1_AMMO
      {
         DUD_AbilityDefaults,

         .used      = true,
         .ammoRegen = {9, 3, 1, 3},
         .desc      = s"Gives CLIP ammo regeneration.",
         .name      = s"Ammo Regen I",
         .nameIco   = s"AMMO-1",
      },

      // ABIL1_DAMAGE_CLIP
      {
         DUD_AbilityDefaults,

         .used    = true,
         .damage  = {1.50k, 1.00k, 1.00k, 1.00k},
         .desc    = s"Gives damage increase to CLIP ammo weapons.",
         .name    = s"Damage-CLIP",
         .nameIco = s"CLIP",
      },

      // ABIL1_DAMAGE_SHEL
      {
         DUD_AbilityDefaults,

         .used    = true,
         .damage  = {1.00k, 1.50k, 1.00k, 1.00k},
         .desc    = s"Gives damage increase to SHEL ammo weapons.",
         .name    = s"Damage-SHEL",
         .nameIco = s"SHEL",
      },

      // ABIL1_DAMAGE_RCKT
      {
         DUD_AbilityDefaults,

         .used    = true,
         .damage  = {1.00k, 1.00k, 1.50k, 1.00k},
         .desc    = s"Gives damage increase to RCKT ammo weapons.",
         .name    = s"Damage-RCKT",
         .nameIco = s"RCKT",
      },

      // ABIL1_DAMAGE_CELL
      {
         DUD_AbilityDefaults,

         .used    = true,
         .damage  = {1.00k, 1.00k, 1.00k, 1.50k},
         .desc    = s"Gives damage increase to CELL ammo weapons.",
         .name    = s"Damage-CELL",
         .nameIco = s"CELL",
      },

      // ABIL1_BERSERK
      {
         DUD_AbilityDefaults,

         .used        = true,
         .healthRegen = 15,
         .speed       = 1.125k,
         .jumpz       = 1.25k,
         .desc        =
            s"Increases movement speed, jump height, fist damage,\n"
            s"and health regen.",
         .name        = s"Berserk Pack",
         .nameIco     = s"BERSERK",
      },

      // ABIL1_7
      {DUD_AbilityDefaults},
   },

   // ABIL2
   {
      // ABIL2_AMMO
      {
         DUD_AbilityDefaults,

         .used      = true,
         .ammoRegen = {3, 9, 1, 3},
         .desc      = s"Gives SHEL ammo regeneration.",
         .name      = s"Ammo Regen II",
         .nameIco   = s"AMMO-2",
      },

      // ABIL2_ARMOR
      {
         DUD_AbilityDefaults,

         .used       = true,
         .armorRegen = 60,
         .desc       = s"Gives armor regeneration.",
         .name       = s"Armor Regen",
         .nameIco    = s"ARMOR",
      },

      // ABIL2_RADIUS
      {
         DUD_AbilityDefaults,

         .used    = true,
         .radius  = {1.50k, 1.50k, 1.50k, 1.50k},
         .desc    = s"Increases explosion radius by 50%.",
         .name    = s"Explosion Radius",
         .nameIco = s"RADIUS",
      },

      // ABIL2_PSPEED
      {
         DUD_AbilityDefaults,

         .used    = true,
         .pspeed  = {2.00k, 2.00k, 2.00k, 2.00k},
         .desc    = s"Increases projectile speed by 100%.",
         .name    = s"Projectile Speed",
         .nameIco = s"PSPEED",
      },

      // ABIL2_5
      {DUD_AbilityDefaults},

      // ABIL2_6
      {DUD_AbilityDefaults},

      // ABIL2_7
      {DUD_AbilityDefaults},
   },

   // ABIL3
   {
      // ABIL3_AMMO
      {
         DUD_AbilityDefaults,

         .used      = true,
         .ammoRegen = {3, 3, 1, 9},
         .desc      = s"Gives CELL ammo regeneration.",
         .name      = s"Ammo Regen III",
         .nameIco   = s"AMMO-3",
      },

      // ABIL3_HEALTH
      {
         DUD_AbilityDefaults,

         .used        = true,
         .healthRegen = 60,
         .desc        = s"Gives health regeneration.",
         .name        = s"Health Regen",
         .nameIco     = s"HEALTH",
      },

      // ABIL3_DAMAGE
      {
         DUD_AbilityDefaults,

         .used    = true,
         .damage  = {1.50k, 1.50k, 1.50k, 1.50k},
         .desc    = s"Gives damage increase to all ammo weapons.",
         .name    = s"Damage-All",
         .nameIco = s"DAMAGE",
      },

      // ABIL3_SPEED
      {
         DUD_AbilityDefaults,

         .used    = true,
         .speed   = 1.50k,
         .desc    = s"Increases movement speed.",
         .name    = s"Speed",
         .nameIco = s"SPEED",
      },

      // ABIL3_JUMP
      {
         DUD_AbilityDefaults,

         .used    = true,
         .jumpz   = 1.75k,
         .desc    = s"Increases jump height",
         .name    = s"Jump",
         .nameIco = s"JUMP",
      },

      // ABIL3_6
      {DUD_AbilityDefaults},

      // ABIL3_7
      {DUD_AbilityDefaults},
   },
};

dud_ability_t *DUD_AbilitySelected[MAX_PLAYERS][3];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUD_DoAbilities
//
void DUD_DoAbilities(int pnum, int tics)
{
   DUD_DoAmmoCap(pnum, tics);
   DUD_DoAmmoRegen(pnum, tics);
   DUD_DoArmorRegen(pnum, tics);
   DUD_DoHealthRegen(pnum, tics);
   DUD_DoJumpHeight(pnum, tics);
   DUD_DoRunSpeed(pnum, tics);
}

//
// DUD_DoAmmoCap
//
void DUD_DoAmmoCap(int pnum, int)
{
   accum ammoBase = 1 + ACS_CheckInventory(s"DUD_Backpack") * 0.5k;

   accum ammoCap[AMMOMAX] = {ammoBase, ammoBase, ammoBase, ammoBase};

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      for(int ammo = 0; ammo < AMMOMAX; ++ammo)
         ammoCap[ammo] += (*itr)->ammoCap[ammo];
   }

   for(int ammo = 0; ammo < AMMOMAX; ++ammo)
   {
      int   cap     = DUD_Ammo[ammo].cap * ammoCap[ammo];
      __str nameDec = DUD_Ammo[ammo].nameDec;

      // If cap already correct, don't adjust.
      if(ACS_GetAmmoCapacity(nameDec) == cap) continue;

      ACS_SetAmmoCapacity(nameDec, cap);

      // Don't keep now-excess ammo.
      int count = ACS_CheckInventory(nameDec);
      if(count > cap)
         ACS_TakeInventory(nameDec, count - cap);
   }
}

//
// DUD_DoAmmoRegen
//
void DUD_DoAmmoRegen(int pnum, int tics)
{
   static int ammoCount[MAX_PLAYERS][AMMOMAX];

   int ammoRegen[AMMOMAX] = {0};

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      for(int ammo = 0; ammo < AMMOMAX; ++ammo)
         ammoRegen[ammo] += (*itr)->ammoRegen[ammo];
   }

   for(int ammo = 0; ammo < AMMOMAX; ++ammo)
   {
      if((ammoCount[pnum][ammo] -= ammoRegen[ammo] * tics) < 0)
      {
         ammoCount[pnum][ammo] += DUD_Ammo[ammo].regen;
         ACS_GiveInventory(DUD_Ammo[ammo].nameDec, 1);
      }
   }
}

//
// DUD_DoArmorRegen
//
void DUD_DoArmorRegen(int pnum, int tics)
{
   static int armorCount[MAX_PLAYERS];
   int armorRegen = 0;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      armorRegen += (*itr)->armorRegen;
   }

   if((armorCount[pnum] -= armorRegen * tics) < 0)
   {
      armorCount[pnum] += 35*60;
      ACS_GiveInventory(s"DUD_ArmorBonusSmall", 1);
   }
}

//
// DUD_DoHealthRegen
//
void DUD_DoHealthRegen(int pnum, int tics)
{
   static int healthCount[MAX_PLAYERS];
   int healthRegen = 0;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      healthRegen += (*itr)->healthRegen;
   }

   for(int i = ACS_CheckInventory(s"DUD_Berserk"); i--;)
      healthRegen += DUD_Ability[0][ABIL1_BERSERK].healthRegen;

   if((healthCount[pnum] -= healthRegen * tics) < 0)
   {
      healthCount[pnum] += 35*60;
      ACS_GiveInventory(s"DUD_HealthBonusSmall", 1);
   }
}

//
// DUD_DoJumpHeight
//
void DUD_DoJumpHeight(int pnum, int)
{
   accum jumpz = 8;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      jumpz *= (*itr)->jumpz;
   }

   for(int i = ACS_CheckInventory(s"DUD_Berserk"); i--;)
      jumpz *= DUD_Ability[0][ABIL1_BERSERK].jumpz;

   ACS_SetActorPropertyFixed(0, APROP_JumpZ, jumpz);
}

//
// DUD_DoRunSpeed
//
void DUD_DoRunSpeed(int pnum, int)
{
   accum speed = 1;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      speed *= (*itr)->speed;
   }

   for(int i = ACS_CheckInventory(s"DUD_Berserk"); i--;)
      speed *= DUD_Ability[0][ABIL1_BERSERK].speed;

   ACS_SetActorPropertyFixed(0, APROP_Speed, speed);
}

//
// DUD_DoWeaponSelect
//
void DUD_DoWeaponSelect(int pnum)
{
   for(int slot = 0; slot < 7; ++slot) for(int weap = 0; weap < WEAPMAX; ++weap)
   {
      dud_weapon_t const *weapon = &DUD_Weapon[slot][weap];

      if(DUD_WeaponSelected[pnum][slot] == weapon)
      {
         if(!ACS_CheckInventory(weapon->nameDec))
            ACS_GiveInventory(weapon->nameDec, 1);
      }
      else
      {
         if(ACS_CheckInventory(weapon->nameDec))
            ACS_TakeInventory(weapon->nameDec, 1);
      }
   }
}

//
// DUD_GetDamageFactor
//
accum DUD_GetDamageFactor(int pnum, int ammo)
{
   accum factor = 1;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      factor *= (*itr)->damage[ammo];
   }

   return factor;
}

//
// DUD_GetPSpeedFactor
//
accum DUD_GetPSpeedFactor(int pnum, int ammo)
{
   accum factor = 1.0;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      factor *= (*itr)->pspeed[ammo];
   }

   return factor;
}

//
// DUD_GetRadiusFactor
//
accum DUD_GetRadiusFactor(int pnum, int ammo)
{
   accum factor = 1.0;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      factor *= (*itr)->radius[ammo];
   }

   return factor;
}

// EOF

