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

static float a = 1; // Red ambient reflectance.
static float d = 1.0; // Red diffuse reflectance.
static float s = 0.7; // White specular reflectance.(1.0 or 0.0) 
static float h = 128; // Shininess.

static float t = 0.0; // Quadratic attenuation factor.(0 or 1)
static bool over = false;

float lightAmb[] = { 1.0, 1.0, 1.0, 1 };
float lightDifAndSpec0[] = { 1, 1.0, 1.0, 1.0 };


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

	glEnable(GL_DEPTH_TEST); // Enable depth testing.


							 // Turn on OpenGL lighting.
	glEnable(GL_LIGHTING);


	// Light0 properties.
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec0);


	glEnable(GL_LIGHT0); // Enable particular light source.
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE); // Enable local viewpoint
}

void setProjection();  //function prototype before function called

					   // Routine to draw a stroke character string.
void writeStrokeString(void *font, const char *string)
{
	const char *c;
	for (c = string; *c != '\0'; c++) glutStrokeCharacter(font, *c);
}

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
	glFrustum(0, 100, 0, 100, 1.9, 20);
	glMatrixMode(GL_MODELVIEW);
}

void drawFrame() {
	glLineWidth(5);
	//outline
	glBegin(GL_LINES);
	glVertex3f(20, 0, -2);
	glVertex3f(60, 0, -2);
	glVertex3f(60, 0, -2);
	glVertex3f(60, 80, -2);
	glVertex3f(60, 80, -2);
	glVertex3f(20, 80, -2);
	glVertex3f(60, 0, -2);
	glVertex3f(20, 0, -2);
	glEnd();

	glLineWidth(1);
	//columns
	glBegin(GL_LINES);
	for (int i = 0; i < 10; i++) {
		glVertex3f(20+i*4, 80, -2);
		glVertex3f(20+i*4, 0, -2);
	}
	glEnd();
	//rows
	glBegin(GL_LINES);
	for (int i = 0; i < 21; i++) {
		glVertex3f(20, 0+i*4, -2);
		glVertex3f(60, 0+i*4, -2);
	}
	glEnd();

}

void drawBoard(board gameBoard) {
	
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 22; y++) {
			if (gameBoard.grid[x][y]->occupied) {
				//glColor3f(gameBoard.grid[x][y]->color[0], gameBoard.grid[x][y]->color[1], gameBoard.grid[x][y]->color[2]);
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, gameBoard.grid[x][y]->color);
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gameBoard.grid[x][y]->color);
				glMaterialfv(GL_FRONT, GL_SPECULAR, gameBoard.grid[x][y]->color);
				//glMaterialfv(GL_FRONT, GL_DIFFUSE, gameBoard.grid[x][y]->color);

				//glColor3i(150,0,200);
				//glColor3f(gameBoard.grid[x][y].color);
				glPushMatrix();
				glScalef(1,1,.3);
				glTranslatef(x*4+22, y*4+2, -4.8);
				glutSolidCube(3.8);
				glPopMatrix();
			}
		}
	}
}
void drawScene()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	// Light quadratic attenuation factor.
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, t);

	glColor3f(0.0, 0.0, 0.0);

	// Light position vectors.	
	float lightPos0[] = { (50.0f),
		(50.0f ), (2.5f), 1.0f };

	// Material property vectors.
	float matAmb[] = { 1, 1, 1, 1.0 };
	float matDif[] = { 1, 1, 1, 1.0 };
	float matSpec[] = { 1, 1, 1, 1.0 };
	float matShine[] = { h };
	float matAmbAndDif2[] = { 0.0, 0.9, 0.0, 1.0 };


	//set PROJECTION MATRIX
	//setProjection();
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(70, 90, -2);

	
	glScalef(0.05, .05, .05);

	// convert a number to a string for printing
	char numString[7]= "score:";
	writeStrokeString(GLUT_STROKE_ROMAN, numString);
	
	glPopMatrix();
	glPushMatrix();
	glTranslatef(70, 80, -2);
	glScalef(0.05, .05, .05);

	sprintf(numString, "%d", gameBoard->score);
	writeStrokeString(GLUT_STROKE_ROMAN, numString);


	glPopMatrix();

	drawFrame();

	glEnable(GL_LIGHTING);
	// Material properties of squares.
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShine);


	drawBoard(*gameBoard);

	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
	glColor3f(1.0, 1.0, 1.0);
	glutWireSphere(0.05, 8, 8);
	glPopMatrix();
	
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
	bool done = false;

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
	case 'x':  //Make bird soar
		cout << "Move Right key hit" << endl;
		cout << gameBoard->canRotate();
		//cout << gameBoard->curr->orint;
		break;
	case 'w':  //Make bird soar
		cout << "finish drop key hit" << endl;
		while (!done) {
			done = gameBoard->settle();
		}
		break;
	case 's':  //Make bird soar
		cout << "finish drop key hit" << endl;
		gameBoard->settle();
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