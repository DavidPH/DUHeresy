//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Mobj functions.
//
//-----------------------------------------------------------------------------

#include "du_defs.h"

#include "duh_arti.h"

#include <math.h>


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUH_DSparilDeath
//
[[call("ScriptS"), extern("ACS")]]
void DUH_DSparilDeath(void)
{
   if(!ACS_ThingCount(T_DSPARILALONE, 0))
      ACS_Floor_LowerToNearest(666, 1);
}

//
// DUH_RaiseThingDelayed
//
[[call("ScriptS"), extern("ACS")]]
void DUH_RaiseThingDelayed(int delay)
{
   // Wait the specified amount of time.
   // However, keep checking if the caller is still dead in case it comes back
   // to life through other means.
   while(delay--)
   {
      ACS_Delay(1);

      if(ACS_GetActorProperty(0, APROP_Health) > 0)
         return;
   }

   // Keep trying to raise, in case something is blocking it.
   while(ACS_GetActorProperty(0, APROP_Health) <= 0)
   {
      ACS_Thing_Raise(0);
      ACS_Delay(1);
   }
}

//
// DUH_ThingDeath
//
[[call("ScriptS"), extern("ACS")]]
void DUH_ThingDeath(int score)
{
   DUH_ArtifactReadyTID(ACS_GetActorProperty(0, APROP_TargetTID), sqrtf(score));
}

// EOF

