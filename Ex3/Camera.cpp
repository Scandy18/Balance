#include <stdlib.h>
#include "Vector3d.hpp"
#include "glut.h"

#define PI 3.1415926

class Camera
{
private:
public:
	//Vector3d target;//摄像机朝向
	Vector3d position;//摄像机位置
	Vector3d rotation;//摄像机朝向 球面坐标系 由2个角度(弧度制)和1个距离(始终为1)确定
	
	Camera()
	{
		Vector3d position = Vector3d(0, 2, 8);
		Vector3d rotation = Vector3d(PI, -PI / 4, 1);
	}
	Camera(Vector3d pos, Vector3d rot)
	{
		position = pos;
		rotation = rot;
	}
	~Camera()
	{

	}

	void setCameraPosition(Vector3d pos)
	{
		position = pos;
	}

	void setCameraRotation(Vector3d rot)
	{
		rotation = rot;
	}

	/* 通过目标获得朝向 */
	void getTarget(Vector3d target)
	{
		Vector3d rot = EulertoRotation(target - position);
		rot.z = 1;
		rotation = rot;
	}

	/* 相机位置移动 */
	void CameraMove(Vector3d offset)
	{
		position += offset;
	}

	/* 相机旋转 */
	void CameraRotate(Vector3d offset)
	{
		rotation += offset;
		rotation.z = 1;
	}

	/* 看向某个坐标 */
	void CameraLookat(Vector3d target)
	{
		/* 更新视角 */
		rotation = EulertoRotation(target - position);
		rotation.z = 1;//单位化

		/* lookat target */
		gluLookAt(position.x, position.y, position.z,
			target.x, target.y, target.z,
			0, 0, 1);
	}

	/* 看向当前方向 */
	void CameraLookat()
	{
		Vector3d eu = RotationtoEuler(rotation);
		gluLookAt(position.x, position.y, position.z,
			position.x + eu.x, position.y + eu.y, position.z + eu.z,
			0, 0, 1);
	}

	/* 直角坐标系转球面坐标系 */
	Vector3d EulertoRotation(Vector3d euler)
	{
		float r = sqrtf(euler.x * euler.x + euler.y * euler.y + euler.z * euler.z);
		float cita = acosf(euler.z / r);
		float fi = atanf(euler.y / euler.x);
		return Vector3d(cita, fi, 1);
	}

	/* 球面坐标系转直角坐标系 */
	Vector3d RotationtoEuler(Vector3d rot)
	{
		float ex = rot.z * sinf(rot.x) * cosf(rot.y);
		float ey = rot.z * sinf(rot.x) * sinf(rot.y);
		float ez = rot.z * cosf(rot.x);
		return Vector3d(ex, ey, ez);
	}



};