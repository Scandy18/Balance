#include"ball.hpp"
#include<stdio.h>
#include<stdlib.h>
#include"time.h"
#include"Windows.h"

#define ModelSize 6
using std::string;
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
Vector3d offset(0, 0.5, -2);//小球初始位置
Vector3d g(0, -9.8, 0);
float fSet = 5;//给力大小
float M = 0.5;//小球质量
float second = 0;//游戏时间

int startTime;
int nowTime = 0;
int wHeight = 0;
int wWidth = 0;

//状态机
bool isGameOver = false;
bool isGameStart = false;
bool isGameComplete = false;
bool timer = false;

//ui 贴图
GLuint congrats;
GLuint gameOver;

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

void _gameStart() 
{
	isGameStart = true;
	timer = true;
	startTime = clock();
}

void _gameOver()
{
	startTime = 0;
	isGameOver = true;
	isGameComplete = false;
	timer = false;
}

void _gameComplete()
{
	startTime = 0;
	isGameComplete = true;
	isGameOver = false;
	timer = false;
}

void status(ball HelloBall)
{
	Vector3d position = HelloBall.GetCenter();
	if (!isGameComplete && position.y <= -1.2)
	{
		_gameOver();
	}
	if (position.y == 0 && (position.x<5.3&&position.x>3.3) && (position.z<1 && position.z>-1))
		_gameComplete();
}


//重新开始函数
void Reset()
{
	HelloBall.SetOffset(offset);
	HelloBall.Init();
}

BOOL WriteBitmapFile(char * filename, int width, int height, unsigned char * bitmapData)
{
	//填充BITMAPFILEHEADER
	BITMAPFILEHEADER bitmapFileHeader;
	memset(&bitmapFileHeader, 0, sizeof(BITMAPFILEHEADER));
	bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType = 0x4d42;	//BM
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//填充BITMAPINFOHEADER
	BITMAPINFOHEADER bitmapInfoHeader;
	memset(&bitmapInfoHeader, 0, sizeof(BITMAPINFOHEADER));
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biWidth = width;
	bitmapInfoHeader.biHeight = height;
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;
	bitmapInfoHeader.biCompression = BI_RGB;
	bitmapInfoHeader.biSizeImage = width * abs(height) * 3;

	//////////////////////////////////////////////////////////////////////////
	FILE * filePtr;			//连接要保存的bitmap文件用
	unsigned char tempRGB;	//临时色素
	int imageIdx;

	//交换R、B的像素位置,bitmap的文件放置的是BGR,内存的是RGB
	for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
	{
		tempRGB = bitmapData[imageIdx];
		bitmapData[imageIdx] = bitmapData[imageIdx + 2];
		bitmapData[imageIdx + 2] = tempRGB;
	}
	errno_t err;
	err = fopen_s(&filePtr,filename, "wb");
	if (NULL == filePtr)
	{
		return FALSE;
	}

	fwrite(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);

	fwrite(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	fwrite(bitmapData, bitmapInfoHeader.biSizeImage, 1, filePtr);

	fclose(filePtr);
	return TRUE;
}

//截图按序号保存
void grab(int width, int height)
{
	static void* screenData;
	RECT rc;
	int len = width * height * 3;
	screenData = malloc(len);
	memset(screenData, 0, len);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenData);

	char fileName;
	int i = 1;
	FILE * filePtr;
	char a[] = ".bmp";
	char b[10];
	_itoa_s(i, b, 10);
	strcat_s(b, a);
	errno_t err;
	err = fopen_s(&filePtr,b, "rb");
	while (filePtr != NULL)
	{
		i += 1;
		_itoa_s(i, b, 10);
		strcat_s(b, a);
		
		err = fopen_s(&filePtr,b, "rb");
	}


	WriteBitmapFile(b, width, height, (unsigned char *)screenData);

	free(screenData);
}

int load_texture(string filename)
{
	int t_width, t_height, t_bytes;
	GLint last_texture_id = 0;
	GLuint texture_id = 0;
	GLubyte* pixels = 0;

	errno_t err;
	FILE *tf;
	err= fopen_s(&tf,filename.c_str(), "rb");
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

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	//char mode[64];
	if (timer) {
		nowTime = clock();
		second = nowTime - startTime;
		second /= 1000;
	}

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	//stime = time - timebase;
	if (time - timebase > 1000) {
		
		sprintf_s(buffer, "FPS:%4.2f     Time:%4.2f",
			frame*1000.0 / (time - timebase),second);
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
	status(HelloBall);
	if (isGameOver)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, gameOver);

		glBegin(GL_QUADS);
		//glScalef(50.0f, 50.0f, 25.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.4f, -1.0f, 8.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.4f, -1.0f, 8.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.4f, 3.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-2.4f, 3.0f, 8.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	if (!isGameOver && isGameComplete) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, congrats);
		//glScalef(50.0f, 50.0f, 25.0f);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(-2.4f, -1.0f, 8.0f);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(2.4f, -1.0f, 8.0f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(2.4f, 3.0f, 8.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-2.4f, 3.0f, 8.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	glutSwapBuffers();
	
}

void reshape(int width, int height)
{
	wHeight = height;
	wWidth = width;
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
	case'w': {
		Nf = Vector3d(0, 0, -fSet) + Nf; 
		_gameStart();
		break;
	}
	case's': {
		Nf = Vector3d(0, 0, fSet) + Nf;
		_gameStart();
		break;
	}
	case'a': {
		Nf = Vector3d(-fSet, 0, 0) + Nf;
		_gameStart();
		break;
	}
	case'd': {
		Nf = Vector3d(+fSet, 0, 0) + Nf;
		_gameStart();
		break;
	}
	case'p':grab(wWidth, wHeight); break;
	case 'r': {//缺少摄像机位置初始化函数
		startTime = 0;
		isGameStart = false;
		isGameComplete = false;
		isGameOver = false;
		timer = false;
		second = 0;
		Reset();
		break;
	}
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
	
	gameOver = load_texture("gameover.bmp");
	congrats = load_texture("congrats.bmp");

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
