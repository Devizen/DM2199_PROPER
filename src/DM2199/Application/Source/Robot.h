#pragma once 

#include "Enemy.h"
#include "Scene.h"

class Robot : public Enemy
{
public:
	enum robotState
	{
		patrolling,
		chasing,
		death,
		escaping,
		attack,
	};


	Robot(float, float, float, float, int typeRobot );
	~Robot();

	virtual void addWaypoint(Vector3);
	virtual void movetoWaypoint(double dt);

	//Setters and Getters for movement speed
	void setMovementSpeed(float);
	float getMovementSpeed();

	//Setters and Getters for damage
	void setDamage(float);
	float getDamage();

	//Setter and Getters for hp
	void setHp(float);
	float  getHp();

	virtual void setRange(float);
	virtual float getRange();

	virtual void setState(int);
	virtual int getState();

	void update();

	bool shoot = false;

	bool inRange = false;

	//Get DT
	static void dtFromScene(double dt);
	static void positionFromCamera(Camera3 pos);

	//Jumping
	bool jump = false;
	bool fall = false;
	float jumpHeight = 20;

private:
	robotState _State;
	Tutorial * _scene;
    SceneEditor* levelEditor;
};
