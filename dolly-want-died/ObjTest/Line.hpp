#pragma once
#include"Vector3d.hpp"
#include"vector"
#include"queue"

#define tolerate 0.1

using std::vector;
using std::queue;

class Line
{
public:
	Vector3d Direction;

	Line(){}
	Line(Vector3d a, Vector3d b, Vector3d d) { SidePoint.push_back(a);SidePoint.push_back(b); Direction = d; }

	bool GetTouchPoint(Vector3d &result,Vector3d center, float radius)
	{
		Vector3d line = SidePoint[0] - SidePoint[1];
		if (fabs(line.x) < 0.00000001 && fabs(line.x) < 0.00000001 && fabs(line.z) < 0.00000001)
		{
			result = SidePoint[0];
			return true;
		}

		Vector3d t = center - SidePoint[0];
		float u = t | line;

		u = u / line.GetModule();

		result = (line * u) + SidePoint[0];

		if((result.x>SidePoint[0].x&&result.x>SidePoint[1].x)||
			(result.x<SidePoint[0].x&&result.x<SidePoint[1].x))
				return false;
		if (result.GetModule() >= radius - 0.03&&result.GetModule() <= radius + 0.03)
			return true;
		return false;


		//Vector3d CenterPoint = (SidePoint[0] + SidePoint[1]) * 0.5;
		//Vector3d t = CenterPoint - center;
		//Vector3d s = SidePoint - center;
		//if(t.GetModule()<radius*radius+tolerate||)
	}
private:
	vector<Vector3d> SidePoint;
};
