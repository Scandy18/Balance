#pragma once
#include"Vector3d.hpp"
#include"FaceCloud.hpp"
#include"sstream"
#include"fstream"
#include"iostream"
//#include"gl/glew.h"
//#include"gl/gl.h"
//#include"gl/glu.h"
#include "gl/glut.h"
#include"algorithm"
#include"string"
#include"vector"
//#pragma comment(lib, "glew32.lib")

#define MaxVertex 8000
#define MaxFace 3000


/*obj文件可以用记事本打开查看内容 v开头是点坐标 vt开头是纹理的uv坐标 vn开头是顶点法向量 
f开头是面 f 1/1/1 2/1/1 3/1/1 4/1/1 表示这个面由四个点构成 每个点的信息：坐标索引/纹理索引/法向量索引 
索引是从1开始而不是0开始
*/

class ObjLoader {
public:
	ObjLoader(){}
	//ObjLoader(std::string filename) { ObjLoader(filename,0.05); }
	ObjLoader(std::string filename, float friction)
	{
		Nface_3 = 0;
		Nface_4 = 0;
		Nvertices = 0;
		std::ifstream file(filename);
		std::string line;
		while (getline(file, line))
		{
			//顶点纹理坐标
			if (line.substr(0, 2) == "vt")
			{
				GLfloat x, y, z;
				std::istringstream s(line.substr(2));
				s >> x; s >> y;// s >> z;
				Vector3d n(x, y, 0);
				vtSets.push_back(n);
			}
			//顶点法向量
			else if (line.substr(0, 2) == "vn")
			{
				GLfloat x, y, z;
				std::istringstream s(line.substr(2));
				s >> x; s >> y; s >> z;
				Vector3d n(x, y, z);
				vnSets.push_back(n);
			}
			//顶点位置
			else if (line.substr(0, 1) == "v")
			{
				GLfloat x, y, z;
				std::istringstream s(line.substr(2));
				s >> x; s >> y; s >> z;
				Vector3d n(x, y, z);
				vSets.push_back(n);
				Vertices[Nvertices++] = x;
				Vertices[Nvertices++] = y;
				Vertices[Nvertices++] = z;
			}
			//面信息
			else if (line.substr(0, 1) == "f")
			{
				std::vector<int> temp;
				std::vector<std::vector<int>> vPointMess;
				std::string l = line.substr(2);
				replace(l.begin(), l.end(), '/', ' ');//因为istringstream只能以空格为分隔符，所以/转化为空格
				int u, v, w;
				
				std::istringstream vtns(l);

				std::vector<Vector3d> FacePoint;
				std::vector<Vector3d> FaceNormal;

				if (vtSets.size() == 0)
				{
					while (vtns >> u)
					{
						//vtns >> v; 
						vtns >> w;
						std::vector<int> vIndexSets;
						vIndexSets.push_back(u - 1);
						//vIndexSets.push_back(v - 1);
						vIndexSets.push_back(w - 1);
						vPointMess.push_back(vIndexSets);
						temp.push_back(u - 1);

						//Texture[2 * u - 2] = vtSets[v - 1].x;
						//Texture[2 * u - 1] = vtSets[v - 1].y;//由于drawelements只看索引 因此需要建立起和索引对应的坐标，法向量，纹理

						Normal[3 * u - 3] = vnSets[w - 1].x;
						Normal[3 * u - 2] = vnSets[w - 1].y;
						Normal[3 * u - 1] = vnSets[w - 1].z;

						FacePoint.push_back(vSets[u - 1]);
						if (vSets[u - 1].x > FaceCloud::MaxFaceCloud->MaxPosition.x)
							FaceCloud::MaxFaceCloud->MaxPosition.x = vSets[u - 1].x;
						if (vSets[u - 1].x < FaceCloud::MaxFaceCloud->MinPosition.x)
							FaceCloud::MaxFaceCloud->MinPosition.x = vSets[u - 1].x;
						if (vSets[u - 1].y > FaceCloud::MaxFaceCloud->MaxPosition.y)
							FaceCloud::MaxFaceCloud->MaxPosition.y = vSets[u - 1].y;
						if (vSets[u - 1].y < FaceCloud::MaxFaceCloud->MinPosition.y)
							FaceCloud::MaxFaceCloud->MinPosition.y = vSets[u - 1].y;
						if (vSets[u - 1].z > FaceCloud::MaxFaceCloud->MaxPosition.z)
							FaceCloud::MaxFaceCloud->MaxPosition.z = vSets[u - 1].z;
						if (vSets[u - 1].z < FaceCloud::MaxFaceCloud->MinPosition.z)
							FaceCloud::MaxFaceCloud->MinPosition.z = vSets[u - 1].z;
						FaceNormal.push_back(vnSets[w - 1]);
					}
				}
				else
				{
					while (vtns >> u)
					{
						vtns >> v; 
						vtns >> w;
						std::vector<int> vIndexSets;
						vIndexSets.push_back(u - 1);
						vIndexSets.push_back(v - 1);
						vIndexSets.push_back(w - 1);
						vPointMess.push_back(vIndexSets);
						temp.push_back(u - 1);

						Texture[2 * u - 2] = vtSets[v - 1].x;
						Texture[2 * u - 1] = vtSets[v - 1].y;//由于drawelements只看索引 因此需要建立起和索引对应的坐标，法向量，纹理

						Normal[3 * u - 3] = vnSets[w - 1].x;
						Normal[3 * u - 2] = vnSets[w - 1].y;
						Normal[3 * u - 1] = vnSets[w - 1].z;

						FacePoint.push_back(vSets[u - 1]);
						FaceNormal.push_back(vnSets[w - 1]);
					}
				}
				fSets.push_back(vPointMess);
				if (temp.size() == 4)
					for (int j = 0;j < 4;j++)
						Vindex_4[Nface_4++] = temp[j];
				if (temp.size() == 3)
					for (int j = 0;j < 3;j++)
						Vindex_3[Nface_3++] = temp[j];

				Face* t = new Face(FacePoint, FaceNormal, friction);
				FaceCloud::MaxFaceCloud->AddtoFaceinCloud(t);
			}
			//注释信息
			else if (line.substr(0, 1) == "#");
		}
		file.close();
	}
	void ElementDraw()
	{
		glEnableClientState(GL_VERTEX_ARRAY);//enable 顶点数组
		glEnableClientState(GL_NORMAL_ARRAY);//enable 法向量数组
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);//enable 纹理数组

