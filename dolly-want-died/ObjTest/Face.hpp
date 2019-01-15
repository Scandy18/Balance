#pragma once
#include"Vector3d.hpp"
#include"Line.hpp"
#include"CollisionBox.hpp"
#include"vector"
#include"queue"

#define Tolerate 0.001
#define MaxTolerate 2.0


#define Pi 3.1415926
#define AngleTolerate 0.1




using std::vector;
using std::queue;

class Face
{
public:

	static vector<Face*> ChooseFace;

	Face(vector<Vector3d> points,vector<Vector3d> normals, float f)
	{
		size = points.size();
		FrictionModule = f;
		Vector3d ptemp(0, 0, 0);
		Vector3d ntemp(0, 0, 0);
		for (int i = 0;i < size;i++)
		{
			ptemp += points[i];
			ntemp += normals[i];
			Point.push_back(points[i]);
		}
		CenterPoint = ptemp / size;
		NormalVector = ntemp / size;
	}
	~Face(){}

	//judge the face is in the collisionbox or not
	bool CenterJudge(CollisionBox box)
	{
		return box.InBox(CenterPoint);
	}
	//judge the face is in touch with the ball or not
	bool NormalJudge(Vector3d center, float radius)
	{
		float nt = (CenterPoint - center).GetModule();
		if (nt <= radius * radius + MaxTolerate && (nt >= radius * radius))
		{
			Vector3d t = center - CenterPoint;
			float AngleCos = t.GetAngel(NormalVector);
			if (AngleCos <= 0) return false;
			float distance = t.GetModule()*AngleCos*AngleCos;
			if (distance <= radius * radius + Tolerate && (distance >= radius * radius - Tolerate))
			{
				ChooseFace.push_back(this);
				Vector3d temp = NormalVector.UnitVector()*sqrt(distance);
				return isPointIn(center - temp);
			}
			return false;
		}
		return false;
		//
	}

	bool isPointIn(Vector3d point)
	{
		
		vector<Vector3d> all;
		float angle = 0;
		for (int i = 0;i < size;i++)
		{
			if (point.x == Point[i].x&&point.y == Point[i].y&&point.z == Point[i].z)
				return true;
			all.push_back(Point[i] - point);
		}

		for (int i = 0;i < size;i++)
		{
			float a = all[i] % (all[(i + 1) % size]);
			angle += a;
		}
		if (angle >= 2 * Pi - AngleTolerate && angle <= 2 * Pi + AngleTolerate)
			return true;
		return false;
	}

	Line* GetLine(Face* t)
	{
		vector<Vector3d> l;
		for (int i = 0;i < size;i++)
		{
			for (int j = 0;j < t->GetSize();j++)
			{
				if (Point[i].x == t->GetPoint(j).x&&Point[i].y == t->GetPoint(j).y&&Point[i].z == t->GetPoint(j).z)
					l.push_back(Point[i]);
			}
		}
		if (l.size() != 2)
			return NULL;
		return new Line(l[0], l[1], (t->GetNormalVector() + NormalVector) / 2);
	}
	
	float GetFrictionModule(){return FrictionModule;}
	
	Vector3d GetNormalVector(){return NormalVector;}

	int GetSize() { return size; }

	Vector3d GetPoint(int i) { return Point[i]; }
protected:
	
	vector<Vector3d> Point;
	Vector3d CenterPoint;
	
	Vector3d NormalVector;

	float FrictionModule;
	int size;
};
vector<Face*> Face::ChooseFace;