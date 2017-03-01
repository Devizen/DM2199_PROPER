#ifndef BOSS_H
#define BOSS_H

#include "../Scene.h"
#include "../SceneMainMenu.h"
#include "../Light.h"

class Boss : public Scene
{
	friend class Sound;
	friend class Camera3;
	friend class SceneMainMenu;

public:
	Boss();
	~Boss();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void renderText();	//Text
	void renderGround();
	static int Loaded(); //Loaded check
	void enemyUpdate(double dt);	 //Enemy

	float rotateAngle;
	float FPS;


	//Day/Night Cycle
	bool cycle = false;
	bool night = false;
	unsigned cycleState = 0;
	void cycleCheck();
	void renderLightSwitch();

	//Torch
	bool torchDead = false;

	//Sprites
	void renderSprites();
	bool swing = false;

	//Potion
	int potionCount = 3;
	int usedPotionTime = 0;
	bool potionUsed = false;
	bool noPotion = false;
	bool notHurt = false;
	bool usedPotionOnce = false;
	bool usedPotionText = false;

	//Antidote
	int antidoteCount = 0;
	int usedAntidoteTime = 0;
	bool antidoteUsed = false;
	bool noAntidote = false;
	bool notPoisoned = false;
	bool usedAntidoteOnce = false;
	bool usedAntidoteText = false;

	//Electric field
	void renderElectricField();

	//Reset All Settings
	void resetAll();

	bool inventoryOpen;
	float startTime;
	float cooldown;

	//Enemy Robot Arm rotation
	float rotateArm = 0.f;
	float rotateIndividualArm = 0.f;
	float rotateLeftArm = 0.f;
	bool robotShoot = false;
	bool shootArmDown = true;
	bool broughtDown = true;

	float scaleDamage = 0.2f;

	bool bulletTouch = false;
	bool damaged = false; // character to enemy



private:
	//Normal
	Light light[3];

	//Skybox
	void RenderSkybox();
	void renderEndBox();
	float rotateSkybox;

	//Text
	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float rotateAngle, float xAxis, float yAxis, float zAxis);
	void renderPosition();

	//Choosing Object, 0 is mountain.
	int selectObject = 0;

	//Enabling and Disabling Collison.
	bool passCol;


	//Render Pause Menu
	void pause();
	//Check if Pause Menu was called
	bool pauseActivate;

	//Cooldown for Button Pressed
	const float cooldownPressed = 0.5f;

	float nextJump = 0.f;

	float nextSplatter = 0.f;

	float nextRun = 0.f;

	float nextPotion = 0.f;

	float nextRobotShoot = 0.f;

	void renderRemainingTime();
	void renderStats();
	void renderPotionCount();
	void renderAntidoteCount();

	void renderInventory();
	void renderHUD();
	void renderEnemy();

	void objectsInit();

    void RenderMesh(Mesh *mesh, bool enableLight);

    //Enemy
    vector<string>enemyType;
    vector<string>enemySpeed;
    vector<string>enemyDamage;
    vector<string>enemyHP;
    vector<string>enemyRange;
    vector<string>enemyState;
    vector<string>numOfWaypoints;
    vector<string>enemyX;
    vector<string>enemyZ;

    //Check if type to load is Object or Enemy
    OBJECTTYPES checkType = NOTHING;
    //Load Configuration from Text File
    void loadConfig();

    //Init enemies information from text file; enemy type, status, waypoint.
    void initEnemy();

    //Render objects.
    void renderObjects();
};

#endif
