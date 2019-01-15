#include"ObjLoader.hpp"
#include<stdio.h>
#include<stdlib.h>

//模型路径
std::string filePath = "total_modified_2.0.obj";
std::string SpherePath = "sphere_init.obj";

ObjLoader objModel;
ObjLoader objSphere;

float Sx = 0;
float Sy = 0;
float Sz = 0;

Vector3d center(0, 0.381, 4.7);
Vector3d offset(0, 1, Sz);
//Vector3d Goff(0.78, 0.78, 0.78);
const float radius = 0.15;
float stime = 1.0 / 600.0;
Vector3d f(0, 0, 0);
Vector3d a(0, 0, 0);
Vector3d g(0, -9.8, 0);
Vector3d v(0, 0, 0);
float M = 0.5;


//实现移动鼠标观察模型所需变量
static float c = 3.1415926 / 180.0f;
static float r = 1.0f;
static int degree = 90;
static int oldPosY = -1;
static int oldPosX = -1;


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
	glTranslatef(0.0f, -1.0f, -8.0f);
	glScalef(0.02f, 0.02f, 0.02f);
	setLightRes();
	glPushMatrix();

	gluLookAt(r*cos(c*degree), 0, r*sin(c*degree), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glScalef(60, 60, 60);
	glTranslatef(offset.x / 1.2, offset.y / 1.2, offset.z / 1.2);
	//glTranslatef(Sx, 10700, Sz);
	objSphere.ElementDraw();
	glPopMatrix();
	//objModel.Draw();//绘制obj模型
	//glCallList(objModel.ListNode);
	//glTranslatef(-6, 0, 4);//new
	glPushMatrix();
	glScaled(60, 60, 60);
	objModel.ElementDraw();
	glPopMatrix();
	//glTranslatef(Sx, Sy, Sz);
	//glScaled(240, 240, 240);
	
	
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

void TouchGet()//获得接触面 所有法向量存放在NormalResult中
{
	queue<FaceCloud*> t;
	vector<FaceCloud*> result;
	vector<Vector3d> NormalResult;
	t.push(FaceCloud::MaxFaceCloud);
	while (t.size() != 0)
	{
		FaceCloud* m = t.front();
		t.pop();
		m->FaceCloudJudge(t, result, center+offset, radius);
	}

	for (int i = 0;i < result.size();i++)
	{
		FaceCloud* tCloud = result[i];
		result[i]->FaceJudge(NormalResult, center+offset, radius);
	}

	f = Vector3d(0, 0, 0);


	for (int i = 0;i < NormalResult.size();i++)
	{
		if (v.isZero())break;
		Vector3d BackNormal = ~NormalResult[i];
		//float vec = (BackNormal) % v;
		float angle = BackNormal.GetAngel(v);
		Vector3d t = BackNormal.UnitVector()*(!v)*angle;
		//t = t * 2 * 0.8;
		//v -= t;
		v -= t;
		f = ((~v)*M / stime)+f;//动量
	}

	f = g * M + f;
	if (f.x == 0 && f.y == 0 && f.z == 0) a = Vector3d(0, 0, 0);
	else
		a = f / M;//牛二
	offset = v * stime + offset;//位移
	v = a * stime + v;//速度变化
	
	printf("%f, %f, %f\n", offset.x, offset.y, offset.z);
}

void myIdle()
{
	TouchGet();
	glutPostRedisplay();
	//printf("%d %d %d", Sx, Sy, Sz);
}

void key(unsigned char k, int x, int y)
{
	switch (k) {
	case'w':Sy += 100;break;
	case's':Sy -= 100;break;
	case'a':Sx -= 100;break;
	case'd':Sx += 100;break;
	case'z':Sz += 100;break;
	case'c':Sz -= 100;break;
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	init();
	objSphere = ObjLoader(SpherePath);
	FaceCloud::MaxFaceCloud->ClearFaceinCloud();
	objModel = ObjLoader(filePath);
	
	FaceCloud::MaxFaceCloud->Insort(2);
	queue<FaceCloud*> t;
	vector<FaceCloud*> result;
	vector<Vector3d> NormalResult;
	t.push(FaceCloud::MaxFaceCloud);

	FaceCloud::MaxFaceCloud->FaceCloudJudge(t, result, center, radius);

	while (t.size() != 0)
	{
		FaceCloud* m = t.front();
		t.pop();
		m->FaceCloudJudge(t, result, center, radius);
	}

	for (int i = 0;i < result.size();i++)
	{
		FaceCloud* tCloud = result[i];
		result[i]->FaceJudge(NormalResult, center, radius);
	}

	/*for (int i = 0;i < FaceCloud::MaxFaceCloud->LastCloud.size();i++)
	{
		for (int j = 0;j < FaceCloud::MaxFaceCloud->LastCloud[i]->FaceinCloud.size();j++)
		{
			if (FaceCloud::MaxFaceCloud->LastCloud[i]->FaceinCloud[j]->NormalJudge(center, radius))
				printf("1\n");
		}
	}*/

	
	
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(moseMove);
	glutMotionFunc(changeViewPoint);
	glutKeyboardFunc(key);
	glutIdleFunc(myIdle);
	glutMainLoop();
	return 0;
}
