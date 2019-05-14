#pragma once
#ifndef DEFS_H
#define DEFS_H

// Screen resolutions (Temporary, must fix later)
#define TRUE_WIDTH	1080
#define TRUE_HEIGHT	720

#define VIEW_WIDTH	1080
#define VIEW_HEIGHT	550

// Sprite defines

// Make movements mathematically prevailent. Ignore checking with if's, use math to determine movements

// Looking based movements; Attack change
#define L_UP		0x001	// Make player look up
#define L_DOWN		0x002	// Make player look down

// Moving based movements
#define M_LEFT		0x004
#define M_RIGHT		0x008

// Key pressing based defines
#define ATTACK		0x010	// Button press for ONLY the melee weapon
#define PROTECT		0x020	// Protect is the key press for quickstep/parry, since that key is used for both parrying and backstepping
#define FIRE		0x040	// Button press for ONLY the firearms
#define JUMP		0x080	// Make player jump
#define MENU		0x100	// Open/close options menu
#define MAP			0x200	// Open/close map
#define INTERACT		0x400	// Button press for interacting with environments & menus

// 3rd party Library includes
#include "sdl.h"
#include "SDL_image.h"
#include "sdl_mixer.h"
#include "SDL_ttf.h"

// C library includes
#include <direct.h>
#include <stdbool.h>

// C++ library includes
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <list>

using namespace std;

// List manipulation
template <typename T>
extern T get_element(list<T>, unsigned int);

// Level loading
extern vector<SDL_Texture *> Z_load_xcf(); // Convert all layers to individual textures

// Collision detection
extern inline bool is_overlap_left(Node *, Node *);	// object, object being checked for overlap
extern inline bool is_overlap_right(Node *, Node *);	// object, object being checked for overlap
extern inline bool is_overlap_up(Node *, Node *);	// object, object being checked for overlap
extern inline bool is_overlap_down(Node *, Node *);	// object, object being checked for overlap
	 
class Node {
	public:
		SDL_Texture	*Image;
		SDL_Rect	*Source_Rect;	// Dimensions for what to copy
		SDL_Rect	*Dest_Rect;		// Dimensions for where to paste
		short int	Adjust_X;		// Positive/negative value to adjust where to place the image next along X axis
		short int	Adjust_Y;		// Positive/negative value to adjust where to place the image next along Y axis
		short int	Width;		// To keep player centered when attacking
		short int	Height;
		bool		is_left;		// Will be used to determine whether to flip the sprite or not
		short	int	Angle;		// Used for rotation


		struct position {
			unsigned char	curr_anim : 6;
			unsigned char	apply_offset : 1;
			unsigned char	vertical : 1;	// Determine if x or y offset needs to be adjusted
			unsigned char : 0;
			unsigned short	frame : 6;		// What frame of the cyce we are in
			unsigned short	offset : 10;	// How much of an offset needs to be applied
			unsigned short : 0;
			unsigned char	past_anim;
		} Position;
		unsigned char	frame_count;	// Determines when to change the animation of the creature


		// Each node is 47 bytes large *Too much?*

		// Background Node manipulation function
		Node();
		Node(string , short, short, int, int, short, short, short, SDL_Renderer *);	// PNG Path, Source X, Source Y, Dest X, Dest Y, Width, Height, Rotation, Main renderer
		void rmNode(list<Node *>);	// Remove this node from the list (list is a queue in this case)
		void free_node();			// Will free any node
};

#endif