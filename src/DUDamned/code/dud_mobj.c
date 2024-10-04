//-----------------------------------------------------------------------------
//
// Copyright (C) 2015, 2024 David Hill
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
#include <stdio.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DUD_SpawnAlpha
//
[[call("ScriptS")]]
static void DUD_SpawnAlpha(char c, accum angle, accum x, accum y, accum z,
   accum velx, accum vely, accum velz, int timeFull, int timeFade)
{
   __str state = (ACS_BeginPrint(), __nprintf("Spawn%c", c), ACS_EndStrParam());

   int tid = DU_MakeTID();

   // If spawn fails, just silently fail.
   if(!ACS_SpawnForced(s"DUD_AlphapelletGFX", x, y, z, tid))
      return;

   // Similarly for setting state, which occurs for unimplemented characters.
   if(!ACS_SetActorState(tid, state))
   {
      ACS_Thing_Remove(tid);
      return;
   }

   ACS_SetActorAngle(tid, angle);
   ACS_SetActorVelocity(tid, velx, vely, velz, 0, 0);

   ACS_Delay(timeFull);

   // Fade out over timeFade tics.
   ACS_SetActorProperty(tid, APROP_RenderStyle, STYLE_Translucent);
   for(int fade = timeFade; fade--;)
   {
      ACS_SetActorPropertyFixed(tid, APROP_Alpha, (accum)fade / timeFade);
      ACS_Delay(1);
   }

   ACS_Thing_Remove(tid);
}

//
// DUD_SpawnAlphaString
//
static void DUD_SpawnAlphaString(__str s, accum angle, accum x, accum y, accum z,
   accum velx, accum vely, accum velz, int timeFull, int timeFadeMin, int timeFadeMax)
{
   int len = ACS_StrLen(s);

   accum dist = -len * 4;
   accum cang = angle + 0.25k;

   for(int i = 0; i != len; ++i, dist += 8)
   {
      accum cx = x + ACS_Cos(cang) * dist;
      accum cy = y + ACS_Sin(cang) * dist;

      int timeFade = ACS_Random(timeFadeMin, timeFadeMax);

      DUD_SpawnAlpha(s[i], angle, cx, cy, z, velx, vely, velz, timeFull, timeFade);
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// DUD_Death
//
[[call("ScriptS"), extern("ACS")]]
void DUD_Death(void)
{
   int healthBase = ACS_GetActorProperty(0, APROP_SpawnHealth);
   int health     = ACS_GetActorProperty(0, APROP_Health);
   int tics       = sqrtf(healthBase - health) * 15 + 1;

   for(int pnum = 0; pnum != MAX_PLAYERS; ++pnum)
   {
      if(!ACS_PlayerInGame(pnum)) continue;

      DUD_DoAbilities(pnum, tics);
   }
}

//
// DUD_DeathAlphabet
//
// Writes the activator's tag in floating text with the activator's facing.
//
[[call("ScriptS"), extern("ACS")]]
void DUD_DeathAlphabet(void)
{
   __str tag   = ACS_GetActorPropertyString(0, APROP_Nametag);
   accum angle = ACS_GetActorAngle(0);
   accum x     = ACS_GetActorX(0);
   accum y     = ACS_GetActorY(0);
   accum z     = ACS_GetActorZ(0) + 40;

   DU_Position tpos = DU_GetTargetPosition(0);
   accum       tang = ACS_VectorAngle(tpos.x - x, tpos.y - y);

   accum sang = DU_AverageAngle(angle, tang);

   DUD_SpawnAlphaString(tag, sang, x, y, z, 0, 0, 1, 35, 35, 70);
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

   int healthMin = -healthBase * (ACS_GameSkill() + 1);

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
   int   tid = DU_MakeTID();
   if(ACS_Spawn(type, x, y, z, tid, ACS_Random(0, 255)))
   {
      ACS_SetActorState(tid, s"Wander");
      ACS_Spawn(s"TeleportFog", x, y, z, 0, 0);

      // Deplete health. Because summoning enemies infinitely would be unfair.
      int healthNew  = health - ACS_Random(sqrtf(healthBase), healthBase);
      ACS_SetActorProperty(0, APROP_Health, healthNew);
   }

   // Branch to start of Respawne state sequence.
   ACS_SetActorState(0, s"Respawne");
}

// EOF

