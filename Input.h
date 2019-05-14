#pragma once
#ifndef INPUT_H
#define INPUT_H

#include "Defs.h"
#include "Creature.h"

// Convert to extern function
// Thread input so that input is reactive

void			grab_input();	// Scans the state of the keyboard
void			null_input_bit(short);

unsigned short	keyboard_input_new, keyboard_input_old;

SDL_Event	e;
Uint8		*keystate;

#endif
