#pragma once
#include"Face.hpp"

class FaceCloud
{
public:

	static vector<Face*> AllFace;

	CollisionBox getCollisionBox() { return box; }
	bool getBaseLayer() { return isBaseLayer; }

	FaceCloud(){}
	FaceCloud(bool l, CollisionBox b) { isBaseLayer = l;box = b; }
	~FaceCloud()
	{
		for (int i = 0;i < LastCloud.size();i++)
		{
			free(LastCloud[i]);
		}
	}

	//sort and block the facecloud inside
	void Insort(){}
	//traverse the face in the cloud and get the face in touch
	void FaceJudge(vector<Vector3d> t, Vector3d center, float radius)
	{
		for (int i = 0;i < FaceinCloud.size();i++)
			if (FaceinCloud[i]->NormalJudge(center, radius))
				t.push_back(FaceinCloud[i]->NormalVector);
	}
	//get the collision facecloud in queue,and the smaller faceclouds in result
	void FaceCloudJudge(queue<FaceCloud*> t,vector<FaceCloud*>result, Vector3d center, float radius)
	{
		if (isBaseLayer)
		{
			if (box.CollisionJudge(center, radius))
				result.push_back(this);
			return;
		}
		else
		{
			if (box.CollisionJudge(center, radius))
				for (int i = 0;i < LastCloud.size();i++)
					t.push(LastCloud[i]);
			return;
		}
	}

private:
	CollisionBox box;
	bool isBaseLayer;
	vector<Face*> FaceinCloud;
	vector<FaceCloud*> LastCloud;
};
