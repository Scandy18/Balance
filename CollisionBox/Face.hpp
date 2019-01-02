#pragma once
#include"Vector3d.hpp"
#include"CollisionBox.hpp"
#include"vector"
#include"queue"

using std::vector;
using std::queue;

class Face
{
public:

	Vector3d NormalVector;

	Face(vector<Vector3d> points,vector<Vector3d> normals, float f)
	{
		size = points.size;
		FrictionModule = f;
		Vector3d ptemp(0, 0, 0);
		Vector3d ntemp(0, 0, 0);
		for (int i = 0;i < size;i++)
		{
			ptemp += points[i];
			ntemp += normals[i];
			Point.push_back(points[i]);
		}
		CenterPoint = ptemp / 4;
		NormalVector = ntemp / 4;
	}
	~Face(){}

	//judge the face is in the collisionbox or not
	bool CenterJudge(CollisionBox box)
	{
		box.InBox(CenterPoint);
	}
	//judge the face is in touch with the ball or not
	bool NormalJudge(Vector3d center, float radius)
	{
		return (CenterPoint - center)*(CenterPoint - center) <= radius * radius + 0.01;
	}
protected:
	
	vector<Vector3d> Point;
	Vector3d CenterPoint;

	float FrictionModule;
	int size;
};
