#ifndef OBJECT_H
#define OBJECT_H

#include "Vector3.h"
#include "Camera3.h"
#include <vector>


class Scene;

class Object
{
public:
	Object(Scene * scene, Vector3 pos) : _scene(scene), position_(pos){};
	Object(Vector3 pos) : position_(pos){};
	virtual ~Object(){};

	virtual void interact() = 0; // update
	virtual void objectRender();

	/*Camera3 camera;*/

	Vector3 position_;
	float scale = 1;
	float rotateX = 0;
	float rotateY = 0;
	float rotateZ = 0;

	float sizeX = 1;
	float sizeY = 1;
	float translateX = 0;
	float translateY = 0;

	unsigned type = 0;


protected:
	Scene *_scene;

	
};
#endif
