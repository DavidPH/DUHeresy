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

#ifndef DU_MENU_DH__
#define DU_MENU_DH__

#include <ACS_ZDoom.h>
#include <stdfix.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct MenuButton MenuButton;
typedef struct MenuLabel  MenuLabel;
typedef struct MenuText   MenuText;
typedef struct Menu       Menu;

[[call("ScriptS")]]
typedef void (*MenuButtonFunc)(MenuButton *);

[[call("ScriptS")]]
typedef void (*MenuLabelFunc)(MenuLabel *);

[[call("ScriptS")]]
typedef void (*MenuTextFunc)(MenuText *);

[[call("ScriptS")]]
typedef void (*MenuFunc)(Menu *);

struct MenuButton
{
   // Rendering
   accum x, y;  // position
   accum w, h;  // size
   __str txt;   // text
   __str img;   // image
   int   color; // text color

   // Callbacks
   MenuButtonFunc click; // on-click
   MenuButtonFunc hover; // mouse-over
   MenuButtonFunc run;   // on-run, pre-draw

   // Misc Data
   MenuButton *next; // next button in menu
   void       *data; // user data
};

struct MenuLabel
{
   // Rendering
   accum x, y;  // position
   accum w, h;  // size
   __str txt;   // text
   __str img;   // image
   int   color; // text color

   // Callbacks
   MenuLabelFunc run; // on-run, pre-draw

   // Misc Data
   MenuLabel *next; // next label in menu
};

struct MenuText
{
   // Rendering
   accum x, y;  // position
   __str txt;   // text
   int   color; // text color

   // Callbacks
   MenuTextFunc run; // on-run, pre-draw

   // Misc Data
   MenuText *next; // next text in menu
};

struct Menu
{
   // Rendering
   accum w, h;
   __str font;

   // Callbacks
   MenuFunc run; // on-run, pre-draw

   MenuButton *buttons;
   MenuLabel  *labels;
   MenuText   *texts;

   // Runtime Data
   accum x, y; // cursor position
};


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

void DU_MenuBegin(Menu *menu);
void DU_MenuEnd(Menu *menu);
void DU_MenuRun(Menu *menu);

#endif//DU_MENU_DH__

