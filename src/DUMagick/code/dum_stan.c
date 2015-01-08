//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Stance functions and data.
//
//-----------------------------------------------------------------------------

#include "dum_stan.h"

#include <stdio.h>


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

__str DUM_StanceName[STANCEMAX] =
{
   s"STANDARD", s"OFFENSIVE", s"DEFENSIVE"
};

StanceType DUM_StanceSelect[MAX_PLAYERS];


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUM_StanceMenu
//
void DUM_StanceMenu(int buttons)
{
   StanceType *stance = &DUM_StanceSelect[ACS_PlayerNumber()];

   if(buttons & BT_FORWARD)
      *stance = STANCE_OFFENSIVE;

   if(buttons & BT_BACK)
      *stance = STANCE_DEFENSIVE;

   if(buttons & (BT_LEFT|BT_MOVELEFT))
      *stance = STANCE_STANDARD;

   if(buttons & (BT_RIGHT|BT_MOVERIGHT))
      *stance = STANCE_STANDARD;

   ACS_BeginPrint();
   __nprintf("OFFENSIVE\n|\nSTANDARD -- %S -- STANDARD\n|\nDEFENSIVE",
      DUM_StanceName[*stance]);
   ACS_EndPrint();
}

// EOF

