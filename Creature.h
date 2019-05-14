#pragma once
#pragma warning(disable:4996)

#ifndef CREATURE_H
#define CREATURE_H

#include "Defs.h"

// Table array that will point to correct animations
unsigned char **arr;

void generate_anim_table();
void free_anim_table();


class Creature : public Node {
	public:
		void change_momentum();			// Use input for creature
		void shift_momentum(char, char);	// Used for scripted movements (A.I., cutscenes, etc.)
		void attack(list<Node *>);		//(attacker, list of all enemies in level, total enemies in level)
		void control_attack();		// Prevents movement during attack, allows/queues attack

		// Initialization/Termination functions
		Creature(SDL_Renderer *, string, string, int, int); // File that holds all the information on the creature

		// Getters
		char			get_moment_x();
		char			get_moment_y();
		char			get_grounded();
		unsigned char	get_health();
		unsigned short	get_action();
		bool			get_facing_left();

		//Setters
		void	set_moment_x(char);
		void	set_moment_y(char);
		void	set_grounded(char);
		void	adjust_health(short);
		void	set_action(unsigned short);
		void	set_facing_left(bool);


	private:
		void	change_position(); // Check to see if animation has changed OR if the animation needs to be cycled (CHANGES FACING LEFT VALUE****)

		// Momentum is how much the player adjusts during the screen
		// Momentum can be negative, or positive. Walking/running takes time to build up and decay
		char	momentum_x;
		char	momentum_y;

		// Attack stats
		struct attack {
			unsigned int start : 5;
			unsigned int end : 5;
			unsigned int dam : 7;
			unsigned int frames : 5;
			unsigned int offset : 10;
			unsigned int : 0;
		};
		
		struct attack atk[3];
		/*
		atk[0] is side stats
		atk[1] is up stats
		atk[2] is down stats
		*/

		unsigned char	queued_attack : 1;	// Pre-registers attack to commence.
		unsigned char	is_attacking : 1;		// To determine whether to expand hitbox or not
		unsigned char	attack_stage : 5;		// Keeps track of the stage of attack
		unsigned char	attack_type : 2;		// Works off of L_UP and L_Down. 0 is side
		unsigned char	grounded : 1;
		unsigned char : 0;


		unsigned char	health;		// Health shall be displayed as a percentage
		unsigned short	action_status;	// Current status of creature (Input) 
		unsigned char	alignment;
		unsigned short	identifier;		// For use with cutscenes. Will either be NULL or something
		bool			struck;
};

#endif