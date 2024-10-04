//-----------------------------------------------------------------------------
//
// Copyright (C) 2015, 2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// DUD menu.
//
//-----------------------------------------------------------------------------

#include "du_menu.h"

#include "dud_abil.h"

#include <stddef.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define SCREEN_W 640
#define SCREEN_H 400

#define MENUBOX_W 64
#define MENUBOX_H 40

#define DUD_MenuInitAbility(button, slot, abil) \
   /* Rendering */ \
   button.x     = MENUBOX_W * (slot + 7); \
   button.y     = MENUBOX_H * (abil + 1); \
   button.w     = MENUBOX_W; \
   button.h     = MENUBOX_H; \
   button.txt   = DUD_Ability[slot][abil].nameIco; \
   button.img   = s"ABILBACK"; \
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
   button.img   = s"WEAPBACK"; \
   button.color = CR_BRICK; \
   \
   /* Callbacks */ \
   button.click = DUD_ClickWeapon; \
   button.hover = DUD_HoverWeapon; \
   button.run   = DUD_RunWeapon; \
   \
   /* Misc Data */ \
   button.next = &button + 1; \
   button.data = (void *)&DUD_Weapon[slot][weap]

#define DUD_MenuInitWeapons(button, slot) \
   DUD_MenuInitWeapon(button[0], slot, 0); \
   DUD_MenuInitWeapon(button[1], slot, 1); \
   DUD_MenuInitWeapon(button[2], slot, 2)


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static MenuButton DUD_MenuAbility[MAX_PLAYERS][3][ABILMAX];
static MenuButton DUD_MenuWeapon[MAX_PLAYERS][7][WEAPMAX];

static MenuText DUD_MenuName[MAX_PLAYERS];
static MenuText DUD_MenuDesc[MAX_PLAYERS];

static Menu DUD_Menu[MAX_PLAYERS];


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

   dud_weapon_t *weapon = (dud_weapon_t *)button->data;

   if(!weapon->used || !weapon->have) return;

   // Figure out which slot number this weapon is for.
   for(slot = 0; weapon >= &DUD_Weapon[slot+1][0]; ++slot);

   // Make this button's ability selected.
   DUD_WeaponSelected[pnum][slot] = weapon;
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

   dud_weapon_t *weapon = (dud_weapon_t *)button->data;

   if(weapon->used)
   {
      DUD_MenuName[pnum].txt = weapon->name;
      DUD_MenuDesc[pnum].txt = weapon->desc;
   }
   else
   {
      DUD_MenuName[pnum].txt = s"";
      DUD_MenuDesc[pnum].txt = s"";
   }
}

//
// DUD_RunWeapon
//
[[call("ScriptS")]]
static void DUD_RunWeapon(MenuButton *button)
{
   int pnum = ACS_PlayerNumber(), slot;

   dud_weapon_t *weapon = (dud_weapon_t *)button->data;
   dud_weapon_t *select;

   // Figure out which slot number this weapon is for.
   for(slot = 0; weapon >= &DUD_Weapon[slot+1][0]; ++slot);

   select = DUD_WeaponSelected[pnum][slot];

   if(weapon->used)
   {
      button->txt = weapon->nameIco;

      if(weapon == select)
         button->color = CR_GOLD;
      else if(weapon->have)
         button->color = CR_BRICK;
      else
         button->color = CR_GRAY;
   }
   else
   {
      button->txt   = NULL;
      button->color = CR_GRAY;
   }
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
// Extern Functions                                                           |
//

//
// DUD_MenuLoop
//
[[call("ScriptS"), script("enter")]]
void DUD_MenuLoop(void)
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
            DUD_DoAbilities(pnum, 1);

            DUD_DoWeaponSelect(pnum);
         }
      }
   }
}

// EOF

