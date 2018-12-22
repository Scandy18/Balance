#include <stdlib.h>
#include "Vector3d.hpp"
#include "glut.h"

#define PI 3.1415926

class Camera
{
private:
public:
	//Vector3d target;//���������
	Vector3d position;//�����λ��
	Vector3d rotation;//��������� ��������ϵ ��2���Ƕ�(������)��1������(ʼ��Ϊ1)ȷ��
	
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

	/* ����ĳ��Ŀ�� */
	void lockObject(Vector3d target)
	{
		Vector3d rot = EulertoRotation(target - position);
		rot.z = 1;
		rotation = rot;
	}

	void CameraMove(Vector3d offset)
	{
		position += offset;
	}

	/* ����ĳ������ */
	void CameraLookat(Vector3d target)
	{
		/* �����ӽ� */
		rotation = EulertoRotation(target - position);
		rotation.z = 1;//��λ��

		/* lookat target */
		gluLookAt(position.x, position.y, position.z,
			target.x, target.y, target.z,
			0, 1, 0);
	}

	/* ����ǰ���� */
	void CameraLookat()
	{
		Vector3d eu = RotationtoEuler(rotation);
		gluLookAt(position.x, position.y, position.z,
			position.x + eu.x, position.y + eu.y, position.z + eu.z,
			0, 1, 0);
	}

	/* ֱ������ϵת��������ϵ */
	Vector3d EulertoRotation(Vector3d euler)
	{
		float r = sqrtf(euler.x * euler.x + euler.y * euler.y + euler.z * euler.z);
		float cita = acosf(euler.z / r);
		float fi = atanf(euler.y / euler.x);
		return Vector3d(cita, fi, 1);
	}

	/* ��������ϵתֱ������ϵ */
	Vector3d RotationtoEuler(Vector3d rot)
	{
		float ex = rot.z * sinf(rot.x) * cosf(rot.y);
		float ey = rot.z * sinf(rot.x) * sinf(rot.y);
		float ez = rot.z * cosf(rot.x);
		return Vector3d(ex, ey, ez);
	}



};