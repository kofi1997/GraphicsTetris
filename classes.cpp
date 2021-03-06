/******************************************
*
* Official Name:  Kofi Dinizulu
*
* Nickname:  Kofi
*
* E-mail:  kdinizul@syr.edu
*
* Assignment:  Final Project
*
* Environment/Compiler:  Visual Studio 2015
*
* Date:  April 12, 2019
*
* References:  none
*
* Interactions:   Press the escape key to quit.
*                 Press S to Soar
Press 0 for ortho view
Press 1 for frust view
press 2 for perspecitve view
press w to finish dropping
press left arrow or a to move left
press right arrow or d to move left

press up arrow or 'w' to finish dropping
press down arrow or 's' to  drop one square

press 'x' to rotate the current piece

press 'r' to reset game
Press right click to bring up menu
*
*******************************************/



#include<vector>
#include<random>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iostream>


#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif
//bitmap files

//#include "blue.bmp"
//#include "cyan.bmp"
//#include "green.bmp"
//#include "magenta.bmp"
//#include "orange.bmp"
//#include "red.bmp"
//#include "yellow.bmp"
//#include "onlinepngtools.bmp"
static unsigned int texture[1]; // Array of texture indices.
static GLUquadricObj *qobj; // Create a pointer to a quadric object.


using namespace std;

class coords {
public:
	int x;
	int y;
};


class layout {
public:
	std::vector<coords> pos;
};

class piece {
public:
	float color[3];
	int anchorx=2;
	int anchory=22;
	int numOrints;
	int orint = 0;
	std::vector<layout> orintLayouts;
	
	virtual void setOrs()=0;
	piece() {
		color[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		color[2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		while (color[0] >= .9&&color[1] >= .9&&color[2] >= .9) {
			color[0] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color[1] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			color[2] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

		}
	}
};

class oPiece : public piece
{
public:
	
	void setOrs() {
		coords* temp= new coords;
		layout *temp2= new layout;
		//orint0
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);

	}

	oPiece():piece() {
		numOrints = 1;
		setOrs();
	}
};

class sPiece : public piece
{
public:
	void setOrs() {
		coords* temp= new coords;
		layout *temp2= new layout;
		//orint0
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	sPiece() :piece(){
		numOrints = 2;

		setOrs();
	}

};

class zPiece : public piece
{
public:
	void setOrs() {
		coords* temp= new coords;
		layout *temp2= new layout;
		//orint0
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 1;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	zPiece() : piece(){
		numOrints = 2;

		setOrs();
	}
};

class iPiece : public piece
{
public:

	void setOrs() {
		coords* temp = new coords;
		layout *temp2 = new layout;
		//orint0
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -2;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -2;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	iPiece(): piece() {
		numOrints = 2;

		setOrs();
	}
};

class lPiece : public piece
{
public:

	void setOrs() {
		coords* temp = new coords;
		layout *temp2 = new layout;
		//orint0
		temp->x = -1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint2
		temp = new coords;
		temp2 = new layout;
		temp->x = 1;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint3
		temp = new coords;
		temp2 = new layout;
		temp->x = -1;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	lPiece(): piece() {
		numOrints = 4;

		setOrs();
	}
};

class jPiece : public piece
{
public:

	void setOrs() {
		coords* temp = new coords;
		layout *temp2 = new layout;
		//orint0
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 1;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint2
		temp = new coords;
		temp2 = new layout;
		temp->x = -1;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint3
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	jPiece(): piece() {
		numOrints = 4;
		setOrs();
	}
};

class tPiece : public piece
{
public:

	void setOrs() {
		coords* temp = new coords;
		layout *temp2 = new layout;
		//orint0
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint1
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint2
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
		//orint3
		temp = new coords;
		temp2 = new layout;
		temp->x = 0;
		temp->y = 1;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = -1;
		temp->y = 0;
		temp2->pos.push_back(*temp);
		temp = new coords;
		temp->x = 0;
		temp->y = -1;
		temp2->pos.push_back(*temp);
		this->orintLayouts.push_back(*temp2);
	}

