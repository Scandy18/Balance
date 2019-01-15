#pragma once
#include"Vector3d.hpp"
#include"CollisionBox.hpp"
#include"vector"
#include"queue"

#define Tolerate 0.4

using std::vector;
using std::queue;

class Face
{
public:

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
		float t = (CenterPoint - center).GetModule();
		return (t <= radius * radius + Tolerate) && (t >= radius * radius - Tolerate);
	}
	
	float GetFrictionModule(){return FrictionModule;}
	
	Vector3d GetNormalVector(){return NormalVector;}
protected:
	
	vector<Vector3d> Point;
	Vector3d CenterPoint;
	
	Vector3d NormalVector;

	float FrictionModule;
	int size;
};
