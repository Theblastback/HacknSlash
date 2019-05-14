// Class file includes
#include "Defs.h"
#include "Input.h"
#include "Render.h"
#include "Creature.h"

Renderer *	init_screen();
void		play_intro(Renderer *);
void		play_ending(Renderer *);

bool		fade_out(Renderer *); //Boolean output will pipe into disable input; false for disable, true for enable
bool		fade_in(Renderer *);

void		enable_input(Input *, bool);
void		transition_0(Renderer *);
void		transition_1(Creature ***, Creature **, Renderer *);
void		transition_2(Creature ***, Creature **, Renderer *);
void		transition_3(Creature ***, Creature **, Renderer *);





int main(int argc, char **argv) {
	// Initialize EVERYTHING		THEME: ~~Family~~
	// Expiration date: Sunday 17:00. Be at donovin at that time

	unsigned short k = sizeof(Creature);


	Defs define;
	char index = 0;
	char inlist = 3;
	char cycler;
	unsigned int last_time, delay;
	char well_get = 1;


	Renderer *render = init_screen();
	Input input;
	char select_trans = 0;


	Creature **player = (Creature **) malloc(sizeof(Creature*));
	*player = (Creature *)malloc(sizeof(Creature));
	player[0] = new Creature(render->get_renderer(), "./character_info/self_s1.txt");
	define.enqueue(render->layer[2], player[0]->creature_rendered);
	
	// list of all of the files that contain character info
	char *init_list[] = {
		"/character_info/mom_s1.txt",
		"/character_info/dad_s1.txt",
		"/character_info/twin_s1.txt"
	};

		// Create cast of characters

	Creature ***sprite_list = (Creature ***)malloc(sizeof(Creature **));
	*sprite_list = (Creature **)malloc(sizeof(Creature *) * 3);
	sprite_list[0][0] = new Creature(render->get_renderer(), init_list[0]);
	sprite_list[0][1] = new Creature(render->get_renderer(), init_list[1]);
	sprite_list[0][2] = new Creature(render->get_renderer(), init_list[2]);
	


	for (index = 0; index < 3; index++) {
		define.enqueue(render->layer[2], (sprite_list[0][index])->creature_rendered);
	}//Enqueues all the arrays


	// Start of main loop////////////////////////////////////////////////////////////////////
	while (select_trans != 4) {
		last_time = SDL_GetTicks();

		input.grab_input(); // Input from player aquired

		// Making sure player doesn't fall off edge
		if (player[0]->creature_rendered->Dest_Rect->x <= 5)
			input.null_input(M_LEFT);
		if (player[0]->creature_rendered->Dest_Rect->x > (TRUE_WIDTH / 2) - 100)
			input.null_input(M_RIGHT);


		player[0] -> set_action_position(input.get_input());

		// Check for interaction
		// +1 when interacting with well

		if (index / 10) {
			player[0]->set_action_position(input.get_input());
			for (cycler = 0; cycler < inlist; cycler++)
				(sprite_list[0][cycler])->set_action_position(0);
		} // Cycling through the characters animations
		index = (index + 1) %11;
		

		if (player[0]->creature_rendered->Dest_Rect->x >= (TRUE_WIDTH / 2) - 100) { // At the well, interacting
			select_trans = select_trans | well_get;
		}


		render->convert_momentum_to_adjust(player[0], *sprite_list, inlist); // HERE
		render->adjust_for_player_movement(player[0]);


		if (player[0]->creature_rendered->Dest_Rect->x <= 10) { // Back at the house
			if (inlist) {
				switch (select_trans) {
				case 1: // Mom has died
					transition_1(sprite_list, player, render);

					well_get = well_get << 1;
					inlist--;
					break;

				case 3: // Dad has died
					transition_2(sprite_list, player, render);
					well_get = well_get << 1;
					inlist--;
					break;

				case 7: // Twin has died
					transition_3(sprite_list, player, render);
					well_get = well_get << 1;
					inlist--;
					break;
				} // Transition to next stage
			}
			else if (select_trans == 15) {
				// Play ending

			}
		}


		render -> render_screen();
		if (input.get_input() & MENU)
			break;

		delay = (SDL_GetTicks() - last_time);
		if ( delay < 33 )
			SDL_Delay(33 - delay);
	}
	// End of main loop//////////////////////////////////////////////////////////
	
	render -> free_render();
	return 0;
}















