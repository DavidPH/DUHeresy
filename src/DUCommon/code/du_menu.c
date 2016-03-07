//-----------------------------------------------------------------------------
//
// Copyright (C) 2012-2015 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Generalized menu system.
//
//-----------------------------------------------------------------------------

#include "du_menu.h"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define BASE_MSGID 100


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DU_MenuHudClear
//
static void DU_MenuHudClear(Menu *, int msgid)
{
   ACS_BeginPrint();
   ACS_MoreHudMessage();
   ACS_OptHudMessage(HUDMSG_PLAIN, msgid, 0, 0,0, 1);
   ACS_EndHudMessage();
}

//
// DU_MenuHudImage
//
static void DU_MenuHudImage(Menu *, int msgid, __str image, accum x, accum y)
{
   ACS_SetFont(image);

   ACS_BeginPrint();
   ACS_PrintString(s"A");
   ACS_MoreHudMessage();
   ACS_OptHudMessage(HUDMSG_PLAIN, msgid, 0, x,y, 0);
   ACS_EndHudMessage();
}

//
// DU_MenuHudText
//
static void DU_MenuHudText(Menu *menu, int msgid, __str text, int color,
   accum x, accum y)
{
   ACS_SetFont(menu->font);

   ACS_BeginPrint();
   ACS_PrintString(text);
   ACS_MoreHudMessage();
   ACS_OptHudMessage(HUDMSG_PLAIN, msgid, color, x,y, 0);
   ACS_EndHudMessage();
}


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

//
// DU_MenuBegin
//
void DU_MenuBegin(Menu *menu)
{
   ACS_SetHudSize(menu->w, menu->h, 1);

   menu->x = menu->w / 2;
   menu->y = menu->h / 2;
}

//
// DU_MenuEnd
//
void DU_MenuEnd(Menu *menu)
{
   int msgid = BASE_MSGID;

   // Cursor.
   DU_MenuHudClear(menu, msgid++);

   // Buttons.
   for(MenuButton *button = menu->buttons; button; button = button->next)
   {
      // button->img
      DU_MenuHudClear(menu, msgid++);

      // button->txt
      DU_MenuHudClear(menu, msgid++);
   }

   // Labels.
   for(MenuLabel *label = menu->labels; label; label = label->next)
   {
      // label->img
      DU_MenuHudClear(menu, msgid++);

      // label->txt
      DU_MenuHudClear(menu, msgid++);
   }

   // Texts.
   for(MenuText *text = menu->texts; text; text = text->next)
   {
      // text->txt
      DU_MenuHudClear(menu, msgid++);
   }
}

//
// DU_MenuRun
//
void DU_MenuRun(Menu *menu)
{
   int msgid = BASE_MSGID;

   // Read buttons.
   int buttons    = ACS_GetPlayerInput(-1, INPUT_BUTTONS);
   int buttonsOld = ACS_GetPlayerInput(-1, INPUT_OLDBUTTONS);
   int buttonsNew = buttons & ~buttonsOld;

   // Poll and cap cursor movement.
   accum inputX = ACS_GetPlayerInputFixed(-1, INPUT_YAW);
   accum inputY = ACS_GetPlayerInputFixed(-1, INPUT_PITCH);

   if(ACS_GetCVar(s"invertmouse"))
      inputY = -inputY;

   menu->x -= inputX * menu->w * 2;
   menu->y -= inputY * menu->h * 2;

   if(menu->x < 0)       menu->x = 0;
   if(menu->x > menu->w) menu->x = menu->w;

   if(menu->y < 0)       menu->y = 0;
   if(menu->y > menu->h) menu->y = menu->h;

   if(menu->run)
      menu->run(menu);

   // Draw cursor.
   DU_MenuHudImage(menu, msgid++, s"CURSOR", (int)menu->x+0.1k, (int)menu->y+0.1k);

   // Draw buttons.
   for(MenuButton *button = menu->buttons; button; button = button->next)
   {
      if(button->run)
         button->run(button);

      int x = button->x + button->w/2;
      int y = button->y + button->h/2;

      DU_MenuHudText(menu, msgid++, button->txt, button->color, x, y);

      if(button->img[0])
         DU_MenuHudImage(menu, msgid++, button->img, x, y);
      else
         DU_MenuHudClear(menu, msgid++);
   }

   // Draw labels.
   for(MenuLabel *label = menu->labels; label; label = label->next)
   {
      if(label->run)
         label->run(label);

      int x = label->x + label->w/2;
      int y = label->y + label->h/2;

      DU_MenuHudText(menu, msgid++, label->txt, label->color, x, y);

      if(label->img[0])
         DU_MenuHudImage(menu, msgid++, label->img, x, y);
      else
         DU_MenuHudClear(menu, msgid++);
   }

   // Draw texts.
   for(MenuText *text = menu->texts; text; text = text->next)
   {
      if(text->run)
         text->run(text);

      accum x = (int)text->x + 0.1;
      accum y = (int)text->y + 0.1;

      DU_MenuHudText(menu, msgid++, text->txt, text->color, x, y);
   }

   // Mouse-over events.
   for(MenuButton *button = menu->buttons; button; button = button->next)
   {
      if(button->hover &&
         menu->x >= button->x && menu->x < button->x + button->w &&
         menu->y >= button->y && menu->y < button->y + button->h)
      {
         button->hover(button);
      }
   }

   // Click events.
   if(buttonsNew & BT_ATTACK)
   {
      for(MenuButton *button = menu->buttons; button; button = button->next)
      {
         if(button->click &&
            menu->x >= button->x && menu->x < button->x + button->w &&
            menu->y >= button->y && menu->y < button->y + button->h)
         {
            button->click(button);
         }
      }
   }
}

// EOF

