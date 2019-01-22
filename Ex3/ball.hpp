#pragma once
#include "Vector3d.hpp"
#include "ObjLoader.hpp"
#include "string"
#include "vector"
#include "queue"

#define MinRotate 0.6

using std::vector;
using std::queue;



class ball
{
public:
	ball() {}

	ball(Vector3d c, Vector3d o, float r, std::string p, float m1)
	{
		center = c;offset = o;radius = r;Path = p;m = m1;g = Vector3d(0, -9.8, 0);
		objModel = ObjLoader(Path,0.005);Rotate=0;
	}

	void GetFrame(Vector3d Nf, float stime)
	{
		Face::ChooseFace.clear();

		queue<FaceCloud*> t;//用于遍历所有面云
		vector<FaceCloud*> result;//存放需要遍历的面
		vector<Vector3d> NormalResult;//存储法向量
		vector<float> FrictionResult;//存储摩擦系数
		t.push(FaceCloud::MaxFaceCloud);//将最大面云压入队列
		while (t.size() != 0)
		{
			FaceCloud* m = t.front();
			t.pop();
			m->FaceCloudJudge(t, result, center + offset, radius);//获取所有需要遍历的面
		}

		for (int i = 0;i < result.size();i++)
		{
			FaceCloud* tCloud = result[i];
			result[i]->FaceJudge(NormalResult,FrictionResult, center + offset, radius);//获取接触面法向量
		}

		f = Nf;

		//Vector3d Tvn(0, 0, 0);

		vector<Vector3d> r;//存储速度分量

		/*if (NormalResult.size() != 0)
			Tvn.x = 1;*/
		//对边界线判定
		if (NormalResult.size() == 0 && !v.isZero() && Face::ChooseFace.size() != 0)
		{
			vector<Line*> AllLine;
			for (int i = 0;i < Face::ChooseFace.size();i++)
			{
				for (int j = i;j < Face::ChooseFace.size();j++)
				{
					if (i == j)continue;
					//vector<Vector3d> t;
					Line* t = Face::ChooseFace[i]->GetLine(Face::ChooseFace[j]);
					if (t != NULL)
						AllLine.push_back(t);
				}
			}
			Vector3d TouchPoint;
			Line* tLine = NULL;
			for (int i = 0;i < AllLine.size();i++)
			{
				tLine = AllLine[i];
				if (AllLine[i]->GetTouchPoint(TouchPoint, center + offset, radius))
					break;
			}
			if (tLine == NULL)
			{
				f = g * m;
				a = f / m;//牛二
				offset = v * stime + offset;//位移
				v = a * stime + v;//速度变化
				return;
			}
			Vector3d fDirection = TouchPoint - center - offset;
			fDirection = fDirection.UnitVector();
			if (tLine->Direction.GetAngel(fDirection)<0)
				f = (~fDirection) * (m * v.GetModule() / radius);
			else
				f = fDirection * (m * v.GetModule() / radius);
			a = f / m;//牛二
			offset = v * stime + offset;//位移
			v = a * stime + v;//速度变化

			printf("%f, %f, %f\n", offset.x, offset.y, offset.z);
			return;
		}

		for (int i = 0;i < NormalResult.size();i++)
		{
			if (v.isZero())break;
			Vector3d BackNormal = ~NormalResult[i];
			//float vec = (BackNormal) % v;
			float angle = BackNormal.GetAngel(v);
			Vector3d t = BackNormal.UnitVector()*(!v)*angle;
			Vector3d fri = v - t;
			fri = (~fri.UnitVector())*FrictionResult[i]*((!v)*m / stime);
			f += fri;
			t = t * 2 * 0.8;
			r.push_back(t);
			//v -= t;
			//f = ((~v)*M / stime)+f;//动量

		}

		for (int i = 0;i < r.size();i++)
			v -= r[i];

		Vector3d F = f;
		F = g * m + F;
		if (F.isZero()) a = Vector3d(0, 0, 0);
		else
			a = F / m;//牛二
		//if (v.x<0.0001&&v.x>-0.0001) v.x = 0;
		offset = v * stime + offset;//位移
		v = a * stime + v;//速度变化
		//if (v.x<0.0001&&v.x>-0.0001) v.x = 0;
		printf("%f, %f, %f\n", offset.x, offset.y, offset.z);

	}

	void Redraw()
	{
		glPushMatrix();
		//glRotatef(-90, 1, 0, 0);
		//glTranslatef(-center.x, -center.y, -center.z);
		Vector3d Trotate = v ^ Vector3d(0, 0, 1);
		if (v.x<0.0001&&v.x>-0.0001) v.x = 0.0;
		float t = !v * 0.3;
		if (t < MinRotate);
		else
			Rotate += t;
		if (Rotate > 360)Rotate -= 360;
		//glRotatef(90, 1, 0, 0);
		//glTranslatef(center.x, center.y, center.z);
		glTranslatef(offset.x, offset.y, offset.z);
		glTranslatef(center.x, center.y, center.z);
		glRotatef(Rotate , Trotate.x, Trotate.y, Trotate.z);
		glTranslatef(-center.x, -center.y, -center.z);
		objModel.ElementDraw();
		glPopMatrix();
	}

	Vector3d GetCenter() { return center + offset; }

	float GetRadius() { return radius; }

	void SetOffset(Vector3d n) { offset = n; }

	void Init() { Vector3d zero(0, 0, 0);a = zero;v = zero;f = zero; }

private:
	Vector3d center;
	Vector3d offset;

	float radius;
	float Rotate;

	Vector3d a;
	Vector3d v;
	Vector3d f;

	float m;

	Vector3d g;

	std::string Path = "sphere_init.obj";
	ObjLoader objModel;


};