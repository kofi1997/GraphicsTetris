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





struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char *data;
};

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
static BitMapFile *getBMPData(string filename)
{
	BitMapFile *bmp = new BitMapFile;
	unsigned int size, offset, headerSize;

	// Read input file name.
	ifstream infile(filename.c_str(), ios::binary);

	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char *)&offset, 4);

	// Get the header size of the bitmap.
	infile.read((char *)&headerSize, 4);

	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read((char *)&bmp->sizeX, 4);
	infile.read((char *)&bmp->sizeY, 4);

	// Allocate buffer for the image.
	size = bmp->sizeX * bmp->sizeY * 24;
	bmp->data = new unsigned char[size];

	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char *)bmp->data, size);

	// Reverse color from bgr to rgb.
	int temp;
	for (int i = 0; i < size; i += 3)
	{
		temp = bmp->data[i];
		bmp->data[i] = bmp->data[i + 2];
		bmp->data[i + 2] = temp;
	}

	return bmp;
}

// Load external textures.
void loadExternalTextures()
{
// Local storage for bmp image data.
BitMapFile *image[1];

// Load the texture.
image[0] = getBMPData("Textures/launch.bmp");

// Bind launch image to texture index[0].
glBindTexture(GL_TEXTURE_2D, texture[0]);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0,
GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);
}





//GLOBALS
int sizex = 500;
int sizey = 500;
int view = 2;

bool useTex = true;

static float a = 1; // Red ambient reflectance.
static float d = 1.0; // Red diffuse reflectance.
static float s = 0.7; // White specular reflectance.(1.0 or 0.0) 
static float h = 128; // Shininess.

static float t = 0.0; // Quadratic attenuation factor.(0 or 1)
static bool over = false;

int closestName;
static unsigned int buffer[1024]; // Hit buffer.
static int hits; // Number of entries in hit buffer.
bool isSelecting = false;

bool frame = false;

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

// Process hit buffer to find record with smallest min-z value.
// Copied exactly from BallAndTorusPicking
void findClosestHit(int hits, unsigned int buffer[])
{
	unsigned int *ptr, minZ;

	minZ = 0xffffffff; // 2^32 - 1
	ptr = buffer;
	closestName = 0;
	for (int i = 0; i < hits; i++)
	{
		ptr++;
		if (*ptr < minZ)
		{
			minZ = *ptr;
			ptr += 2;
			closestName = *ptr;
			ptr++;
		}
		else ptr += 3;
	}

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
	sizex = w;
	sizey = h;
	setProjection();

	
	
}


void setProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(view==0)
		glOrtho(0, 100, 0, 100, 1.9, 20);
	else if(view==1)
		glFrustum(0, 100, 0, 100, 1.9, 20);
	else
		gluPerspective( 161, sizex/sizey,1,15);
	glMatrixMode(GL_MODELVIEW);
}

void drawFrame() {
	glLineWidth(5);
	if(frame){
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
		glVertex3f(20, 0, -2);
		glVertex3f(20, 80, -2);
		//
		glVertex3f(20, 0, -.85);
		glVertex3f(60, 0, -.85);
		glVertex3f(60, 0, -.85);
		glVertex3f(60, 80, -.85);
		glVertex3f(60, 80, -.85);
		glVertex3f(20, 80, -.85);
		glVertex3f(60, 0, -.85);
		glVertex3f(20, 0, -.85);
		glVertex3f(20, 0, -.85);
		glVertex3f(20, 80, -.85);
		glEnd();

		glLineWidth(1);
		//columns
		glBegin(GL_LINES);
		for (int i = 0; i < 10; i++) {
			glVertex3f(20 + i * 4, 80, -2);
			glVertex3f(20 + i * 4, 0, -2);
		}
		glEnd();
		glBegin(GL_LINES);
		for (int i = 0; i < 10; i++) {
			glVertex3f(20 + i * 4, 80, -.85);
			glVertex3f(20 + i * 4, 0, -.85);
		}
		glEnd();
		//rows
		glBegin(GL_LINES);
		for (int i = 0; i < 21; i++) {
			glVertex3f(20, 0 + i * 4, -2);
			glVertex3f(60, 0 + i * 4, -2);
		}
		glEnd();
		glBegin(GL_LINES);
		for (int i = 0; i < 21; i++) {
			glVertex3f(20, 0 + i * 4, -.85);
			glVertex3f(60, 0 + i * 4, -.85);
		}
		glEnd();
	}
	else {
		if (view == 2) {
			glBegin(GL_LINES);
			glVertex3f(20, 0, -.85);
			glVertex3f(60, 0, -.85);
			glVertex3f(60, 0, -.85);
			glVertex3f(60, 80, -.85);
			glVertex3f(60, 80, -.85);
			glVertex3f(20, 80, -.85);
			glVertex3f(60, 0, -.85);
			glVertex3f(20, 0, -.85);
			glVertex3f(20, 0, -.85);
			glVertex3f(20, 80, -.85);
			glEnd();

			glLineWidth(1);
			//columns
			glBegin(GL_LINES);
			for (int i = 0; i < 10; i++) {
				glVertex3f(20 + i * 4, 80, -.85);
				glVertex3f(20 + i * 4, 0, -.85);
			}
			glEnd();
			//rows
			glBegin(GL_LINES);
			for (int i = 0; i < 21; i++) {
				glVertex3f(20, 0 + i * 4, -.85);
				glVertex3f(60, 0 + i * 4, -.85);
			}
			glEnd();
		}
		else {
			glBegin(GL_LINES);
			glVertex3f(20, 0, -2);
			glVertex3f(60, 0, -2);
			glVertex3f(60, 0, -2);
			glVertex3f(60, 80, -2);
			glVertex3f(60, 80, -2);
			glVertex3f(20, 80, -2);
			glVertex3f(60, 0, -2);
			glVertex3f(20, 0, -2);
			glVertex3f(20, 0, -2);
			glVertex3f(20, 80, -2);

			glBegin(GL_LINES);
			for (int i = 0; i < 10; i++) {
				glVertex3f(20 + i * 4, 80, -2);
				glVertex3f(20 + i * 4, 0, -2);
			}
			glEnd();

			glBegin(GL_LINES);
			for (int i = 0; i < 21; i++) {
				glVertex3f(20, 0 + i * 4, -2);
				glVertex3f(60, 0 + i * 4, -2);
			}
			glEnd();
		}
	}
	
}

