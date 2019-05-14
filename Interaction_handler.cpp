#include "Handler.h"

Interaction * create_interaction(string) {} // Let this create an interaction. Worry about formating later

void sort_interaction_handler() { // Any time data has been loaded in, sort the interaction handler so that not all interactions are checked
	Interaction * temp;

	/// Not optimized well at all. Hundreds of loaded interactions may pose a problem
	for (short outer = 0; outer < interact_handler.size(); outer++) {
		for (short inner = outer + 1; inner < interact_handler.size(); inner++) {
			if (interact_handler[inner]->trigger_area.x > interact_handler[outer]->trigger_area.x) {
				temp = interact_handler[inner];
				interact_handler[inner] = interact_handler[outer];
				interact_handler[outer] = temp;
			}
		}
	}
}

void load_interact_handler(string) {
	// Need to sort out how files are going to be loaded in


}

void clear_interact_handler() {
	interact_handler.clear();
}