#pragma once
#include"Face.hpp"


class TriangleFace : public Face
{
public:
	/*TriangleFace(Vector3d x, Vector3d y, Vector3d z, Vector3d mx, Vector3d my, Vector3d mz, float f)
	{
		size = 3;
		FrictionModule = f;
		CenterPoint = (x + y + z) / 3;s
		Point.push_back(x);Point.push_back(y);Point.push_back(z);
		NormalVector = (mx + my + mz) / 3;
		if ((mx - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (my + mz) / 2;
		}
		else if ((my - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (mx + mz) / 2;
		}
		else if ((mz - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (mx + my) / 2;
		}
	}*/
	TriangleFace(Vector3d x, Vector3d y, Vector3d z, Vector3d mx, Vector3d my, Vector3d mz, float f)
	{
		size = 3;
		FrictionModule = f;
		CenterPoint = (x + y + z) / 3;
		Point.push_back(x);Point.push_back(y);Point.push_back(z);
		NormalVector = (mx + my + mz) / 3;
	}
	~TriangleFace() {}

	bool CenterJudge(CollisionBox box) { box.InBox(CenterPoint); }

private:
	//int size;
};

