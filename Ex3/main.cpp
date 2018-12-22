
#include <stdlib.h>
#include "glut.h"
#include "Camera.cpp"
#include "Vector3d.hpp"

Camera camera = Camera(Vector3d(0, 2, 8), Vector3d(7 * PI / 8, -PI / 2, 1));
Vector3d offset = Vector3d(0, 0, 0);
float moving_speed = 0.03;

float fTranslate;
float fRotate    = 0.0f;
float fScale     = 1.0f;	// set inital scale value to 1.0f

bool bAnim = false;

int wHeight = 0;
int wWidth = 0;


double pot_x_off = 0;
double pot_y_off = 0;


void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{
	glPushMatrix();
	glTranslatef(0 + pot_x_off, 0 + pot_y_off, 4 + 1);
	glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
	glutSolidTeapot(1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0, 3.5);
	glScalef(5, 4, 1);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, 1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5, -1, 1.5);
	Draw_Leg();
	glPopMatrix();

	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(0, 0, -1);
	glScalef(100, 100, 1);
	glutSolidCube(1.0);
	glPopMatrix();
}

void updateView(int width, int height)
{
	glViewport(0,0,width,height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	float whRatio = (GLfloat)width/(GLfloat)height;
	gluPerspective(60.0f, whRatio, 0.1, 800);
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
}

void reshape(int width, int height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}
	wHeight = height;
	wWidth = width;
	updateView(wHeight, wWidth);
}

void idle()
{
	glutPostRedisplay();
}

void key(unsigned char k, int x, int y)
{
	switch(k)
	{
	case 27:
	case 'q': {exit(0); break; }

	case ' ': {bAnim = !bAnim; break;}

	case 'a': {
		offset += Vector3d(-moving_speed, 0, 0);
		break;
			  }
	case 'd': {
		offset += Vector3d(moving_speed, 0, 0);
		break;
			  }
	case 'w': {
		offset += Vector3d(0, 0, -moving_speed);
		break;
			  }
	case 's': {
		offset += Vector3d(0, 0, moving_speed);
		break;
			  }

			  //²èºøÏà¹Ø²Ù×÷
	case 'l': {//todo, hint:use the ARRAY that you defined, and notice the teapot can NOT be moved out the range of the table.
		if(pot_x_off < 2)
			pot_x_off += 0.2;
		break;
			  }
	case 'j': {//todo
		if(pot_x_off > -2)
			pot_x_off -= 0.2;
		break;
			  }
	case 'i': {//todo
		if(pot_y_off < 2)
			pot_y_off += 0.2;
		break;
			  }
	case 'k': {//todo
		if(pot_y_off > -2)
			pot_y_off -= 0.2;
		break;
			  }
	}
}
void keyup(unsigned char k, int x, int y)
{
	switch (k)
	{
		case 'a': {
			offset = Vector3d(0, 0, 0);
			break;
		}
		case 'd': {
			offset = Vector3d(0, 0, 0);
			break;
		}
		case 'w': {
			offset = Vector3d(0, 0, 0);
			break;
		}
		case 's': {
			offset = Vector3d(0, 0, 0);
			break;
		}
	}

}


void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	camera.CameraMove(offset);
			
	camera.CameraLookat();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	glRotatef(fRotate, 0, 1.0f, 0);			// Rotate around Y axis
	glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);
	Draw_Scene();						// Draw Scene

	if (bAnim) fRotate    += 0.5f;

	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple Balance");

	glutIgnoreKeyRepeat(1);
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyup);

	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


