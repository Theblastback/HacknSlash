#include "Creature.h"
#include <vector>

extern vector< list<Node *> > layer;


void generate_anim_table() {
	arr = (unsigned char **)malloc(sizeof(unsigned char *) * 9);
	for (char i = 0; i < 9; i++)
		arr[i] = (unsigned char *)malloc(sizeof(char) * 3);

	unsigned char pixel = 0;
	for (int outer = 0; outer <= 8; outer++) {
		for (char inner = 0; inner < 3; inner++) {
			arr[outer][inner] = pixel;
			if ((outer != 8) && (outer != 3) && (outer != 5) && (outer != 7)) // Values for jump will not change
				pixel++;
		}
	}
}


/* Table will look like

0)	0 , 1 , 2	IDLE
1)	3 , 4 , 5	ATTACK
2)	6 , 7 , 8	PROTECT (Parry)
3)	9 , 9 , 9	M_LEFT
4)	10, 11,12	FIRE
5)	13, 13,13	Quick Step
6)	14, 15,16	MISC ACTIONS
7)	17, 17,17	M_RIGHT
8)	18, 18,18	JUMP

*/
void free_anim_table() {
	for (char i = 0; i < 9; i++)
		free(arr[i]);

	free(arr);
}


// Constructor
// Need to decide what the config files will look like, such that I can modify this constructor appropiately
Creature::Creature(SDL_Renderer *render, string file, string leftward, int x_coor, int y_coor) {
	// File is that of the base creature. Contains Filename(name), true/false(leftward), x coor, y coor
	string path = "./_Sources/_Sprite_Data/" + file;
	fstream sprite_stats(path, fstream::in);

	if (sprite_stats.good()) {
		// Grab info from the sprite stat file
		sprite_stats >> this->Width >> this->Height >> this->health >> this->atk[0] >> this->atk[2] >> this->attack_up;
		
		this->Source_Rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
		this->Dest_Rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));

		if (this->Source_Rect == NULL)
			cout << "Source_Rect failed" << endl;
		if (this->Dest_Rect == NULL)
			cout << "Dest_Rect failed" << endl;

		// Setting the rectangular coordinates for the source image
		this->Source_Rect->x = 0;
		this->Source_Rect->y = 0;
		this->Source_Rect->w = this->Width;
		this->Source_Rect->h = this->Height;

		// Setting the rectangular coordinates for the destination image
		this->Dest_Rect->x = x_coor;
		this->Dest_Rect->y = y_coor;
		this->Dest_Rect->w = this->Width;
		this->Dest_Rect->h = this->Height;

		// Load image to node
		path = "./_Sources/_Sprite_Image/" + file;
		this->Image = IMG_LoadTexture(render, path.c_str());
		if (this->Image == NULL)
			cout << "Error: Failed to load texture" << endl;

		this->There = nullptr;

		this->Position = 0;
		if (!leftward.compare("FALSE"))
			this->is_left = false;
		else
			this->is_left = true;

		this->Adjust_X = 0;
		this->Adjust_Y = 0;

		this->momentum_x = 0;
		this->momentum_y = 0;
		this->grounded = false;

		this->is_attacking = 0;
		this->attack_stage = 0;

		sprite_stats.close();
	}
}

void Creature::change_momentum() {

	// Past momentum - (leftward momentum) + (rightward momentum) - cap starting at 15
	// Leftward and rightward cannot incur at the same time
	

	this -> momentum_x = (this -> momentum_x + (-1* ((this->action_status &  M_LEFT) >> 1)) + ((this->action_status & M_RIGHT) >> 2)) - ((this -> momentum_x / 10) * (abs(this -> momentum_x) - 10));

	// Decay rate when no buttons pressed
	this -> momentum_x = this -> momentum_x / 2;

	// Past momentum + (upward momentum) - (downward momentum)
	this -> momentum_y = this -> momentum_y + 2 - ((((this->action_status & JUMP) * this -> grounded) / JUMP) * 20) ;

	// Automatically resets y momentum if on the ground
	this -> momentum_y = this -> momentum_y * this -> grounded;
}

