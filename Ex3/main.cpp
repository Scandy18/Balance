
#include <cstdlib>
#include <cstring>
#include "ObjLoader.hpp"
#include "glut.h"
#include "Camera.cpp"
#include "Vector3d.hpp"

using std::string;

std::string filePath = "total.obj";
std::string SpherePath = "sphere.obj";

ObjLoader objModel;
ObjLoader objSphere;

Camera camera = Camera(Vector3d(0, -2, 2), Vector3d(-5 * PI / 8, 0, 1));
Vector3d camera_move_offset = Vector3d(0, 0, 0);
Vector3d camera_rotate_offset = Vector3d(0, 0, 0);
float moving_speed = 0.01f;
float rotation_sensitity = 0.003f;

int wHeight = 0;
int wWidth = 0;

double pot_x_off = 0;
double pot_y_off = 0;

GLuint desk_tex;
GLuint ground_tex;

void Draw_Leg()
{
	glScalef(1, 1, 3);
	glutSolidCube(1.0);
}

void Draw_Scene()
{

	/*glPushMatrix();
	glTranslatef(0 + pot_x_off, 0 + pot_y_off, 4 + 1);
	//glRotatef(90, 1, 0, 0); //notice the rotation here, you may have a TRY removing this line to see what it looks like.
	glutSolidTeapot(1);
	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, desk_tex);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2d(0, 1);
	glVertex3f(0, 10, 0);
	glTexCoord2d(1, 1);
	glVertex3f(10, 10, 0);
	glTexCoord2d(1, 0);
	glVertex3f(10, 0, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, ground_tex);
	glBegin(GL_QUADS);
	glTexCoord2d(0, 0);
	glVertex3f(-10, -10, 0);
	glTexCoord2d(0, 1);
	glVertex3f(-10, 0, 0);
	glTexCoord2d(1, 1);
	glVertex3f(0, 0, 0);
	glTexCoord2d(1, 0);
	glVertex3f(0, -10, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);



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

	glPushMatrix();
	glTranslatef(0, 0, -1);
	glScalef(100, 100, 1);
	glutSolidCube(1.0);
	glPopMatrix();*/

	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -20.0f);
	glScalef(0.02f, 0.02f, 0.02f);

	glPushMatrix();

	glPushMatrix();
	glScalef(0.05f, 0.05f, 0.05f);
	//glTranslatef(Sx, Sy, Sz);
	objSphere.ElementDraw();
	glPopMatrix();

	glTranslatef(-6, 0, 4);//new
	glPushMatrix();
	glScaled(60, 60, 60);
	objModel.ElementDraw();
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
		camera_move_offset += Vector3d(-moving_speed * sinf(camera.rotation.y), moving_speed * cosf(camera.rotation.y), 0);
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

			  //茶壶相关操作
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
	if ((camera_rotate_offset.x || camera_rotate_offset.y) && camera.rotation.x >= CAM_LOW_BOUND && camera.rotation.x <= CAM_UP_BOUND) {
		camera.CameraRotate(camera_rotate_offset);
	}
	if (camera.rotation.x < CAM_LOW_BOUND) camera.rotation.x += rotation_sensitity;
	if (camera.rotation.x > CAM_UP_BOUND) camera.rotation.x -= rotation_sensitity;
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

int load_texture(string filename)
{
	int t_width, t_height, t_bytes;
	GLint last_texture_id = 0;
	GLuint texture_id = 0;
	GLubyte* pixels = 0;

	FILE *tf = fopen(filename.c_str(), "rb");
	if (tf == 0) return 0;

	//读取BMP文件头部信息
	fseek(tf, 0x0012, SEEK_SET);
	fread(&t_width, 4, 1, tf);
	fread(&t_height, 4, 1, tf);
	fseek(tf, 54, SEEK_SET);//54是BMP文件头部长度

	//计算总大小
	int line_bytes = t_width * 3;//rgb
	while (line_bytes % 4)
		++line_bytes;
	t_bytes = line_bytes * t_height;

	//存储像素信息，即图片内容
	pixels = (GLubyte*)malloc(t_bytes);
	if (pixels == 0) {
		fclose(tf);
		return 0;
	}
	if (fread(pixels, t_bytes, 1, tf) <= 0) {
		free(pixels);
		fclose(tf);
		return 0;
	}

	//分配一个纹理编号给该纹理
	glGenTextures(1, &texture_id);

	if (texture_id == 0) {
		free(pixels);
		fclose(tf);
		return 0;
	}

	//绑定纹理
	GLint lastTextureID = last_texture_id;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	//材质模型不匹配策略
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	/*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//载入纹理
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);

	free(pixels);
	return texture_id;
}

int main (int argc,  char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(480,480);
	int windowHandle = glutCreateWindow("Simple Ballance");

	objModel = ObjLoader(filePath);
	objSphere = ObjLoader(SpherePath);
	FaceCloud::MaxFaceCloud->Insort(2);

	desk_tex = load_texture("wood.bmp");
	ground_tex = load_texture("ground.bmp");

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


