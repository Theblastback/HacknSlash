#include "Defs.h"
#pragma warning(disable:4996)

template <typename T>
T get_element(list<T> data, unsigned int index) {
	auto element = data.begin();

	std::advance(element, (index - 1));

	return (*element);
}


// NODE RELATED FUNCTIONS /////////////////////////////////////////////////////////////////////////////
Node::Node() {}

Node::Node(string path, short S_X, short S_Y, int D_X, int D_Y, short Width, short Height, short rotation, SDL_Renderer * renderer) {
	cout << path << endl;

	this->Source_Rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
	this->Dest_Rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));

	if (this->Source_Rect == NULL)
		cout << "Source_Rect failed" << endl;
	if (this->Dest_Rect == NULL)
		cout << "Dest_Rect failed" << endl;

	// Setting the rectangular coordinates for the source image
	this -> Source_Rect -> x = S_X;
	this -> Source_Rect -> y = S_Y;
	this -> Source_Rect -> w = Width;
	this -> Source_Rect -> h = Height;

	// Setting the rectangular coordinates for the destination image
	this -> Dest_Rect -> x = D_X;
	this -> Dest_Rect -> y = D_Y;
	this -> Dest_Rect -> w = Width;
	this -> Dest_Rect -> h = Height;

	// Load image to node
	this -> Image = IMG_LoadTexture(renderer, path.c_str());
	if (this->Image == NULL)
		cout << "Error: Failed to load texture" << endl;

	cout << "______________________________________________" << endl;

	this->Angle = rotation;
	this->Position = 0;
	this->is_left = false;

	this->Adjust_X = 0;
	this->Adjust_Y = 0;

	this->Width = this->Source_Rect->x;
	this->Height = this->Source_Rect->h;

	cout << "Created node" << endl;
}

void Node::rmNode(list<Node *> list) {
	if ( !list.empty() ) {
		list.remove(this);
		
		this -> free_node();
	}
}

void Node::free_node() {	// Free either type of node in program
	printf("start\n");
	if (this != nullptr) {
		if (this->Image != NULL)
			SDL_DestroyTexture(this->Image);

		free(this->Source_Rect);
		free(this->Dest_Rect);

		cout << "N_attempt free of node" << endl;
		delete this;
	}
}


// Universal functions /////////////////////////////////////////////////////////////////////////////////

bool is_overlap_left(Node *block, Node *checked) {
	if (	(block -> Dest_Rect -> x + block -> Dest_Rect -> w) >= checked -> Dest_Rect -> x &&
		 checked -> Dest_Rect -> y <= (block -> Dest_Rect -> y + block -> Dest_Rect -> h) && 
		(checked -> Dest_Rect -> y + checked -> Dest_Rect -> h) >= block -> Dest_Rect -> y &&
		 checked -> Dest_Rect -> x > block -> Dest_Rect -> x )	
		return(true);
	else
		return(false);
}

bool is_overlap_right(Node *block, Node *checked) {
	if ( ((checked -> Dest_Rect -> x + checked -> Dest_Rect -> w ) >= block -> Dest_Rect -> x) &&
	      (checked -> Dest_Rect -> x < block -> Dest_Rect -> x ) &&
	      (checked -> Dest_Rect -> y <= (block -> Dest_Rect -> y + block -> Dest_Rect -> h)) &&
	      (checked -> Dest_Rect -> y + checked -> Dest_Rect -> h) >= block -> Dest_Rect -> y )	
		return(true);
	else
		return(false);
}

bool is_overlap_up(Node *block, Node *checked) {
	if (	(block -> Dest_Rect -> y + block -> Dest_Rect -> h ) >= checked -> Dest_Rect -> y &&
		 checked -> Dest_Rect -> y < block -> Dest_Rect -> y &&
		(checked -> Dest_Rect -> x + checked -> Dest_Rect -> w) >= block -> Dest_Rect -> x &&
		 checked -> Dest_Rect -> x <= (block -> Dest_Rect -> x + block -> Dest_Rect -> w))
		return(true);
	else
		return(false);
}

bool is_overlap_down(Node *block, Node *checked) {
	if ( (checked -> Dest_Rect -> y + checked -> Dest_Rect -> h) >= block -> Dest_Rect -> y &&
	      checked -> Dest_Rect -> y > block -> Dest_Rect -> y &&
	     (checked -> Dest_Rect -> x + checked -> Dest_Rect -> w) >= block -> Dest_Rect -> x &&
	      checked -> Dest_Rect -> x <= (block -> Dest_Rect -> x + block -> Dest_Rect -> w))
		return(true);
	else
		return(false);
}
