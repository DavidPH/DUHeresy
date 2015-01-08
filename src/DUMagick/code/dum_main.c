//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Top-level menu driver and input handler.
//
//-----------------------------------------------------------------------------

#include "dum_spel.h"
#include "dum_stan.h"

#include <stddef.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// MenuState
//
typedef enum MenuState
{
   MS_NONE,
   MS_HUD,
   MS_SPELL,
   MS_STANCE,
} MenuState;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DUM_MainMenu
//
[[call("ScriptS"), extern("ACS"), script("Enter")]]
void DUM_MainMenu(void)
{
   int buttons, oldbuttons, newbuttons = 0;
   int pnum = ACS_PlayerNumber();
   MenuState ms = MS_HUD;

   if(pnum < 0) return;

   while(true)
   {
      oldbuttons = newbuttons;
      newbuttons = ACS_GetPlayerInput(pnum, INPUT_BUTTONS);
      buttons = newbuttons & ~oldbuttons;

      switch(ms)
      {
      case MS_NONE:
         break;

      case MS_HUD:
         if(buttons & BT_USER1)
         {
            ACS_SetPlayerProperty(0, true, PROP_TOTALLYFROZEN);

            switch(ACS_PlayerClass(pnum))
            {
            case CLASS_FIGHTER:
               ms = MS_STANCE;
               break;

            case CLASS_CLERIC:
            case CLASS_MAGE:
               if(!DUM_SpellSelect[pnum])
                  DUM_SpellSelect[pnum] = &DUM_SpellBasic;
               ms = MS_SPELL;
               break;
            }
         }
         else
         {
            ACS_SetPlayerProperty(0, false, PROP_TOTALLYFROZEN);

            // Cast spell.
            if(buttons & BT_USER2)
            {
               SpellData **spell = &DUM_SpellSelect[pnum];

               if(*spell)
               {
                  // Clear spell text, if any.
                  ACS_BeginPrint();
                  ACS_EndPrint();

                  if((*spell)->funcCast)
                     (*spell)->funcCast(*spell);

                  *spell = NULL;
               }
               else
               {
                  ACS_BeginPrint();
                  ACS_PrintString(s"No spell selected.");
                  ACS_EndPrint();
               }
            }

            // TODO: Draw HUD.
         }
         break;

      case MS_SPELL:
         if(buttons & BT_USER1)
         {
            // Clear menu.
            ACS_BeginPrint();
            ACS_EndPrint();

            ms = MS_HUD;
         }
         else
            DUM_SpellMenu(buttons);
         break;

      case MS_STANCE:
         if(buttons & BT_USER1)
         {
            // Clear menu.
            ACS_BeginPrint();
            ACS_EndPrint();

            ms = MS_HUD;
         }
         else
            DUM_StanceMenu(buttons);
         break;
      }

      ACS_Delay(1);
   }
}

// EOF

