#include"ball.hpp"
#include<stdio.h>
#include<stdlib.h>

#define ModelSize 6

//模型路径
std::string ObjPath[ModelSize] =
{ 
	"asset/s1/s1.obj",
	"asset/s2/s2.obj",
	"asset/s3/s3.obj",
	"asset/s4/s4.obj",
	"asset/s5/s5.obj",
	"asset/s6/s6.obj"
};

std::string MtlPath[ModelSize] =
{
	"asset/s1/s1.mtl",
	"asset/s2/s2.mtl",
	"asset/s3/s3.mtl",
	"asset/s4/s4.mtl",
	"asset/s5/s5.mtl",
	"asset/s6/s6.mtl"
};

std::string SpherePath = "sphere_init.obj";

float FrictionModule = 0.005;//通用摩擦系数 0.005还行
float FrictionModule_Final = 0;//最后一段摩擦系数
float stime = 1.0 / 1600.0;//时间片长度
Vector3d offset(0, 1, -2);//小球初始位置
Vector3d g(0, -9.8, 0);
float fSet = 5;//给力大小
float M = 0.5;//小球质量

//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 1.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;

ObjLoader objSphere;

vector<ObjLoader> objModel;

ball HelloBall;

Vector3d Nf(0, 0, 0);

//重新开始函数
void Reset()
{
	HelloBall.SetOffset(offset);
	HelloBall.Init();
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	//char mode[64];

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	//stime = time - timebase;
	if (time - timebase > 1000) {
		
		sprintf_s(buffer, "FPS:%4.2f",
			frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}

	char *c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}

//安置光源
void setLightRes() {
	GLfloat lightPosition[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING); //启用光源
	glEnable(GL_LIGHT0);   //使用指定灯光
}

//初始化
void init() {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("ObjLoader");
	//glewInit();
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	setLightRes();
	glEnable(GL_DEPTH_TEST);
}

void display()
{
	glColor3f(1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, -1.0f, -12.0f);
	//glScalef(0.02f, 0.02f, 0.02f);
	setLightRes();
	glPushMatrix();

	gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	HelloBall.Redraw();

	glPushMatrix();
	for (int i = 0;i < objModel.size();i++)
	{
		objModel[i].ElementDraw();
	}
	glPopMatrix();
		
	glPopMatrix();
	
	getFPS();
	glutSwapBuffers();
	
}

void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (GLdouble)width / (GLdouble)height, 1.0f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
}

//移动鼠标360观察模型
void moseMove(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		oldPosX = x; oldPosY = y;
	}
}
void changeViewPoint(int x, int y)
{
	int temp = x - oldPosX;
	degree += temp;
	oldPosX = x;
	oldPosY = y;
}

void myIdle()
{
	//TouchGet();
	HelloBall.GetFrame(Nf,stime);
	glutPostRedisplay();
	//printf("%d %d %d", Sx, Sy, Sz);
}

void key(unsigned char k, int x, int y)
{
	switch (k) {
	case'w':Nf = Vector3d(0, 0, -fSet) + Nf;break;
	case's':Nf = Vector3d(0, 0,  fSet) + Nf;break;
	case'a':Nf = Vector3d(-fSet, 0, 0) + Nf;break;
	case'd':Nf = Vector3d(+fSet, 0, 0) + Nf;break;
	}
}

void KeyUp(unsigned char k, int x, int y)
{
	switch (k) {
	case'w':Nf.z = 0;break;
	case's':Nf.z = 0;break;
	case'a':Nf.x = 0;break;
	case'd':Nf.x = 0;break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init();

	HelloBall = ball(Vector3d(0, 0.381, 4.7), offset, 0.15, SpherePath, M);
	FaceCloud::MaxFaceCloud->ClearFaceinCloud();

	for (int i = 0;i < ModelSize;i++)
	{
		if (i == 4)
		{
			objModel.push_back(ObjLoader(ObjPath[i], FrictionModule_Final));
			continue;
		}
		objModel.push_back(ObjLoader(ObjPath[i], FrictionModule));
	}
	
	FaceCloud::MaxFaceCloud->Insort(2);
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutKeyboardFunc(key);
	glutIgnoreKeyRepeat(1);
	glutKeyboardUpFunc(KeyUp);
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}