void Creature::shift_momentum(char x, char y) {
	this -> momentum_x = this -> momentum_x + x;
	this -> momentum_y = this -> momentum_y + y;
}

void Creature::change_position() {
	unsigned short check = 0;
	unsigned char left_check, right_check, up_check, down_check;

	left_check = ((this->action_status & M_LEFT) / M_LEFT);
	right_check = ((this->action_status & M_RIGHT) / M_RIGHT);
	up_check = ((this->action_status & L_UP) / L_UP);
	down_check = ((this->action_status & L_DOWN) / L_DOWN);

	this->is_left = (!(this->action_status & M_RIGHT)) * ((this -> action_status & M_LEFT) | this->is_left);

	// Figure out how to determine if protect key press is a parry or quickstep. (side parry needs M_left or M_right pressed)
	for (unsigned short select = ATTACK; select <= JUMP; select = select << 1) {
		if ((check = this->action_status & select))
			break;
	}

	// Input is not an ATTACK, protect/quickstep, shot, or jump
	if (!check) {
		// If map is not being opened, then assume creature is moving
		if (!(check = (this->action_status & MAP)) )
			check = ((this->action_status & M_LEFT) | (this->action_status & M_RIGHT)) - 1;
	} else if (check == PROTECT) {
		// Determine if this is a quickstep or a parry
		check = check + (3 * ((!left_check) | (!right_check) | (!up_check)));
	}

	this->Position.curr_anim = arr[check][(up_check | down_check)];
}

void Creature::control_attack() {
	this->queued_attack = this->queued_attack || (this->action_status & ATTACK);

	if (this->is_attacking || this->queued_attack) {
		// Null movement bits. This is called before change to momentum call
		this->action_status = (this->action_status | M_LEFT | M_RIGHT) ^ (M_LEFT | M_RIGHT);
		this->attack(layer[2]);
	}
}


// Getters ###########################

char Creature::get_moment_x() {
	return(this -> momentum_x);
}
char Creature::get_moment_y() {
	return(this -> momentum_y);
}
unsigned char Creature::get_health() {
	return(this -> health);
}
unsigned short Creature::get_action() {
	return(this -> action_status);
}
char Creature::get_grounded() {
	return(this -> grounded);
}
bool Creature::get_facing_left() {
	return(this -> is_left);
}

// Setters #########################

void Creature::set_moment_x(char x) {
	this -> momentum_x = x;
}
void Creature::set_moment_y(char y) {
	this -> momentum_y = y;
}
void Creature::adjust_health(short shift) {
	if ( this -> health + shift <= 0 )
		this -> health = 0;
	else if ( this -> health + shift > 255 )
		this -> health = 255;
	else
		this -> health = health + shift;
}
void Creature::set_action(unsigned short input) {
	this -> action_status = input;
	this->change_position();
}
void Creature::set_grounded(char state) {
	this -> grounded = state;
}
void Creature::set_facing_left(bool value) {
	this -> is_left = value;
}

// Attack Functions #########################

// General purpose function that determines all of the attacks

void Creature::attack(list<Node *> lay_3) {
	bool beyond_start, before_end;

	// When there is a queued attack and is available to start, take current action status rather than past
	if (!this->is_attacking && this->queued_attack) {
		this->attack_type = this->action_status & (L_UP | L_DOWN);
		this->attack_stage = 0;
		this->queued_attack = 0;
	}
	// Animations are handled in the change_position() function

	// Handle when to deal damage (Single hit during active frames
	beyond_start = this->attack_stage > this->atk[this->attack_type].start;
	before_end = this->attack_stage < this->atk[this->attack_type].end;

	if (beyond_start && before_end) {

	}

	this->attack_stage = (this->attack_stage + 1) % (this->atk[this->attack_type].frames + 1);
	// This will end the attack once all of the frames have been played
	this->is_attacking = this->attack_stage;
}

