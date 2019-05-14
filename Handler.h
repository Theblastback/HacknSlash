#pragma once

#ifndef HANDLER_H
#define HANDLER_H

#include "Creature.h"
#include <vector>

// Interactions handler function
extern void			clear_interact_handler();
extern void			load_interact_handler(string);

vector<Interaction *> interact_handler;

class Interaction {
	public:
		SDL_Rect	*	trigger_area;
		char		*	dest_file;
};


#endif