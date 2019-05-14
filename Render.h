#pragma once
#ifndef RENDER_H
#define RENDER_H

#include "Defs.h"
#include "Creature.h"
#include <math.h>
#include <vector>

// Definitely convert class to extern functions



// CONVERSION FUNCTIONS
extern void adjust_for_player_movement(Creature *); // USE ONLY WHEN PLAYER IS MOVING!! (player -> adjust_x, player -> adjust_y). Shifts ALL enemies in level.
extern void convert_momentum_to_adjust(Creature *, Creature **, short); // (Player, list of creatures in level, total number of enemies in level); Does not change animation


// RENDERING FUNCTIONS
extern void render_screen();	// Will render the screen
extern void fade_out_screen();	// Fade screen to black and free all queues
extern void fade_in_screen();	// Slowly loads in everything (to prevent frame spikes), then fade in the screen

				
// INITIALIZATION FUNCTIONS
extern void init_SDL();		// Initialize all parts of sdl we will use
extern void init_render();	// Creates the window and renderer


// FREEING FUNCTIONS
extern void free_SDL();		// Exit out of SDL
extern void free_render();	// Frees the window and the renderer


// Rendering cruds
extern inline void cycle_animation(Node *); // Used only to cycle animations. Changes x, y of nodes's source image. resets adjusts
extern bool is_grounded(Node *);	// 0 for grounded, 1 for not. Used in convert_momentum_to_adjust()

vector< list<Node *> > layer;	
list<Node *> fader;

SDL_Renderer	*main_renderer;
SDL_Window		*main_window;
	

#endif