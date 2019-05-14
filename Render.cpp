#include "Render.h"

///// Coversion & Checking functions ///////////////////////////////////////////////////////////////////

extern inline bool is_overlap_left(Node *, Node *);
extern inline bool is_overlap_right(Node *, Node *);
extern inline bool is_overlap_up(Node *, Node *);
extern inline bool is_overlap_down(Node *, Node *);


/// CHANGE LATER ONCE CONCEPT OF GRAVITY FINALIZED
void convert_momentum_to_adjust(Creature *player, Creature **list, short total) {
	short index;

	player->Adjust_X += player->get_moment_x();
	player->Adjust_Y += player->get_moment_y();

	//player -> set_grounded(is_grounded(player -> creature_rendered));

	for (index = 0; index < total; index++) {
		list[index]->Adjust_X += list[index]->get_moment_x();
		list[index]->Adjust_Y += list[index]->get_moment_y();

		//list[index] -> set_grounded(is_grounded(list[index] -> creature_rendered));
	}
}

void adjust_for_player_movement(Creature *player) { // x is adjust x for player, y is adjust y for player
	char index;
	list<Node *>::iterator element;
	list<Node *> lay_index;
	Node * tmp;



	for (index = 1; index < 5; index++) {
		lay_index = layer[index];

		if (!lay_index.empty()) {
			for ( element = lay_index.begin(); element != lay_index.end(); ++element ) {
				tmp = *element;
				if (tmp != player) {
					tmp->Adjust_X += (player->Adjust_X * (1 + (index / 2)) *-1);
					tmp->Adjust_Y += player->Adjust_Y * (1 + (index / 2)) *-1;
				}

			}
		}
	}

	if (player->Adjust_X > 0)
		player->Adjust_X = 1;
	else if (player->Adjust_X < 0)
		player->Adjust_X = -1;
}

bool is_grounded(Node *check) {
	list<Node *>::iterator index = (layer[2]).begin();
	Node * tmp;
	bool status = false;


	while ((index != layer[3].end()) && (status != true)) {
		tmp = *index;
		status = status || ( ( is_overlap_left(tmp, check) || is_overlap_right(tmp, check)) && is_overlap_down(tmp, check) );
		++index;
	}

	return(status);
}


////// Rendering functions //////////////////////////////////////////////////////////////////////////////

void render_screen() {
	Node *tmp;

	for (unsigned char out_index = 0; out_index <= 5; out_index++) {
		list<Node *>::iterator in_index = (layer[out_index]).begin();

		while (in_index != layer[out_index].end()) {
			tmp = *in_index;
			cycle_animation(tmp);

			// Let cycle_animation handle adjusting the offset for the destination rect
			SDL_RenderCopyEx(main_renderer, tmp->Image, tmp->Source_Rect, tmp->Dest_Rect, (const double)tmp->Angle, NULL, (const SDL_RendererFlip)(tmp->is_left));
			++in_index;
		}

	}

	SDL_RenderPresent(main_renderer);

}

// Should change the destination rect depending on how the animation adjusts the image.
void inline cycle_animation(Node *tmp) {

	// Other processes will change what the current, and only current, animation is
	tmp -> Source_Rect -> y = tmp->Source_Rect->w * tmp->Position.curr_anim;

	tmp->frame_count = tmp->frame_count % 10;

	// Reset frame if animation changes, else advance frame by one frame every 10 frames
	tmp->Position.frame = (tmp->Position.frame + !tmp->frame_count) * short(tmp->Position.curr_anim == tmp->Position.past_anim);

	tmp->Position.past_anim = tmp->Position.curr_anim;

	tmp->Source_Rect->x = (tmp->Position.frame * tmp->Source_Rect->w);


	/********* Incomplete **********\
	// Look up		=	y = h * 1
	// Look down	=	y = h * 2
	// Left/right	=	y = h * 3
	// Attack		=	y = h * 4
	// Protect		=	y = h * 5
	// Interact		=	y = h * 6
	// Jump		=	y = h * 7
	\*******************************/

	

	// Cycle where the image is to go
	tmp -> Dest_Rect -> x += tmp -> Adjust_X;
	tmp -> Dest_Rect -> y += tmp -> Adjust_Y;

	// Reset what the values of adjust are
	tmp -> Adjust_X = 0;
	tmp -> Adjust_Y = 0;
}



///// Initializations //////////////////////////////////////////////////////////////////////////////////

void init_SDL() {										// Start up SDL and initialize the fonts
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { // Get all of the goodies of sdl started up
		fprintf(stderr, "Couldn't init SDL: %s\n", SDL_GetError());
		exit(1);
	}
	// Start of PNG expansion
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		fprintf(stderr, "Couldn't init png support: %s \n", IMG_GetError());
		exit(2);
	}

	SDL_ShowCursor(SDL_DISABLE);
}

void init_render() {
	char index;

	// layer[0]: furthest background; Movement = base * 0;
	// layer[1]: Another background; Moves with player; Movement = base * 0.5;
	// layer[2]: All enemies, with weapons; Moves with player; Movement = base;
	// layer[3]: All terrain player steps on; Moves with player; Movement = base;
	// layer[4]: Foreground; Moves faster than player; Movement = base * 2;
	// layer[5]: HUD; displays under screem; Movement = 0; *Special;

	for (index = 0; index < 6; index++) {
		layer[index] = list<Node *>();
	}

	if (SDL_CreateWindowAndRenderer(TRUE_WIDTH, TRUE_HEIGHT, SDL_WINDOW_RESIZABLE, &main_window, &main_renderer)) {
		fprintf(stderr, "Couldn't get window to pop up: %s\n", SDL_GetError());
		exit(3);
	}
}


///// Freeing Statements /////////////////////////////////////////////////////////////////////////////

void free_SDL() {
	// Exit SDL_Image
	IMG_Quit();
	// Clean up and exit SDL2
	SDL_Quit();
}

void free_render() {
	char cycle;
	for (cycle = 0; cycle <= 5; cycle++)
		layer[cycle].clear();


	
	SDL_DestroyRenderer(main_renderer);
	SDL_DestroyWindow(main_window);
}