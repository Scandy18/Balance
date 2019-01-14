#pragma once
#include"Vector3d.hpp"

class CollisionBox
{
public:
	CollisionBox(){}
	CollisionBox(Vector3d max,Vector3d min)
	{
		Center = (max+min)/2;
		Vector3d s = max-min;
		if(s.x>=s.y&&s.x>=s.z)
			Side=s.x;
		else if(s.y>=s.x&&s.y>=s.z)
			Side=s.y;
		else if(s.z>=s.x&&s.z>=s.y)
			Side=s.z;
	}
	CollisionBox(Vector3d c,float s){Center=c;Side=s;}
	~CollisionBox(){}

	bool CollisionJudge(Vector3d center, float radius)
	{
		Vector3d t = Center - center;
		float tsize = !t;
		if (radius >= tsize)return true;
		Vector3d near = t * ((tsize - radius) / tsize);
		Vector3d tnear = Center + near;
		if (tnear.x<Center.x - Side / 2 || tnear.x>Center.x + Side / 2 || tnear.y<Center.y - Side / 2 || tnear.y>Center.y + Side / 2 || tnear.z<Center.z - Side / 2 || tnear.z>Center.z + Side / 2)
			return false;
		/*else
		{
			Vector3d far = t * ((tsize + radius) / tsize);
			Vector3d tfar = Center + far;
			if (tfar.x<Center.x - Side / 2 || tfar.x>Center.x + Side / 2 || tfar.y<Center.y - Side / 2 || tfar.y>Center.y + Side / 2 || tfar.z<Center.z - Side / 2 || tfar.z>Center.z + Side / 2)
				return false;
		}*/
		return true;
	}

	bool InBox(Vector3d point)
	{
		if (point.x<Center.x - Side / 2 || point.x>Center.x + Side / 2 || point.y<Center.y - Side / 2 || point.y>Center.y + Side / 2 || point.z<Center.z - Side / 2 || point.z>Center.z + Side / 2)
			return false;
		return true;
	}

	float GetSide(){return Side;}
	Vector3d GetCenter(){return Center;}
private:
	float Side;
	Vector3d Center;
};
