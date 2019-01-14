#pragma once
#include"cmath"
#include"string"

//作者：万能涛 转载时请注明转载信息 

class Vector3d
{
public:
	float x;
	float y;
	float z;

	Vector3d() { x = 0;y = 0;z = 0; }
	Vector3d(float px,float py,float pz) { x = px;y = py;z = pz; }
	//获得向量分量
	Vector3d ComponentX() { return Vector3d(x, 0, 0); }
	Vector3d ComponentY() { return Vector3d(0, y, 0); }
	Vector3d ComponentZ() { return Vector3d(0, 0, z); }

	float GetModule() { return sqrt(x * x + y * y + z * z); }

	Vector3d UnitVector() { Vector3d  n(x, y, z); return n / !n; }
	
	Vector3d operator + (Vector3d &d) { Vector3d  n(x, y, z);n.x = x + d.x;n.y = y + d.y;n.z = z + d.z;return n; }
	void operator += (Vector3d &d) { x = x + d.x;y = y + d.y;z = z + d.z; }
	Vector3d operator - (Vector3d &d) { Vector3d  n(x, y, z);n.x = x - d.x;n.y = y - d.y;n.z = z - d.z;return n; }
	void operator -= (Vector3d &d) { x = x - d.x;y = y - d.y;z = z - d.z; }
	Vector3d operator * (float t) { Vector3d  n(t*x, t*y, t*z);return n; }
	void operator *= (float t) { x = x * t;y = y * t;z = z * t; }
	Vector3d operator / (float t) { Vector3d  n(x/t, y/t, z/t);return n; }
	void operator /= (float t) { x = x / t;y = y / t;z = z / t; }
	//取模 !vector
	float operator ! () { float t = GetModule();return t; }
	//点积（找不到点怎么打）v1|v2
	float operator | (Vector3d &d) { return x * d.x + y * d.y + z * d.z; }
	//求夹角 %
	float operator % (Vector3d &d) { Vector3d  n(x, y, z); return acos((n | d) / (! n)*(! d)); }
	//叉积 ^ 求法线
	Vector3d operator ^ (Vector3d &d) { Vector3d  n(y*d.z - z * d.y, z*d.x - x * d.z, x*d.y - y * d.x);return n; }
	//取反 前缀单目~
	Vector3d operator ~ () { Vector3d n(-x, -y, -z);return n; }

	~Vector3d(){}
};

