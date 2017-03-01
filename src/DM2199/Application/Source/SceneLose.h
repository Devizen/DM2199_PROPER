#ifndef SCENELOSE_H
#define SCENELOSE_H

#include "Scene.h"
#include "Camera.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "FactoryObject.h"
#include "Enemy.h"
#include "Sound.h"

//Randomize
#include <stdlib.h>
#include <time.h>

class SceneLose : public Scene
{
    friend class Tutorial;
    friend class Level1;
    friend class Level2;
    friend class Level3;
    friend class Level4;
    friend class Boss;

	enum GEOMETRY_TYPE
	{
		//Main Menu
		GEO_MAINMENU = 0,
		LEVELSELECT,

		//Arrow
		GEO_ARROW,

		GEO_LOSESCREEN,
		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,

		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,

		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_LIGHT2_POSITION,
		U_LIGHT2_COLOR,
		U_LIGHT2_POWER,
		U_LIGHT2_KC,
		U_LIGHT2_KL,
		U_LIGHT2_KQ,

		U_LIGHT2_TYPE,
		U_LIGHT2_SPOTDIRECTION,
		U_LIGHT2_COSCUTOFF,
		U_LIGHT2_COSINNER,
		U_LIGHT2_EXPONENT,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		//Text
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	enum MENU
	{
		//First Page of Main Menu
		RETRY, 
		RETURN,
		QUIT,

	};

	unsigned m_parameters[U_TOTAL];

public:
	SceneLose();
	~SceneLose();

	void RenderMesh(Mesh *mesh, bool enableLight);

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	//Sound
	std::vector<Sound *> soundStorage;

	float _dt;
	float _elapsedTime = 0;

	int choice = 0;
private:
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;

	Camera3 camera;

	MS modelStack, viewStack, projectionStack;

	//Normal
	Light light[3];

	//Text
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, int x, int y, int sizex, int sizey);

	//Options
	float pressDelay = 0.f;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.2f;

    //Update the levels for retrying
    static void setRetry(unsigned input);
};



#endif