		glVertexPointer(3, GL_FLOAT, 0, Vertices);//定义顶点信息映射到Vertices，一个坐标3个参数
		glNormalPointer(GL_FLOAT, 0, Normal);//定义法向量信息映射到Normal，一个法向量默认3个参数
		//glTexCoordPointer(2, GL_FLOAT, 0, Texture);//定义纹理信息映射到Texture（这里不知道为什么加载不出）

		glDrawElements(GL_TRIANGLES, Nface_3, GL_UNSIGNED_INT, Vindex_3);//画三角形mesh网格

		glDrawElements(GL_QUADS, Nface_4, GL_UNSIGNED_INT, Vindex_4);//画四边形mesh网格
	}
	//低帧 过时
	void Draw()
	{
		for (int i = 0; i < fSets.size(); i++) 
		{
			Vector3d Vertex[4];//存储不超过4个顶点的位置信息
			Vector3d Texture[4];//存储每个顶点的纹理信息
			Vector3d NorVector[4];//存储每个顶点法向量信息

			GLubyte Index[4];
			GLfloat Vers[12];

			Vector3d Vn;//累加平均计算面的法向量

			for (int j = 0;j < fSets[i].size();j++)
			{
				Index[j] = j;
				Vertex[j] = vSets[fSets[i][j][0]];//存取顶点信息
				Vers[3*j] = Vertex[j].x;
				Vers[3 * j + 1] = Vertex[j].y;
				Vers[3 * j + 2] = Vertex[j].z;
				Texture[j] = vtSets[fSets[i][j][1]];//存取纹理信息
				NorVector[j] = vnSets[fSets[i][j][2]];//存取法向量信息
				Vn += NorVector[j];
			}
			Vn = Vn.UnitVector();//得到法向量单位向量

			glNormal3f(Vn.x, Vn.y, Vn.z);//绘制法向量

			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(3, GL_FLOAT, 0, Vers);

			glDrawElements(GL_LINE_LOOP, fSets[i].size(), GL_UNSIGNED_BYTE, Index);
		}
	}
	//VBO 存在bug 但是更快 帧数更高
	void vboInit()
	{
		/*glGenBuffers(1, &vertexBufferObject);

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, Nvertices * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 0, (GLvoid*)0);
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &indexBufferObject_3);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_3);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Nface_3 * sizeof(GLuint), Vindex_3, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &indexBufferObject_4);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_4);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Nface_4 * sizeof(GLuint), Vindex_4, GL_STATIC_DRAW);
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	}

	void vboDraw()
	{
		/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_3);
		glDrawElements(GL_TRIANGLES, Nface_3, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_4);
		glDrawElements(GL_QUADS, Nface_4, GL_UNSIGNED_BYTE, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
	}
private:
	std::vector<Vector3d>vSets;//顶点位置
	std::vector<Vector3d>vtSets;//顶点纹理
	std::vector<Vector3d>vnSets;//顶点法向量
	std::vector<std::vector<std::vector<int>>>fSets;//内测为3个索引集合 一共有3个或4个点构成mesh网格 这些点的集合

	GLfloat Vertices[3*MaxVertex];
	int Nvertices;
	GLuint Vindex_4[4*MaxVertex];
	int Nface_4;
	GLuint Vindex_3[3*MaxVertex];
	int Nface_3;

	GLfloat Normal[3*MaxVertex];
	GLfloat Texture[2*MaxVertex];

	//GLuint vertexBufferObject;
	//GLuint indexBufferObject_3;
	//GLuint indexBufferObject_4;

};
