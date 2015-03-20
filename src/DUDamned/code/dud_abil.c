//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Ability functions.
//
//-----------------------------------------------------------------------------

#include "dud_abil.h"

#include "du_menu.h"

#include <stdlib.h>

//----------------------------------------------------------------------------|
// Defines                                                                    |
//

#define SCREEN_W 640
#define SCREEN_H 400

#define MENUBOX_W 64
#define MENUBOX_H 40

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

#define DUD_MenuInitAbility(button, slot, abil) \
   /* Rendering */ \
   button.x     = MENUBOX_W * (slot + 7); \
   button.y     = MENUBOX_H * (abil + 1); \
   button.w     = MENUBOX_W; \
   button.h     = MENUBOX_H; \
   button.txt   = DUD_Ability[slot][abil].nameIco; \
   button.img   = NULL; \
   button.color = CR_BRICK; \
   \
   /* Callbacks */ \
   button.click = DUD_ClickAbility; \
   button.hover = DUD_HoverAbility; \
   \
   /* Misc Data */ \
   button.next = &button + 1; \
   button.data = (void *)&DUD_Ability[slot][abil]

#define DUD_MenuInitAbilities(button, slot) \
   DUD_MenuInitAbility(button[0], slot, 0); \
   DUD_MenuInitAbility(button[1], slot, 1); \
   DUD_MenuInitAbility(button[2], slot, 2); \
   DUD_MenuInitAbility(button[3], slot, 3); \
   DUD_MenuInitAbility(button[4], slot, 4); \
   DUD_MenuInitAbility(button[5], slot, 5); \
   DUD_MenuInitAbility(button[6], slot, 6)

#define DUD_MenuInitWeapon(button, slot, weap) \
   /* Rendering */ \
   button.x     = MENUBOX_W * (slot + 0); \
   button.y     = MENUBOX_H * (weap + 3); \
   button.w     = MENUBOX_W; \
   button.h     = MENUBOX_H; \
   button.txt   = DUD_Weapon[slot][weap].nameIco; \
   button.img   = NULL; \
   button.color = CR_BRICK; \
   \
   /* Callbacks */ \
   button.click = DUD_ClickWeapon; \
   button.hover = DUD_HoverWeapon; \
   \
   /* Misc Data */ \
   button.next = &button + 1; \
   button.data = (void *)&DUD_Weapon[slot][weap]

#define DUD_MenuInitWeapons(button, slot) \
   DUD_MenuInitWeapon(button[0], slot, 0); \
   DUD_MenuInitWeapon(button[1], slot, 1); \
   DUD_MenuInitWeapon(button[2], slot, 2)


//----------------------------------------------------------------------------|
// Static Variables                                                           |
//

[[no_init]]
static MenuButton DUD_MenuAbility[MAX_PLAYERS][3][ABILMAX];
[[no_init]]
static MenuButton DUD_MenuWeapon[MAX_PLAYERS][7][WEAPMAX];

[[no_init]]
static MenuText DUD_MenuName[MAX_PLAYERS];
[[no_init]]
static MenuText DUD_MenuDesc[MAX_PLAYERS];

static Menu DUD_Menu[MAX_PLAYERS];


//----------------------------------------------------------------------------|
// Global Variables                                                           |
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
         .ammoRegen = {9, 3, 1, 0},
         .desc      = s"Gives basic ammo regeneration.",
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
         .ammoRegen = {5, 3, 1, 3},
         .desc      = s"Gives ammo regeneration.",
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
         .ammoRegen = {1, 3, 1, 9},
         .desc      = s"Gives advanced ammo regeneration.",
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
// Static Functions                                                           |
//

//
// DUD_ClickAbility
//
[[call("ScriptS")]]
static void DUD_ClickAbility(MenuButton *button)
{
   int pnum = ACS_PlayerNumber(), slot;

   dud_ability_t  *ability = (dud_ability_t *)button->data;
   dud_ability_t **select;

   if(!ability->used) return;

   // Figure out which slot number this ability is for.
   for(slot = 0; ability >= &DUD_Ability[slot+1][0]; ++slot) {}

   select = &DUD_AbilitySelected[pnum][slot];

   // Clear old ability's button.
   DUD_MenuAbility[pnum][slot][*select - &DUD_Ability[slot][0]].color = CR_BRICK;

   // Make this button's ability selected.
   button->color = CR_GOLD;
   *select = ability;
}

