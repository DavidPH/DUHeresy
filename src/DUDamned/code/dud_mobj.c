//-----------------------------------------------------------------------------
//
// Copyright (C) 2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Mobj functions.
//
//-----------------------------------------------------------------------------

#include "du_defs.h"

#include "dud_abil.h"

#include <math.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUD_Death
//
[[call("ScriptS"), extern("ACS")]]
void DUD_Death(void)
{
   int tics = sqrtf(ACS_GetActorProperty(0, APROP_SpawnHealth)) * 35;

   for(int pnum = 0; pnum != MAX_PLAYERS; ++pnum)
   {
      if(!ACS_PlayerInGame(pnum)) continue;

      DUD_DoAbilities(pnum, tics);
   }
}

//
// DUD_Respawne
//
[[call("ScriptS"), extern("ACS")]]
void DUD_Respawne(void)
{
   __str type = ACS_GetActorClass(0);

   int healthBase = ACS_GetActorProperty(0, APROP_SpawnHealth);
   int health     = ACS_GetActorProperty(0, APROP_Health);

   int healthMin = -healthBase * (ACS_GameSkill() + 4) / 2;

   // If the caller is too dead, then it stays dead.
   if(health < healthMin) {ACS_Thing_Remove(0); return;}

   // Set transparency based on life remaining.
   accum alpha = (double)health / healthMin;
   ACS_SetActorPropertyFixed(0, APROP_Alpha, 1 - alpha);
   ACS_SetActorProperty(0, APROP_RenderStyle, STYLE_Translucent);

   // Delay before summoning. Deader monsters spawn slower.
   int delay = (healthBase - health) * (8 - ACS_GameSkill()) * 35;
   delay = ACS_Random(sqrtf(delay), delay) + 35;

   for(; delay; --delay)
   {
      ACS_Delay(1);

      if(ACS_GetActorProperty(0, APROP_Health) > 0)
         return;
   }

   // Try to summon a new enemy.
   accum x = ACS_GetActorX(0), y = ACS_GetActorY(0), z = ACS_GetActorZ(0);
   ACS_Spawn(type, x, y, z, 0, ACS_Random(0, 255));
   ACS_Spawn(s"TeleportFog", x, y, z, 0, 0);

   // Deplete health. Afterall, summoning enemies infinitely would be unfair.
   int healthNew  = health - ACS_Random(sqrtf(healthBase), healthBase);
   ACS_SetActorProperty(0, APROP_Health, healthNew);

   // Branch to start of Respawne state sequence.
   ACS_SetActorState(0, s"Respawne");
}

// EOF

