//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Spell functions and data.
//
//-----------------------------------------------------------------------------

#include "dum_spel.h"

#include <stdio.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DUM_SpellBasic_Cast
//
[[call("ScriptS")]]
static void DUM_SpellBasic_Cast(SpellData const *spell)
{
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
}

//
// DUM_SpellBurst_Cast
//
[[call("ScriptS")]]
static void DUM_SpellBurst_Cast(SpellData const *spell)
{
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
   ACS_Delay(3);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
   ACS_Delay(3);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
}

//
// DUM_SpellCircle_Cast
//
[[call("ScriptS")]]
static void DUM_SpellCircle_Cast(SpellData const *spell)
{
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.000k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.125k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.250k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.375k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.500k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.625k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.750k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 0.875k, ACS_GetActorPitch(0), 15);
}

//
// DUM_SpellSpread_Cast
//
[[call("ScriptS")]]
static void DUM_SpellSpread_Cast(SpellData const *spell)
{
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) + 1/16.0k, ACS_GetActorPitch(0), 15);
   DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
      ACS_GetActorAngle(0) - 1/16.0k, ACS_GetActorPitch(0), 15);
}

//
// DUM_SpellStream_Cast
//
[[call("ScriptS")]]
static void DUM_SpellStream_Cast(SpellData const *spell)
{
   int i = 15;

   while(i-- && (ACS_GetPlayerInput(-1, INPUT_BUTTONS) & BT_USER2))
   {
      DU_FireMissile(ACS_ActivatorTID(), s"DUM_BasicBolt", 3,
         ACS_GetActorAngle(0), ACS_GetActorPitch(0), 15);
      ACS_Delay(2);
   }
}


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

SpellData *DUM_SpellSelect[MAX_PLAYERS];

//
// DUM_SpellBasic
//
SpellData DUM_SpellBasic =
{
   s"BASIC",
   DUM_SpellBasic_Cast,
   NULL,
   NULL,

   &DUM_SpellSpread,
   &DUM_SpellCircle,
   &DUM_SpellBurst,
   &DUM_SpellBurst,
};

//
// DUM_SpellBurst
//
SpellData DUM_SpellBurst =
{
   s"BURST",
   DUM_SpellBurst_Cast,
   NULL,
   NULL,

   &DUM_SpellStream,
   &DUM_SpellStream,
   &DUM_SpellBurst,
   &DUM_SpellBurst,
};

//
// DUM_SpellCircle
//
SpellData DUM_SpellCircle =
{
   s"CIRCLE",
   DUM_SpellCircle_Cast,
   NULL,
   NULL,

   &DUM_SpellCircle,
   &DUM_SpellCircle,
   &DUM_SpellCircle,
   &DUM_SpellCircle,
};

//
// DUM_SpellSpread
//
SpellData DUM_SpellSpread =
{
   s"SPREAD",
   DUM_SpellSpread_Cast,
   NULL,
   NULL,

   &DUM_SpellSpread,
   &DUM_SpellSpread,
   &DUM_SpellSpread,
   &DUM_SpellSpread,
};

//
// DUM_SpellStream
//
SpellData DUM_SpellStream =
{
   s"STREAM",
   DUM_SpellStream_Cast,
   NULL,
   NULL,

   &DUM_SpellStream,
   &DUM_SpellStream,
   &DUM_SpellStream,
   &DUM_SpellStream,
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUM_SpellMenu
//
void DUM_SpellMenu(int buttons)
{
   SpellData **spell = &DUM_SpellSelect[ACS_PlayerNumber()];

   if(buttons & BT_FORWARD)
   {
      *spell = (*spell)->up;
      if((*spell)->funcSelect)
         (*spell)->funcSelect(*spell);
   }

   if(buttons & BT_BACK)
   {
      *spell = (*spell)->down;
      if((*spell)->funcSelect)
         (*spell)->funcSelect(*spell);
   }

   if(buttons & (BT_LEFT|BT_MOVELEFT))
   {
      *spell = (*spell)->left;
      if((*spell)->funcSelect)
         (*spell)->funcSelect(*spell);
   }

   if(buttons & (BT_RIGHT|BT_MOVERIGHT))
   {
      *spell = (*spell)->right;
      if((*spell)->funcSelect)
         (*spell)->funcSelect(*spell);
   }

   ACS_BeginPrint();
   __nprintf("%S\n|\n%S -- %S -- %S\n|\n%S",
                            (*spell)->up->name,
      (*spell)->left->name, (*spell)->name, (*spell)->right->name,
                            (*spell)->down->name);
   ACS_EndPrint();
}

// EOF