	tPiece(): piece() {
		numOrints = 4;
		setOrs();
	}
};

class block {
public:
	bool occupied;
	bool fixed;
	float color[4] = { 0,0,0,1 };
};

class board {
public:
	block* grid[10][24];
	piece *curr;
	piece *next;
	int score = 0;

	void getPiece() {
		int p = rand() % 7;
		switch (p) {
		case 1:
			curr = new sPiece;
			break;
		case 2:
			curr = new zPiece;
			break;
		case 3:
			curr = new iPiece;
			break;
		case 4:
			curr = new lPiece;
			break;
		case 5:
			curr = new jPiece;
			break;
		case 6:
			curr = new tPiece;
			break;
		default:
			curr = new oPiece;
			break;
		}
		setPiece();
	}
	board() {
		srand(time(NULL));
	
		for (int c = 0; c < 10; c++) {
			for (int r = 0; r < 24; r++) {
				grid[c][r] = new block();
			}
		}
		getPiece();
	}
	layout getLayout() {
		std::vector<layout>::iterator it = curr->orintLayouts.begin();
		int i = 0;
		while (i < curr->orint) {
			it++;
			i++;
		}
		return *it;
	}
	void setPiece() {
		grid[curr->anchorx][curr->anchory]->occupied = true;
		grid[curr->anchorx][curr->anchory]->color[0] = curr->color[0];
		grid[curr->anchorx][curr->anchory]->color[1] = curr->color[1];
		grid[curr->anchorx][curr->anchory]->color[2] = curr->color[2];
		layout lay = getLayout();
		std::vector<coords>::iterator it2 = lay.pos.begin();
		while (it2 != lay.pos.end()) {
			grid[curr->anchorx+it2->x][curr->anchory+it2->y]->occupied = true;
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[0] = curr->color[0];
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[1] = curr->color[1];
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[2] = curr->color[2];
			it2++;
		}
	}
	void unsetPiece() {
		grid[curr->anchorx][curr->anchory]->occupied = false;
		grid[curr->anchorx][curr->anchory]->color[0] = 0;
		grid[curr->anchorx][curr->anchory]->color[1] = 0;
		grid[curr->anchorx][curr->anchory]->color[2] = 0;
		layout lay = getLayout();
		
		std::vector<coords>::iterator it2 = lay.pos.begin();
		while (it2 != lay.pos.end()) {
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->occupied = false;
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[0] = 0;
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[1] = 0;
			grid[curr->anchorx + it2->x][curr->anchory + it2->y]->color[2] = 0;
			it2++;
		}
	}
	bool collide(bool right ) {
		layout lay = getLayout();
		if (!right) {
			if (curr->anchorx==9 || grid[curr->anchorx + 1][curr->anchory]->occupied == true)
				return true;
			std::vector<coords>::iterator it2 = lay.pos.begin();
			while (it2 != lay.pos.end()) {
				if (curr->anchorx + it2->x >= 9 || (grid[curr->anchorx + it2->x + 1][curr->anchory + it2->y]->occupied == true))
					return true;
				it2++;
			}
		}
		else {
			if (curr->anchorx==0||grid[curr->anchorx -1][curr->anchory]->occupied == true)
				return true;
			std::vector<coords>::iterator it2 = lay.pos.begin();
			while (it2 != lay.pos.end()) {
				if (curr->anchorx + it2->x <= 0 || grid[curr->anchorx + it2->x-1][curr->anchory + it2->y]->occupied == true)
					return true;
				it2++;
			}
		}
		return false;
	}
	int settle() {
		unsetPiece();
		layout lay = getLayout();
		if (curr->anchory == 0 || grid[curr->anchorx][curr->anchory - 1]->occupied == true ) {
			std::cout << "cant move down anymore" << std::endl;
			setPiece();
			 score+=checkComplete();
			getPiece();
			return true;

		}
		std::vector<coords>::iterator it2 = lay.pos.begin();
		while (it2 != lay.pos.end()) {
			if (curr->anchory + it2->y <= 0 || grid[curr->anchorx + it2->x][curr->anchory + it2->y - 1]->occupied == true) {
				std::cout << "cant move down anymore v2" << std::endl;
				setPiece();
				score+=checkComplete();
				getPiece();
				return true;
			}
			it2++;
		}
		stepDown();
		setPiece();
		return false;
	}
	
