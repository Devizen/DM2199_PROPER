#include "Bullet.h"
#include "Scene.h"
#include "Levels/Tutorial.h"

Bullet::Bullet(Scene* scene, Vector3 pos, float rotateYaw, float rotatePitch) : Object(scene, pos)
{

	type = Tutorial::GEO_LIGHTBALL3;
	_defaultYaw = _scene->camera.getYaw();
	_defaultPitch = _scene->camera.getPitch();
	_defaultPosition = pos;
	scale = 5;
	rotateY = -rotateYaw;
	rotateZ = rotatePitch;

	mingxiue26rox = false;
}
Bullet::Bullet(Scene* scene, Vector3 pos) :Object(scene, pos)
{
	_defaultPosition = pos;
	type = Tutorial::GEO_LIGHTBALL2;
	scale = 5;

	mingxiue26rox = true;
}

Bullet::~Bullet()
{
}

void Bullet::interact(){

	//Player's bullet movement
	if (mingxiue26rox == false)
	{
		position_.x += (float)(bulletSpeed * cos(Math::DegreeToRadian(_defaultYaw))* _scene->_dt);
		position_.y += (float)(bulletSpeed * tan(Math::DegreeToRadian(_defaultPitch)) * _scene->_dt);
		position_.z += (float)(bulletSpeed* sin(Math::DegreeToRadian(_defaultYaw))* _scene->_dt);

	}

	//Robot's bullet movement
	if (mingxiue26rox == true)
	{
		Vector3 distance = (_defaultPosition - _scene->camera.getPosition());
		Vector3 unitDistance = distance.Normalized();
		float moveX = unitDistance.x * bulletSpeed* _scene->_dt;
		float moveZ = unitDistance.z * bulletSpeed* _scene->_dt;

		// Rotate the enemy towards the player
		rotateY = -Math::RadianToDegree(atan2(distance.z, distance.x));

		// Move the Enemy
		position_.x -= moveX;
		position_.z -= moveZ;
	}

	//Delete bullet when character's bullet hits enemy or when  it went too far
	if (mingxiue26rox == false)
	{
		for (vector<Enemy*>::iterator itEnemy = _scene->enemyStorage.begin(); itEnemy != _scene->enemyStorage.end(); itEnemy++)
		{
			if ((position_ - (*itEnemy)->_Position).Length() < 20 || (_defaultPosition - position_).Length() >= _maxDistance)
			{
				_scene->objFactory.destroyFactoryObject(this);
				return;
			}

		}
	}

	//Delete bullet when enemy's bullet travels too far
	if (mingxiue26rox == true)
	{
		for (vector<Enemy*>::iterator itEnemy = _scene->enemyStorage.begin(); itEnemy != _scene->enemyStorage.end(); itEnemy++)
		{
			if ((_defaultPosition - position_).Length() >= 50)
			{
				_scene->objFactory.destroyFactoryObject(this);
				return;
			}
		}
	}
}



