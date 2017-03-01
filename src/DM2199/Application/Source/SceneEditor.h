#ifndef SCENEEDITOR_H
#define SCENEEDITOR_H

#include "Scene.h"
#include "SceneMainMenu.h"
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
#include <vector>

using std::vector;

class SceneEditor : public Scene
{
    friend class Object;
    friend class Bullet;
    friend class Enemy;
    friend class Sound;
    friend class Camera3;
    friend class SceneMainMenu;
    friend class SceneSplash;
    friend class SceneLoading;
    friend class File;

public:
    SceneEditor();
    ~SceneEditor();

    void renderGround();



    virtual void Init();
    virtual void Update(double dt);
    virtual void Render();
    virtual void Exit();

    float rotateAngle;

    float FPS;


    //Day/Night Cycle
    bool cycle = false;
    bool night = false;
    unsigned cycleState = 0;
    void cycleCheck();
    void renderLightSwitch();

    //Text
    void renderText();

    //Sprites
    void renderSprites();

    //Reset All Settings
    void resetAll();

    FactoryObject objFactory;
    float _dt;
    float _elapsedTime = 0;

    //Sound
    std::vector<Sound *> soundStorage;

    bool inventoryOpen;

    float startTime;
    float cooldown;

    //Loaded check
    static int Loaded();

    //Number of Objects
    int numOfObjects = 0;
    //Defining the meshList
    static const int totalObjects = 100;

    //Copy View from Camera3
    static void copyPositionTargetView(Vector3 position, Vector3 target, Vector3 view, float pitch, float yaw);

    void RenderMesh(Mesh *mesh, bool enableLight);

private:
    //Object
    vector<string>objectName;
    vector<string>objectTexture;
    vector<string>objectPosX;
    vector<string>objectPosZ;

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

    //Normal
    Light light[3];

    //Skybox
    void RenderSkybox();
    void renderEndBox();
    float rotateSkybox;

    //Text
    void RenderText(Mesh* mesh, std::string text, Color color);
    void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
    void RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float sizez, float rotateAngle, float xAxis, float yAxis, float zAxis);
    void renderPosition();

    void renderSelectObject();


    // Cool Down Time
    const float coolDown = 0.25f;
    //Cool down between shooting each bullet
    float nextBulletTime = 0;
    //Cool Down between each walk sound
    float nextWalkTime = 0;

    //Level Editor
    //Load Configuration from Text File
    void loadConfig();

    //Init enemies information from text file; enemy type, status, waypoint.
    void initEnemy();

    //Select Enemies
    ENEMIES selectEnemies = ROBOT1;

    //Toggle between Enemy and Object editing modes.
    MODE toggle = NEUTRAL_IN_OBJECT_MODE;

    //Add Enemies
    void addEnemy();

    //Temporary Enemy Creation
    Enemy* tempEnemyCreate[100];
    unsigned tempEnemyCount = 0;

    //Activate/Deactivate Waypoint Adder
    bool waypointCheck = false;
    
    //Check if waypoint/s is/are added completely.
    bool waypointFinish = true;

    //Prompt end waypoint message to continue.
    bool promptWaypoint = false;

    //Prompt ended waypoint.
    bool endedWaypoint = false;

    //Render Temporary Enemy
    void renderTempEnemy();

    //Check if temporary enemy is needed
    bool tempEnemy = false;

    //Saving Total Waypoint
    int totalWaypoint = 0;

    //Remove Enemies
    void removeEnemy();

    //Removes Waypoints
    void removeWaypoint();

    //Check if type to load is Object or Enemy
    OBJECTTYPES checkType = NOTHING;

    //Render objects from text file
    void renderObjects();

    //Level Select for Edit
    static void selectLevel(string input);

    //Delay for placing objects
    float pressDelay = 0.f;

    //Rotate Objects
    ROTATION selectRotations = ZERO;
    float rotateSelections = 0.f;
    bool rotated = false;

    //Choosing Object, 0 is mountain.
    OBJECTS selectObject = EMPTY3;

    //Add object to scene
    void addObject();

    //Remove previous object from scene
    void removeObject();
    int generateObjects = 0;
    bool removedObject = false;

    //Enabling and Disabling Collison.
    bool passCol;

    //Pause Menu
    MENU selectOptions = CONTINUE;

    //Save Switch; 0 off, 1 save
    unsigned saveEnter = 0;
    float autoSave = 0.f;
    const float autoSaveCooldown = 20.f;
    bool saved = false;

    //Save to Text File Function
    void save();

    //Load from Text File Function
    void load();
    //How many object in the text file.
    unsigned objectCount = 0;

    //Render Pause Menu
    void pause();
    //Check if Pause Menu was called
    bool pauseActivate;

    //Cooldown for Button Pressed
    const float cooldownPressed = 0.5f;

    //Init Object Meshes
    void objectsInit();
    vector <string> initName;
    vector <string> initTexture;

    //Enemy
    void renderEnemy();
    void enemyUpdate(double dt);
    vector<Enemy*> enemyStorage;
    float nextSplatter = 0.f;

    float rotateArm = 0.f;

    float rotateIndividualArm = 0.f;
    float rotateLeftArm = 0.f;
    bool robotShoot = false;
    bool shootArmDown = true;
    bool broughtDown = true;
    float scaleDamage = 0.2f;
    float nextRobotShoot = 0.f;
    bool bulletTouch = false;
    bool damaged = false; // character to enemy

    //Activate Mouse
    float mouseStart;

    bool red = false;
    bool samePos = false;
    
};



#endif