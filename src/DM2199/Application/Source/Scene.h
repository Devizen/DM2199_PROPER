#ifndef SCENE_H
#define SCENE_H

#include "FactoryObject.h"
#include "MatrixStack.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Mtx44.h"
#include <vector>
#include "Enemy.h"
#include "Sound.h"

using std::vector;
class Sound;

class Scene
{
    friend FactoryObject;

public:
    enum GEOMETRY_TYPE
    {
        GEO_FLOOR = 0,
        GEO_AXES,
        GEO_X,
        GEO_Y,
        GEO_Z,
        GEO_FPS,

        //Skybox Day
        GEO_LEFT,
        GEO_RIGHT,
        GEO_TOP,
        GEO_BOTTOM,
        GEO_FRONT,
        GEO_BACK,

        //SkyBox Night
        GEO_LEFTn,
        GEO_RIGHTn,
        GEO_TOPn,
        GEO_BOTTOMn,
        GEO_FRONTn,
        GEO_BACKn,

        //Environment
        GEO_MOUNTAIN_2,
        GEO_GROUND,

        //Enemy 1
        GEO_ENEMYHEAD,
        GEO_ENEMYLEFTARM,
        GEO_ENEMYRIGHTARM,
        GEO_ENEMYLEFTLEG,
        GEO_ENEMYRIGHTLEG,
        GEO_ENEMYTORSO,

        //Enemy 2
        GEO_ENEMYLEFTARM2,
        GEO_ENEMYRIGHTARM2,
        GEO_ENEMYLEFTLEG2,
        GEO_ENEMYRIGHTLEG2,
        GEO_ENEMYTORSO2,

        //Enemy 3
        GEO_ENEMYLEFTARM3,
        GEO_ENEMYRIGHTARM3,
        GEO_ENEMYLEFTLEG3,
        GEO_ENEMYRIGHTLEG3,
        GEO_ENEMYTORSO3,

        //Enemy 4
        GEO_ENEMYLEFTARM4,
        GEO_ENEMYRIGHTARM4,
        GEO_ENEMYLEFTLEG4,
        GEO_ENEMYRIGHTLEG4,
        GEO_ENEMYTORSO4,

        //Spider
        GEO_SPIDER,

        GEO_FLICKER,
        GEO_HP75,
        GEO_HP50,
        GEO_HP25,
        GEO_ENEMYHEALTHBAR,

        //Item & Inventory
        GEO_SWORD,

        //minimap
        GEO_MINI_PLAYER,
        GEO_MINI_GROUND,
        GEO_MINI_ENEMY,
        GEO_CURRENT_SELECT,

        GEO_INVENTORY,
        GEO_GAME_HUD,
        GEO_HP,

        GEO_EN,
        GEO_ARROW,
        GEO_ACTIVE_SELECT,
        GEO_SECONDARY_SELECT,
        GEO_TIME,

        GEO_COUNTDOWN,

        //Text
        GEO_TEXT,

        //UI
        GEO_FACE,
        GEO_BACKGROUND,
        GEO_MESSAGES,

        //Sprites
        GEO_HANDL1,
        GEO_HANDL2,
        GEO_HANDR1,
        GEO_HANDR2,
        GEO_HANDI,
        GEO_GUN1,
        GEO_GUN2,
        GEO_GUN3,
        GEO_GUNI,
        GEO_CANNON1,
        GEO_CANNON2,
        GEO_CANNON3,
        GEO_SWORD1,
        GEO_SWORD2,
        GEO_SWORD3,
        GEO_SWORDI,
        GEO_TORCH,
        GEO_TORCHI,
        GEO_HEALTHPOTION,
        GEO_ANTIDOTE,

        //Electric Field
        GEO_ELECTRIC_FIELD,

        //Portal Lightball
        GEO_LIGHTBALL,
        GEO_LIGHTBALL2,
        GEO_LIGHTBALL3,

        //Pause Menu
        PAUSE,
        ARROW,

        //Objects
        GEO_MOUNTAIN,
        GEO_LAMP,
        GEO_LANTERN,
        GEO_TOMBSTONE,
        GEO_TREE,
        GEO_STATUE1,
        GEO_STATUE2,
        GEO_PORTAL,

        //Collided Objects
        COLLIDED_MOUNTAIN,
        COLLIDED_LAMP,
        COLLIDED_LANTERN,
        COLLIDED_TOMBSTONE,
        COLLIDED_TREE,
        COLLIDED_STATUE1,
        COLLIDED_STATUE2,

        //UI
        LEVELEDITOR,
        SELECTION,
        COLLISIONON,
        COLLISIONOFF,
        SAVEON,
        SAVEOFF,
        ROTATEON,
        ROTATEOFF,
        WAYPOINTON,
        WAYPOINTOFF,
        TOGGLEEN,
        TOGGLEENON,
        TOGGLEOBJ,
        TOGGLEOBJON,
        TOGGLEOBJOFF,
        WAYPOINTPROMPT,
        ROTATE0,
        ROTATE90,
        ROTATE180,
        ROTATE270,
        COLLIDED,

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

