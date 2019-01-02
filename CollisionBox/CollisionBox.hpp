#pragma once
#include"Vector3d.hpp"

class CollisionBox
{
public:
	CollisionBox(){}
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

private:
	float Side;
	Vector3d Center;
};