//
// DUD_ClickWeapon
//
[[call("ScriptS")]]
static void DUD_ClickWeapon(MenuButton *button)
{
   int pnum = ACS_PlayerNumber(), slot;

   dud_weapon_t  *weapon = (dud_weapon_t *)button->data;
   dud_weapon_t **select;

   if(!weapon->used) return;

   // Figure out which slot number this weapon is for.
   for(slot = 0; weapon >= &DUD_Weapon[slot+1][0]; ++slot);

   select = &DUD_WeaponSelected[pnum][slot];

   // Clear old weapon's button.
   DUD_MenuWeapon[pnum][slot][*select - &DUD_Weapon[slot][0]].color = CR_BRICK;

   // Make this button's ability selected.
   button->color = CR_GOLD;
   *select = weapon;
}

//
// DUD_HoverAbility
//
[[call("ScriptS")]]
static void DUD_HoverAbility(MenuButton *button)
{
   int pnum = ACS_PlayerNumber();

   DUD_MenuName[pnum].txt = ((dud_ability_t *)button->data)->name;
   DUD_MenuDesc[pnum].txt = ((dud_ability_t *)button->data)->desc;
}

//
// DUD_HoverWeapon
//
[[call("ScriptS")]]
static void DUD_HoverWeapon(MenuButton *button)
{
   int pnum = ACS_PlayerNumber();

   DUD_MenuName[pnum].txt = ((dud_weapon_t *)button->data)->name;
   DUD_MenuDesc[pnum].txt = ((dud_weapon_t *)button->data)->desc;
}

//
// DUD_MenuInit
//
static void DUD_MenuInit(int pnum)
{
   // Selected abilities.
   DUD_AbilitySelected[pnum][0] = &DUD_Ability[0][0];
   DUD_AbilitySelected[pnum][1] = &DUD_Ability[1][0];
   DUD_AbilitySelected[pnum][2] = &DUD_Ability[2][0];

   // Selected weapons.
   DUD_WeaponSelected[pnum][0] = &DUD_Weapon[0][0];
   DUD_WeaponSelected[pnum][1] = &DUD_Weapon[1][0];
   DUD_WeaponSelected[pnum][2] = &DUD_Weapon[2][0];
   DUD_WeaponSelected[pnum][3] = &DUD_Weapon[3][0];
   DUD_WeaponSelected[pnum][4] = &DUD_Weapon[4][0];
   DUD_WeaponSelected[pnum][5] = &DUD_Weapon[5][0];
   DUD_WeaponSelected[pnum][6] = &DUD_Weapon[6][0];

   // Menu root.
   DUD_Menu[pnum].w       = SCREEN_W;
   DUD_Menu[pnum].h       = SCREEN_H;
   DUD_Menu[pnum].font    = s"CONFONT";
   DUD_Menu[pnum].buttons = &DUD_MenuAbility[pnum][0][0];
   DUD_Menu[pnum].labels  = NULL;
   DUD_Menu[pnum].texts   = &DUD_MenuName[pnum];

   // Abilities menu stuff.
   DUD_MenuInitAbilities(DUD_MenuAbility[pnum][0], 0);
   DUD_MenuInitAbilities(DUD_MenuAbility[pnum][1], 1);
   DUD_MenuInitAbilities(DUD_MenuAbility[pnum][2], 2);

   DUD_MenuAbility[pnum][0][ABILMAX-1].next = &DUD_MenuAbility[pnum][1][0];
   DUD_MenuAbility[pnum][1][ABILMAX-1].next = &DUD_MenuAbility[pnum][2][0];
   DUD_MenuAbility[pnum][2][ABILMAX-1].next = &DUD_MenuWeapon[pnum][0][0];

   DUD_MenuAbility[pnum][0][0].color = CR_GOLD;
   DUD_MenuAbility[pnum][1][0].color = CR_GOLD;
   DUD_MenuAbility[pnum][2][0].color = CR_GOLD;

   // Weapons menu stuff.
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][0], 0);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][1], 1);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][2], 2);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][3], 3);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][4], 4);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][5], 5);
   DUD_MenuInitWeapons(DUD_MenuWeapon[pnum][6], 6);

   DUD_MenuWeapon[pnum][0][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][1][0];
   DUD_MenuWeapon[pnum][1][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][2][0];
   DUD_MenuWeapon[pnum][2][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][3][0];
   DUD_MenuWeapon[pnum][3][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][4][0];
   DUD_MenuWeapon[pnum][4][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][5][0];
   DUD_MenuWeapon[pnum][5][WEAPMAX-1].next = &DUD_MenuWeapon[pnum][6][0];
   DUD_MenuWeapon[pnum][6][WEAPMAX-1].next = NULL;

   DUD_MenuWeapon[pnum][0][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][1][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][2][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][3][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][4][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][5][0].color = CR_GOLD;
   DUD_MenuWeapon[pnum][6][0].color = CR_GOLD;

   // Miscellaneous menu stuff.
   DUD_MenuName[pnum].x     = 0;
   DUD_MenuName[pnum].y     = MENUBOX_H * 6 + 8;
   DUD_MenuName[pnum].txt   = NULL;
   DUD_MenuName[pnum].color = CR_BRICK;
   DUD_MenuName[pnum].next  = &DUD_MenuDesc[pnum];

   DUD_MenuDesc[pnum].x     = 0;
   DUD_MenuDesc[pnum].y     = MENUBOX_H * 6 + 24;
   DUD_MenuDesc[pnum].txt   = NULL;
   DUD_MenuDesc[pnum].color = CR_BRICK;
   DUD_MenuDesc[pnum].next  = NULL;
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUD_AddWeapon
//
void DUD_AddWeapon(int slot, int weap)
{
   DUD_Weapon[slot][weap].used = true;
}