void drawBoard(board gameBoard) {
	
	for (int x = 0; x < 10; x++) {
		for (int y = 0; y < 22; y++) {
			if (gameBoard.grid[x][y]->occupied) {
				glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, gameBoard.grid[x][y]->color);
				glMaterialfv(GL_FRONT, GL_SPECULAR, gameBoard.grid[x][y]->color);
				
				glPushMatrix();
				glScalef(1, 1, .3);
				glTranslatef(x * 4 + 22, y * 4 + 2, -4.8);
				if (x == gameBoard.curr->anchorx && y == gameBoard.curr->anchory)
				{
					if (isSelecting) {
						glLoadName(1); 
						for (int i = 0; i < 3; i++) {
							glPushMatrix();
							glTranslatef(gameBoard.curr->orintLayouts[gameBoard.curr->orint].pos[i].x * 4, gameBoard.curr->orintLayouts[gameBoard.curr->orint].pos[i].y * 4, 0);
							glutSolidCube(3.8);
							glPopMatrix();
						}

					}
				}
				
				glutSolidCube(3.8);
				glPopMatrix();
			}
		}
	}
}

void drawTetris() {
	// Light quadratic attenuation factor.
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, .001);

	glColor3f(0.0, 0.0, 0.0);

	// Light position vectors.	
	float lightPos0[] = { (50.0f),
		(50.0f), (-2.5f), 1.0f };

	// Material property vectors.
	float matAmb[] = { 1, 1, 1, 1.0 };
	float matDif[] = { 1, 1, 1, 1.0 };
	float matSpec[] = { 1, 1, 1, 1.0 };
	float matShine[] = { h };
	float matAmbAndDif2[] = { 0.0, 0.9, 0.0, 1.0 };
	setProjection();

	if (view == 2)
		gluLookAt(50, 50, 10.0, 50.0, 50.0, -10.0, 0.0, 1, 0.0);

	//set PROJECTION MATRIX
	glDisable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(70, 90, -2);


	glScalef(0.05, .05, .05);

	char numString[7] = "score:";
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
	//glTranslatef(lightPos0[0], lightPos0[1], lightPos0[2]);
	//glColor3f(.0, .0, .0);
	//glutWireSphere(0.05, 8, 8);
	glPopMatrix();
}
void drawScene()
{


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	isSelecting = 0;
	drawTetris();
	
	glutSwapBuffers();
}
void timerFunction(int val) {
	int x=gameBoard->settle();
	drawScene();
	glutTimerFunc(300, timerFunction, 1);
	glutPostRedisplay();

}
void keyInput(unsigned char key, int x, int y)
{
	bool done = false;

	switch (key)
	{
	case 27:  //exit with escape key
		exit(0);
		break;
	case 'a':  //Move piece Left
		cout << "Move Left key hit" << endl;
		 gameBoard->stepLeft();
		break;
	case 'd':  //Move piece Right
		cout << "Move Right key hit" << endl;
		 gameBoard->stepRight();
		break;
	case 'x':  //Rotate piece
		cout << "Rotate key hit" << endl;
		cout << gameBoard->canRotate();
		//cout << gameBoard->curr->orint;
		break;
	case 'w':  //Drop into Resting spot
		cout << "Finish drop key hit" << endl;
		while (!done) {
			done = gameBoard->settle();
		}
		break;
	case 's':  //Step Down
		cout << "Step Down key hit" << endl;
		gameBoard->settle();
		break;
	case 'r':  //Step Down
		cout << "reset key hit" << endl;
		gameBoard=new board;
		break;
	case 'f':  //Step Down
		cout << "frame key hit" << endl;
		frame = !frame;
		break;
	case '0'://ortho view
		cout << "ortho view key hit" << endl;
		view = 0;
		break;
	case '1'://frust view
		cout << "frust view key hit" << endl;
		view = 1;
		break;
	case '2'://perspective view
		cout << "perspective view key hit" << endl;
		view = 2;
		break;
	} 
	
	glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y)
{

	if (key == GLUT_KEY_DOWN)
	{
		cout << "Step Down key hit" << endl;
		gameBoard->settle();
	}
	if (key == GLUT_KEY_UP)
	{
		bool done = false;
		while (!done) {
			done = gameBoard->settle();
		}
	}
	if (key == GLUT_KEY_LEFT)
	{
		cout << "Step Left key hit" << endl;
		gameBoard->stepLeft();
	}
	if (key == GLUT_KEY_RIGHT)
	{
		cout << "Step right key hit" << endl;
		gameBoard->stepRight();
	}
	drawScene();
}

