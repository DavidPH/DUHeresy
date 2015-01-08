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

#ifndef DUM_STAN_H__
#define DUM_STAN_H__

#include "du_defs.h"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// StanceType
//
typedef enum StanceType
{
   STANCE_STANDARD,
   STANCE_OFFENSIVE,
   STANCE_DEFENSIVE,

   STANCEMAX
} StanceType;


//----------------------------------------------------------------------------|
// Global Variables                                                           |
//

extern __str DUM_StanceName[STANCEMAX];

extern StanceType DUM_StanceSelect[MAX_PLAYERS];


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

extern void DUM_StanceMenu(int buttons);

#endif//DUM_STAN_H__