//
// DUD_DoAmmoCap
//
void DUD_DoAmmoCap(int pnum)
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
void DUD_DoAmmoRegen(int pnum)
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
      if((ammoCount[pnum][ammo] -= ammoRegen[ammo]) < 0)
      {
         ammoCount[pnum][ammo] += DUD_Ammo[ammo].regen;
         ACS_GiveInventory(DUD_Ammo[ammo].nameDec, 1);
      }
   }
}

//
// DUD_DoArmorRegen
//
void DUD_DoArmorRegen(int pnum)
{
   static int armorCount[MAX_PLAYERS];
   int armorRegen = 0;

   for(dud_ability_t **itr = DUD_AbilitySelected[pnum], **end = itr + 3;
      itr != end; ++itr)
   {
      armorRegen += (*itr)->armorRegen;
   }

   if((armorCount[pnum] -= armorRegen) < 0)
   {
      armorCount[pnum] += 35*60;
      ACS_GiveInventory(s"ArmorBonus", 1);
   }
}

//
// DUD_DoHealthRegen
//
void DUD_DoHealthRegen(int pnum)
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

   while((healthCount[pnum] -= healthRegen) < 0)
   {
      healthCount[pnum] += 35*60;
      ACS_GiveInventory(s"HealthBonus", 1);
   }
}

//
// DUD_DoJumpHeight
//
void DUD_DoJumpHeight(int pnum)
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
void DUD_DoRunSpeed(int pnum)
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

//
// DUD_Main
//
[[call("ScriptS"), script("Enter")]]
void DUD_Main(void)
{
   int pnum = ACS_PlayerNumber();

   if(!DUD_Menu[pnum].w)
      DUD_MenuInit(pnum);

   while(true)
   {
      ACS_Delay(1);

      if(ACS_GetPlayerInput(-1, INPUT_BUTTONS) & BT_USER1)
      {
         if(!(ACS_GetPlayerInput(-1, INPUT_OLDBUTTONS) & BT_USER1))
         {
            ACS_SetPlayerProperty(0, true, PROP_TOTALLYFROZEN);
            DU_MenuBegin(&DUD_Menu[pnum]);
         }
         else
         {
            DU_MenuRun(&DUD_Menu[pnum]);
         }
      }
      else
      {
         if(ACS_GetPlayerInput(-1, INPUT_OLDBUTTONS) & BT_USER1)
         {
            ACS_SetPlayerProperty(0, false, PROP_TOTALLYFROZEN);
            DU_MenuEnd(&DUD_Menu[pnum]);
         }
         else
         {
            // Apply selected abilities.

            DUD_DoAmmoCap(pnum);
            DUD_DoAmmoRegen(pnum);
            DUD_DoArmorRegen(pnum);
            DUD_DoHealthRegen(pnum);
            DUD_DoJumpHeight(pnum);
            DUD_DoRunSpeed(pnum);
            DUD_DoWeaponSelect(pnum);
         }
      }
   }
}

// EOF

