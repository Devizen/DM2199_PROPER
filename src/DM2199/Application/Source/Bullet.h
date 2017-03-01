#ifndef BULLET_H
#define BULLET_H


#include "Object.h"

class Scene;

class Bullet : public Object
{
public:
	Bullet(Scene * scene ,Vector3 pos, float rotateYaw, float rotatePitch);
	Bullet(Scene * scene, Vector3 pos);
	~Bullet();

	virtual void interact();

private:
	Vector3 _defaultPosition;
	float _defaultYaw;
	float _defaultPitch;

	const float _maxDistance = 1000.f;
	const float bulletSpeed = 400.0f;

	bool mingxiue26rox = false;
	bool characterGotHit = false;


};


#endif