        U_LIGHT3_POSITION,
        U_LIGHT3_COLOR,
        U_LIGHT3_POWER,
        U_LIGHT3_KC,
        U_LIGHT3_KL,
        U_LIGHT3_KQ,

        U_LIGHT3_TYPE,
        U_LIGHT3_SPOTDIRECTION,
        U_LIGHT3_COSCUTOFF,
        U_LIGHT3_COSINNER,
        U_LIGHT3_EXPONENT,

		U_LIGHT4_POSITION,
		U_LIGHT4_COLOR,
		U_LIGHT4_POWER,
		U_LIGHT4_KC,
		U_LIGHT4_KL,
		U_LIGHT4_KQ,

		U_LIGHT4_TYPE,
		U_LIGHT4_SPOTDIRECTION,
		U_LIGHT4_COSCUTOFF,
		U_LIGHT4_COSINNER,
		U_LIGHT4_EXPONENT,


		U_LIGHT5_POSITION,
		U_LIGHT5_COLOR,
		U_LIGHT5_POWER,
		U_LIGHT5_KC,
		U_LIGHT5_KL,
		U_LIGHT5_KQ,

		U_LIGHT5_TYPE,
		U_LIGHT5_SPOTDIRECTION,
		U_LIGHT5_COSCUTOFF,
		U_LIGHT5_COSINNER,
		U_LIGHT5_EXPONENT,


		U_LIGHT6_POSITION,
		U_LIGHT6_COLOR,
		U_LIGHT6_POWER,
		U_LIGHT6_KC,
		U_LIGHT6_KL,
		U_LIGHT6_KQ,

		U_LIGHT6_TYPE,
		U_LIGHT6_SPOTDIRECTION,
		U_LIGHT6_COSCUTOFF,
		U_LIGHT6_COSINNER,
		U_LIGHT6_EXPONENT,

		U_LIGHT7_POSITION,
		U_LIGHT7_COLOR,
		U_LIGHT7_POWER,
		U_LIGHT7_KC,
		U_LIGHT7_KL,
		U_LIGHT7_KQ,

		U_LIGHT7_TYPE,
		U_LIGHT7_SPOTDIRECTION,
		U_LIGHT7_COSCUTOFF,
		U_LIGHT7_COSINNER,
		U_LIGHT7_EXPONENT,

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
        CONTINUE = 0,
        BACKTOMAINMENU,
    };
    Scene() {}
    ~Scene() {}

    virtual void Init() = 0;
    virtual void Update(double dt) = 0;
    virtual void Render() = 0;
    virtual void Exit() = 0;
    virtual void RenderMesh(Mesh *mesh, bool enableLight);

    enum OBJECTTYPES
    {
        NOTHING = 0,
        OBJECT,
        ENEMY,
    };


    Camera3 camera;
    FactoryObject objFactory;
    MS modelStack, viewStack, projectionStack;
    float _dt;
    Mesh* meshList[NUM_GEOMETRY];
    unsigned m_parameters[U_TOTAL];
    unsigned m_vertexArrayID;
    unsigned m_programID;
    enum OBJECTS
    {
        MOUNTAIN = 0,
        LAMP,
        LANTERN,
        TOMBSTONE,
        TREE,
        STATUE1,
        STATUE2,
        EMPTY1,
        EMPTY2,
        EMPTY3,
    };

    enum ENEMIES
    {
        ROBOT1 = 0,
        ROBOT2,
        ROBOT3,
        ROBOT4,
        SPIDER,
        BLANK1,
        BLANK2,
        BLANK3,
        BLANK4,
        BLANK5,
    };

    enum MODE
    {
        NEUTRAL_IN_OBJECT_MODE = 0,
        OBJECT_TO_ENEMY,
        NEUTRAL_IN_ENEMY_MODE,
        ENEMY_TO_OBJECT_FAIL,
        ENEMY_TO_OBJECT_SUCCESS,
        ENEMY_WAYPOINT_SAVED,
    };

    enum ROTATION
    {
        ZERO = 0,
        NINETY,
        HUNDREDEIGHTY,
        TWOHUNDREDSEVENTY,
    };

    //Enemy storage
    vector<Enemy *> enemyStorage;
    //Sound storage
    vector<Sound *> soundStorage;
    //Pause Menu
    MENU selectOptions = CONTINUE;

    float _elapsedTime = 0;
    float swingTime = 0;

    //Number of Objects
    int numOfObjects = 0;
    //Defining the meshList
    static const int totalObjects = 100;

protected:
    vector<string>objectName;
    vector<string>objectTexture;
    vector<string>objectPosX;
    vector<string>objectPosZ;
    Mesh* meshListPredefined[totalObjects];
    vector <string> initName;
    vector <string> initTexture;


    //Cool down between shooting each bullet
    float nextBulletTime = 0;
    //Cool Down between each walk sound
    float nextWalkTime = 0;
    float nextJump = 0.f;
    float nextSplatter = 0.f;
    float nextRun = 0.f;
    float nextPotion = 0.f;
    float nextRobotShoot = 0.f;

  
    // Cool Down Time
    const float coolDown = 0.25f;

	Vector3 DistanceBetween;


};
#endif
