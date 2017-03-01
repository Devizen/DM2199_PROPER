#include "Object.h"
//#include "Levels/Tutorial.h"
//#include "Levels/Level1.h"
//#include "Levels/Level2.h"
//#include "Levels/Level3.h"
//#include "Levels/Level4.h"
//#include "Levels/Boss.h"
#include "Scene.h"



void Object::objectRender()
{
	_scene->modelStack.PushMatrix();
	_scene->modelStack.Translate(position_.x, position_.y, position_.z);
	_scene->modelStack.Rotate(rotateX, 1, 0, 0);
	_scene->modelStack.Rotate(rotateZ, 0, 0, 1);
	_scene->modelStack.Rotate(rotateY, 0, 1, 0);
	_scene->modelStack.Scale(scale, scale, scale);
	_scene->RenderMesh(_scene->meshList[type], true);
	_scene->modelStack.PopMatrix();

}
