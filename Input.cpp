#include "Input.h"
// Toggle attack input
void grab_input() {
	SDL_PollEvent(&e);
	unsigned short sec_byte = 0;


	keystate = (Uint8 *) SDL_GetKeyboardState(NULL);

	keyboard_input_new = 0;
	// Must nullify cases of both left and right being pressed & up and down being pressed

	// Check can only be true if and only if only one of the vertical keys are pressed
	unsigned char check = keystate[SDL_SCANCODE_W] ^ keystate[SDL_SCANCODE_D];

	keyboard_input_new = keyboard_input_new | (keystate[SDL_SCANCODE_W] * L_UP * check);
	keyboard_input_new = keyboard_input_new | (keystate[SDL_SCANCODE_D] * L_DOWN * check);

	// Case for left and right being pressed
	check = (keystate[SDL_SCANCODE_A] ^ keystate[SDL_SCANCODE_D]);

	keyboard_input_new = keyboard_input_new | (keystate[SDL_SCANCODE_A] * M_LEFT * check);
	keyboard_input_new = keyboard_input_new | (keystate[SDL_SCANCODE_D] * M_RIGHT * check);
	
	// Movement halting actions
	sec_byte = (keystate[SDL_SCANCODE_J] * ATTACK);
	sec_byte = (keystate[SDL_SCANCODE_K] * PROTECT);
	sec_byte = (keystate[SDL_SCANCODE_L] * FIRE);

	// Misc keys
	sec_byte = keyboard_input_new = keyboard_input_new | (keystate[SDL_SCANCODE_E] * INTERACT);

	sec_byte = (keystate[SDL_SCANCODE_P] * MENU);
	sec_byte = (keystate[SDL_SCANCODE_ESCAPE] * MENU);

	sec_byte = (keystate[SDL_SCANCODE_M] * MAP);
	sec_byte = (keystate[SDL_SCANCODE_SPACE] * JUMP);

	keyboard_input_new = keyboard_input_new | sec_byte;

	null_input_bit(sec_byte & keyboard_input_old);
}


void null_input_bit(short null_value) {
	keyboard_input_new = (keyboard_input_new | null_value) ^ null_value;
}