// INIT_SCREEN /////
Renderer * init_screen() {
	Renderer *renderer = new Renderer;
	Defs *define = new Defs;
	Defs::node *tmp;

	define->init_SDL();
	renderer->init_render();


	define -> main_renderer = renderer->get_renderer();
	define->main_window = renderer->get_window();
	// Background

	// Creating background
	tmp = define -> mkNode("/Game_Jam_sprites/background_layer_0.png", 0, 0, 0, 0, TRUE_WIDTH, TRUE_HEIGHT);
	define -> enqueue(renderer->layer[0], tmp);

	// Creating moving background
	tmp = define->mkNode("/Game_Jam_sprites/house_layer_1.png", 0, 270, 0, -105, 935, 655);
	define->enqueue(renderer->layer[1], tmp);

	tmp = define -> mkNode("/Game_Jam_sprites/live_trees_layer_1.png", 0, 0, 0, 0, 1980, TRUE_HEIGHT); // house d_ = 100, w = 935, h = 655
	define -> enqueue(renderer->layer[1], tmp);



	// Creating terrain
	tmp = define -> mkNode("/Game_Jam_sprites/Terrain_layer_3.png", 0, 0, 0, 0, 1980, TRUE_HEIGHT);
	define -> enqueue(renderer->layer[3], tmp);

	tmp = NULL;
	// Creating foreground
//	tmp = define -> mkNode("", 0, 0, 0, 0, TRUE_WIDTH, TRUE_HEIGHT);
//	define -> enqueue(renderer->layer[4], tmp);

	delete define;
	return(renderer);
}


// TRANSITION 1
void transition_1(Creature ***list, Creature **player, Renderer *render) {
	Defs define;
	char index = 0;

	for (index = 0; index < 3; index++) {
		list[0][index] -> free_creature(render -> layer[2]);
		printf("____________________________\n");
		fflush(stdout);
	}

	player[0] -> free_creature(render -> layer[2]);
	

	//free(*list);
	//*list = (Creature **)malloc(sizeof(Creature) * 2);
	if (list[0][1] == NULL)
		printf("list0 null\n");

	if (list[0][1] == nullptr)
		printf("list0 nullptr\n");

	if (list[0][1] == NULL)
		printf("list1 null\n");

	if (list[0][1] == nullptr)
		printf("list1 nullptr\n");

	printf("\n\nBefore adding new creatures to array\n");
	list[0][0] = new Creature(render -> get_renderer(), "./character_info/dad_s2.txt");
	list[0][1] = new Creature(render -> get_renderer(), "./character_info/twin_s2.txt");

	if (player[0] == NULL)
		printf("player is null\n");

	if (player[0] == nullptr)
		printf("player is nullptr");


	player[0] = new Creature(render -> get_renderer(), "./character_info/self_s2.txt");
	define.enqueue(render->layer[2], (*player)->creature_rendered);

	define.enqueue(render->layer[2], (*list)[0] -> creature_rendered);
	define.enqueue(render->layer[2], (*list)[1] -> creature_rendered);
	
}


// TRANSITION 2
void transition_2(Creature ***list, Creature **player, Renderer *render) {
	Defs define;
	char index = 0;

	for (index = 0; index < 2; index++) {
		(*list[index])->free_creature(render->layer[2]);
		delete list[index];
	}

	(*player)->free_creature(render->layer[2]);
	delete (*player);

	char *init_list = {"./character_info/twin_s3.txt"};

	// Faster to just manually do it
	*list[0] = new Creature(render -> get_renderer(), init_list);

	(*player) = new Creature(render -> get_renderer(), "./character_info/self_s3.txt");

	define.enqueue(render->layer[2], (*list[0])->creature_rendered);

	define.enqueue(render->layer[2], (*player)->creature_rendered);

}

// TRANSITION 3
void transition_3(Creature ***list, Creature **player, Renderer *render) {
	Defs define;

	(*list[0])->free_creature(render->layer[2]);
	delete list[0];

	(*player)->free_creature(render->layer[2]);
	delete (*player);


	// Faster to just manually do it

	(*player) = new Creature(render -> get_renderer(), "./character_info/self_s4.txt");

	define.enqueue(render->layer[2], (*player)->creature_rendered);
}




/*
TODO:
DELIVER INPUT TO CREATURES (main)

-> Make HUD functions + interactions (SDL_TTF) + make input &/| render inherit it(?)
-> Make a database for this that will grab files(Must be set up as a tree; Illustrated in game_engine_design.txt)


MAKE EVERYTHING FLOW TOGETHER COHERRENTLY

FIX:




ENEMIES IN FILES SHOULD BE FORMATED AS SUCH:
- Location for the sprite image
- Short int for x coordinate of source sprite
- Short int for y coordinate of source sprite
- Short int for x coordinate of destination in render
- Short int for y coordinate of destination in render
- Short int for width of whole image
- Short int for height of whole image
- Location for weapon sprite image

SPRITES MUST ALWAYS BE FACING THE RIGHT
png files preferred


Far future plans:
	- Parrying, repostes and executions
	- Dodging

*/