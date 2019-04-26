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
Press F to make the wings flap
Press 0 for top view
Press 1 for left view
press 2 for front view
press 3 for right view
press 4 for back view
press up arrow to increase fleight speed
Press down arrow to decrease fleight speed
Press left click and drag a tree to move it (in top view only)" << endl;
*
*******************************************/

#include <iostream>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include <vector>
#include "classes.cpp"

#define PI 3.14159265


using namespace std;

//GLOBALS
int sizex = 500;
int sizey = 500;
board* gameBoard;


void drawScene();
void printInteraction()
{
	cout << "S to Soar" << endl;
	cout << "F to make the wings flap" << endl;
	cout << "0 for top view" << endl;
	cout << "1 for left view" << endl;
	cout << "2 for front view" << endl;
	cout << "3 for right view" << endl;
	cout << "4 for back view" << endl;
	cout << "up arrow to increase fleight speed" << endl;
	cout << "down arrow to decrease fleight speed" << endl;
	cout << "left click and drag a tree to move it (in top view only)" << endl;

}

void setup()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glLineWidth(5);
	gameBoard = new board();
}

void setProjection();  //function prototype before function called

void resize(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	setProjection();
	sizex = w;
	sizey = h;
	/*
	//Replaced with call to setProjection
	//  here and
	//  in drawScene, since needs to
	//  reflect user's choice.
	*/
}


void setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0, 30, 0, 30, 1.9, 5);
	glFrustum(0, 30, 0, 30, 1.9, 5);
	glMatrixMode(GL_MODELVIEW);
}

void drawFrame() {
	glLineWidth(5);
	//outline
	glBegin(GL_LINES);
	glVertex3f(0, 0, -2.4);
	glVertex3f(10, 0, -2.4);
	glVertex3f(10, 0, -2.4);
	glVertex3f(10, 20, -2.4);
	glVertex3f(10, 20, -2.4);
	glVertex3f(0, 20, -2.4);
	glVertex3f(10, 0, -2.4);
	glVertex3f(0, 0, -2.4);
	glEnd();

	glLineWidth(1);
	//columns
	glBegin(GL_LINES);
	for (int i = 0; i < 10; i++) {
		glVertex3f(0+i, 20, -2.4);
		glVertex3f(0+i, 0, -2.4);
	}
	glEnd();
	//rows
	glBegin(GL_LINES);
	for (int i = 0; i < 21; i++) {
		glVertex3f(0, 0+i, -2.4);
		glVertex3f(10, 0+i, -2.4);
	}
	glEnd();

}

void drawBoard(board gameBoard) {
	
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 24; y++) {
			if (gameBoard.grid[x][y]->occupied) {
				glColor3f(gameBoard.grid[x][y]->color[0], gameBoard.grid[x][y]->color[1], gameBoard.grid[x][y]->color[2]);
				//glColor3i(150,0,200);
				//glColor3f(gameBoard.grid[x][y].color);
				glPushMatrix();
				glTranslatef(x+.5, y+.5, -3);
				glutSolidCube(.9);
				glPopMatrix();
			}
		}
	}
}
void drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);

	//set PROJECTION MATRIX
	//setProjection();
	drawFrame();
	drawBoard(*gameBoard);

	
	glutSwapBuffers();
}
void timerFunction(int val) {
	int x=gameBoard->settle();
	//cout << x << endl;
	drawScene();
	glutTimerFunc(300, timerFunction, 1);
	glutPostRedisplay();

}
void keyInput(unsigned char key, int x, int y)
{
	//does nothing yet
	switch (key)
	{
	case 27:  //exit with escape key
		exit(0);
		break;
	case 'a':  //Make bird soar
		cout << "Move Left key hit" << endl;
		 gameBoard->stepLeft();
		break;
	case 'd':  //Make bird soar
		cout << "Move Right key hit" << endl;
		 gameBoard->stepRight();
		break;
	case 'z':  //Make bird soar
		cout << "Move Left key hit" << endl;
		gameBoard->leftRotate();
		//cout << gameBoard->curr->orint;

		break;
	case 'x':  //Make bird soar
		cout << "Move Right key hit" << endl;
		cout << gameBoard->canRotate(true);
		//cout << gameBoard->curr->orint;
		break;
	case 'w':  //Make bird soar
		cout << "finish drop key hit" << endl;
		bool done = false;
		while(!done)
			done= gameBoard->settle();
		break;
	} //end switch
	glutPostRedisplay();
}




void check() {

}

int main(int argc, char ** const argv)
{
	//printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tetris!");

	//bool dem;

	//dem = new bool[4][4];

	//cout << dem[2][2];
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	//glutMouseFunc(mousePickFunction);
	//glutSpecialFunc(specialKeyInput);
	//glutMotionFunc(mouseMotion);
	//glutIdleFunc(idleFunction);
	glutTimerFunc(300, timerFunction, 1);
	glutMainLoop();
	return 0;
}