	void stepDown() {
		this->curr->anchory--;

	}

	bool stepLeft() {
		unsetPiece();
		bool x= collide(true);
		if (!x) {
			curr->anchorx--;
		}
		setPiece();

		return x;
	}
	int stepRight() {
		unsetPiece();
		bool x= collide(false);
		if (!x) {
			curr->anchorx++;
		}
		setPiece();

		return curr->anchorx;
	}

	bool canRotate() {
		int nextorint;
		layout nextlay;
		unsetPiece();
		bool shift = false;
		bool shift2 = false;
		if (curr->orint < curr->numOrints - 1)
			nextorint = curr->orint + 1;
		else
			nextorint = 0;
		nextlay = curr->orintLayouts[nextorint];
		std::vector<coords>::iterator it1 = nextlay.pos.begin();
		for (int i = 0; i < 3; i++) {
			if (curr->anchorx + nextlay.pos[i].x > 9) {
				shift = true;
				break;
			}
			else if (curr->anchorx + nextlay.pos[i].x < 0) {
				shift2 = true;
				break;
			}
			if (grid[curr->anchorx + nextlay.pos[i].x][curr->anchory + it1->y]->occupied) {
				setPiece();
				return shift;

			}
		}
		if (shift) {
			if (grid[curr->anchorx - 1][curr->anchory + it1->y]->occupied) {
				setPiece();
				return shift;
			}

			for (int i = 0; i < 3; i++) {
				if (curr->anchorx + nextlay.pos[i].x - 1 > 9) {
					shift = true;
					break;
				}
				if (grid[curr->anchorx + nextlay.pos[i].x - 1][curr->anchory + it1->y]->occupied) {
					setPiece();
					return shift;

				}
			}
			curr->anchorx--;
		}
		else if (shift2) {
			int x = 0;
			for (int i = 0; i < 3; i++) {
				if (curr->anchorx + nextlay.pos[i].x < x) {
					x = curr->anchorx + nextlay.pos[i].x;
					
				}
			}
			if (x == -1) {
				if (grid[curr->anchorx + 1][curr->anchory + it1->y]->occupied) {
					setPiece();
					return shift;
				}

				for (int i = 0; i < 3; i++) {
					if (curr->anchorx + nextlay.pos[i].x + 1 < 0) {
						shift = true;
						break;
					}
					if (grid[curr->anchorx + nextlay.pos[i].x + 1][curr->anchory + it1->y]->occupied) {
						setPiece();
						return shift;

					}
				}
				curr->anchorx++;
			}
			else {
				if (grid[curr->anchorx + 2][curr->anchory + it1->y]->occupied) {
					setPiece();
					return shift;
				}

				for (int i = 0; i < 3; i++) {
					if (curr->anchorx + nextlay.pos[i].x + 2 < 0) {
						shift = true;
						break;
					}
					if (grid[curr->anchorx + nextlay.pos[i].x + 2][curr->anchory + it1->y]->occupied) {
						setPiece();
						return shift;

					}
				}
				curr->anchorx += 2;
			}
		}
		rightRotate();


		return shift;
	}
	
	void rightRotate() {

		if (curr->orint < curr->numOrints - 1) {
			curr->orint++;

		}
		else {
			curr->orint = 0;

		}
		setPiece();


	}
	int checkComplete() {
		int points=0;
		int numcomp = 0;
		for (int r = 23; r >= 0; r--) {
			bool fin = true;
			for (int c = 0; c < 10; c++) {
				if (!grid[c][r]->occupied) {
					fin = false;
				}
			}
			if (fin) {
				numcomp++;
				points += 100*numcomp;
				for (int a = r + 1; a < 24; a++) {
				
					for (int c = 0; c < 10; c++) {
						grid[c][a - 1] = grid[c][a];

					}
				}
				for (int c = 0; c < 10; c++) {
					grid[c][23] = new block();

				}
			}
		}
		return points;
	}

};
