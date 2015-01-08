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

#ifndef DUM_SPEL_H__
#define DUM_SPEL_H__

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct SpellData SpellData;

[[call("ScriptS")]]
typedef void (*SpellFunc)(SpellData const *);

//
// SpellData
//
struct SpellData
{
   __str name;
   SpellFunc funcCast;
   SpellFunc funcSelect;
   SpellFunc funcPassive;

   SpellData *up;
   SpellData *down;
   SpellData *left;
   SpellData *right;
};


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

extern SpellData *DUM_SpellSelect[MAX_PLAYERS];

extern SpellData DUM_SpellBasic;
extern SpellData DUM_SpellBurst;
extern SpellData DUM_SpellCircle;
extern SpellData DUM_SpellSpread;
extern SpellData DUM_SpellStream;


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

extern void DUM_SpellMenu(int buttons);

#endif//DUM_SPEL_H__

