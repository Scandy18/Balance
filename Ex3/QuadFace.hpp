#pragma once
#include"Face.hpp"


class QuadFace : public Face
{
public:
	/*QuadFace(Vector3d x, Vector3d y, Vector3d z, Vector3d r, Vector3d mx, Vector3d my, Vector3d mz, Vector3d mr, float f)
	{
		size = 4;
		FrictionModule = f;
		CenterPoint = (x + y + z + r) / 4;
		Point.push_back(x);Point.push_back(y);Point.push_back(z);Point.push_back(r);
			NormalVector = (mx + my + mz + mr) / 4;
		if ((mx - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (mr + my + mz) / 3;
			if ((mr - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (my + mz) / 2;
			}
			else if ((my - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mr + mz) / 2;
			}
			else if ((mz - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mr + my) / 2;
			}
		}
		else if ((my - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (mx + mr + mz) / 3;
			if ((mx - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mr + mz) / 2;
			}
			else if ((mr - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mx + mz) / 2;
			}
			else if ((mz - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mx + mr) / 2;
			}
		}
		else if ((mz - NormalVector).Sum() < Vector_Tolerate)
		{
			NormalVector = (mx + my + mr) / 3;
			if ((mx - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (my + mr) / 2;
			}
			else if ((my - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mx + mr) / 2;
			}
			else if ((mr - NormalVector).Sum() < Vector_Tolerate)
			{
				NormalVector = (mx + my) / 2;
			}
		}
	}*/
	QuadFace(Vector3d x, Vector3d y, Vector3d z, Vector3d r, Vector3d mx, Vector3d my, Vector3d mz, Vector3d mr, float f)
	{
		size = 4;
		FrictionModule = f;
		CenterPoint = (x + y + z + r) / 4;
		Point.push_back(x);Point.push_back(y);Point.push_back(z);Point.push_back(r);
		NormalVector = (mx + my + mz + mr) / 4;
	}
	~QuadFace() {}

	
	bool CenterJudge(CollisionBox box)
	{
		box.InBox(CenterPoint);
	}


private:
	//int size;
};