// The mouse callback routine.
//based on BallAndTorusPicking pickFunction
void mousePickFunction(int button, int state, int x, int y)
{
	int viewport[4]; // Viewport data.

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) //mouse clicked
	{
		//**
		cout << "X= " << x << " Y = " << y << endl;

		cout << "left button pressed" << endl;
		glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.
		glSelectBuffer(1024, buffer); // Specify buffer to write hit
									  //records in selection mode
		(void)glRenderMode(GL_SELECT); // Enter selection mode.

									   //**************  Set new viewing volume ***********
									   // Save the viewing volume defined in the setProjection routine.
									   // the real projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();

		// Define a viewing volume corresponding to selecting in
		//3 x 3 region around the cursor.
		glLoadIdentity();
		gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);

		//real projection used, copied from setProjection
		
		//************** END of Set new viewing volume ***********

		//---------------  Selecting --------------
		//set up modelview for selecting
		glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
		glLoadIdentity();

		glInitNames(); // Initializes the name stack to empty.
		glPushName(0); // Puts name 0 on top of stack.

					   // Determine hits by drawing so that names are assigned.
		isSelecting = 1;
		drawTetris();
		//drawBoxAndCubes();  //ALL drawing must be done
		//NO MESSING with projection matrix

		hits = glRenderMode(GL_RENDER); // Return to rendering mode, returning number of hits.
		cout << "Number of hit = " << hits << endl;
		// Determine closest of the hit objects (if any).
		// closest object name will be save in closestName.
		findClosestHit(hits, buffer);

		//** Do something special with knowledge of what was hit
		cout << "closest hit = " << closestName << endl;
		//---------------  END OF Selecting --------------
		if(closestName==1)
			gameBoard->canRotate();
		//back to drawing mode
		// Restore viewing volume of the resize routine and return to modelview mode.
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);

		glutPostRedisplay();  //redraw for real, knowing what was hit.

	} // end if left button pressed


} //end mousePickFunction

void top_menu(int id)
{
	if (id == 1) exit(0);
	if (id == 2) gameBoard = new board();
	if (id == 3) gameBoard->canRotate();

	glutPostRedisplay();

}


// Routine to make the menu.
void makeMenu(void)
{
	// The sub-menu is created first (because it should be visible when the top
	// menu is created): its callback function is registered and menu entries added.
	
	glutCreateMenu(top_menu);
	
	glutAddMenuEntry("Quit", 1);
	glutAddMenuEntry("Reset", 2);
	glutAddMenuEntry("rotate", 3);


	// The menu is attached to a mouse button.
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char ** const argv)
{
	//printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Tetris!");



	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutMouseFunc(mousePickFunction);
	glutSpecialFunc(specialKeyInput);
	//glutMotionFunc(mouseMotion);
	//glutIdleFunc(idleFunction);
	glutTimerFunc(300, timerFunction, 1);
	makeMenu();

	glutMainLoop();
	return 0;
}