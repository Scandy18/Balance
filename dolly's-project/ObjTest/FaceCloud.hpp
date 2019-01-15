#pragma once
#include"Face.hpp"

class FaceCloud
{
public:

	//static vector<Face*> AllFace;
	static FaceCloud* MaxFaceCloud;
	Vector3d MaxPosition;
	Vector3d MinPosition;

	CollisionBox getCollisionBox() { return box; }
	bool getBaseLayer() { return isBaseLayer; }

	FaceCloud(){MaxPosition=Vector3d(0,0,0);MinPosition=Vector3d(1000,1000,1000);isBaseLayer=false;}
	FaceCloud(bool l, CollisionBox b) { isBaseLayer = l;box = b; MaxPosition=Vector3d(0,0,0);MinPosition=Vector3d(1000,1000,1000);}
	~FaceCloud()
	{
		for (int i = 0;i < LastCloud.size();i++)
		{
			free(LastCloud[i]);
		}
	}

	//sort and block the facecloud inside
	void Insort(float radius)
	{
		box = CollisionBox(MaxPosition,MinPosition);
		/*while(box.GetSide()/4.0>=radius)
		{
			float s = box.GetSide()/2;
			
		}*/
		int NumCloud = box.GetSide()/(2*radius);
		float NewSide = box.GetSide()/(1.0*NumCloud);
		float ParentSide = box.GetSide();
		if(NumCloud==1)return;
		for(int i=0;i<NumCloud;i++)
		{
			for(int j=0;j<NumCloud;j++)
			{
				for(int k=0;k<NumCloud;k++)
				{
					Vector3d NewCenter(box.GetCenter().x - ParentSide / 2.0 + NewSide / 2.0 + i * NewSide, box.GetCenter().y - ParentSide / 2.0 + NewSide / 2.0 + j * NewSide, box.GetCenter().z - ParentSide / 2.0 + NewSide / 2.0 + k * NewSide);
					CollisionBox t(NewCenter,NewSide);
					FaceCloud* NewCloud = new FaceCloud(true,t);
					vector<Face*>::iterator it;
					for (it = FaceinCloud.begin();it != FaceinCloud.end();)
					{
						if ((*it)->CenterJudge(t))
						{
							NewCloud->AddtoFaceinCloud(*it);
							it = FaceinCloud.erase(it);
						}
						else
							it++;
					}
					/*for(int m=0;m<FaceinCloud.size();m++)
					{
						if(FaceinCloud[m]->CenterJudge(t))
							NewCloud->AddtoFaceinCloud(FaceinCloud[m]);
					}*/
					LastCloud.push_back(NewCloud);
				}
			}
		}
	}
	//traverse the face in the cloud and get the face in touch
	void FaceJudge(vector<Vector3d> &t, Vector3d center, float radius)
	{
		for (int i = 0;i < FaceinCloud.size();i++)
			if (FaceinCloud[i]->NormalJudge(center, radius))
				t.push_back(FaceinCloud[i]->GetNormalVector());
	}
	//get the collision facecloud in queue,and the smaller faceclouds in result
	void FaceCloudJudge(queue<FaceCloud*> &t,vector<FaceCloud*> &result, Vector3d center, float radius)
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
	
	void AddtoFaceinCloud(Face* t)
	{
		FaceinCloud.push_back(t);
	}

	void RemoveFaceinCloud(vector<Face*>::iterator t)
	{
		FaceinCloud.erase(t);
	}

	void ClearFaceinCloud()
	{
		FaceinCloud.clear();
	}

	//vector<FaceCloud*> LastCloud;
	//vector<Face*> FaceinCloud;

private:
	CollisionBox box;
	bool isBaseLayer;
	vector<FaceCloud*> LastCloud;
	vector<Face*> FaceinCloud;
	
};

FaceCloud* FaceCloud::MaxFaceCloud = new FaceCloud();