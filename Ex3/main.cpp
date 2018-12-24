
#include <stdlib.h>
#include "glut.h"
#include "Camera.cpp"
#include "Vector3d.hpp"

Camera camera = Camera(Vector3d(0, -4, 4), Vector3d(-5 * PI / 8, 0, 1));
Vector3d camera_move_offset = Vector3d(0, 0, 0);
Vector3d camera_rotate_offset = Vector3d(0, 0, 0);
float moving_speed = 0.01f;
float rotation_sensitity = 0.003f;

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
	//glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
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

	case 'd': {
		camera_move_offset += Vector3d(-moving_speed * sinf(camera.rotation.y), moving_speed, 0);
		break;
	}
	case 'a': {
		camera_move_offset += Vector3d(moving_speed * sinf(camera.rotation.y), -moving_speed * cosf(camera.rotation.y), 0);
		break;
	}
	case 's': {
		camera_move_offset += Vector3d(moving_speed * cosf(camera.rotation.y), moving_speed * sinf(camera.rotation.y), 0);
		break;
	}
	case 'w': {
		camera_move_offset += Vector3d(-moving_speed * cosf(camera.rotation.y), -moving_speed * sinf(camera.rotation.y), 0);
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
			camera_move_offset = Vector3d(0, 0, 0);
			break;
		}
		case 'd': {
			camera_move_offset = Vector3d(0, 0, 0);
			break;
		}
		case 'w': {
			camera_move_offset = Vector3d(0, 0, 0);
			break;
		}
		case 's': {
			camera_move_offset = Vector3d(0, 0, 0);
			break;
		}
	}

}

void specialkey(int k, int x, int y)
{
	switch (k)
	{
		case GLUT_KEY_UP: {
			camera_rotate_offset += Vector3d(rotation_sensitity, 0, 0);
			break;
		}
		case GLUT_KEY_DOWN: {
			camera_rotate_offset += Vector3d(-rotation_sensitity, 0, 0);
			break;
		}
		case GLUT_KEY_RIGHT: {
			camera_rotate_offset += Vector3d(0, -rotation_sensitity, 0);
			break;
		}
		case GLUT_KEY_LEFT: {
			camera_rotate_offset += Vector3d(0, rotation_sensitity, 0);
			break;
		}
	}
}
void specialkeyup(int k, int x, int y)
{
	switch (k)
	{
	case GLUT_KEY_UP: {
		camera_rotate_offset = Vector3d(0, 0, 0);
		break;
	}
	case GLUT_KEY_DOWN: {
		camera_rotate_offset = Vector3d(0, 0, 0);
		break;
	}
	case GLUT_KEY_LEFT: {
		camera_rotate_offset = Vector3d(0, 0, 0);
		break;
	}
	case GLUT_KEY_RIGHT: {
		camera_rotate_offset = Vector3d(0, 0, 0);
		break;
	}
	}
}

void mousemove()
{

}

void redraw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();									// Reset The Current Modelview Matrix

	if(camera_move_offset.x || camera_move_offset.y || camera_move_offset.z)
		camera.CameraMove(camera_move_offset);
	if (camera_rotate_offset.x || camera_rotate_offset.y)
		camera.CameraRotate(camera_rotate_offset);
	camera.CameraLookat();
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_pos[] = {5,5,5,1};

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glEnable(GL_LIGHT0);

	//glRotatef(-90, 1, 0, 0);
	glScalef(0.2, 0.2, 0.2);

	Draw_Scene();						// Draw Scene

	glutSwapBuffers();
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple Ballance");

	glutIgnoreKeyRepeat(1);
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);	
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyup);
	glutSpecialFunc(specialkey);
	glutSpecialUpFunc(specialkeyup);

	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}


