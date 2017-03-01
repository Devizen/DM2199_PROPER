#include "Level2.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "../Application.h"
#include "../MeshBuilder.h"
#include "../LoadTGA.h"
#include "../LoadOBJ.h"

#include "../shader.hpp"
#include "../Utility.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "../ItemInfo.h"
#include "../Inventory.h"

#include "../Bullet.h"

#include "../Spider.h"

#include "../Robot.h"

using std::cout;
using std::cin;

static string choosenLevel = "";

Level2::Level2()
{
}

Level2::~Level2()
{
}

Inventory* level2_inventory = 0;

void Level2::loadConfig()
{

    static unsigned linePos = 1;
    unsigned waypoints = 0;

    const string object = "--Object--";
    const string objectEnd = "--Object End--";
    const string enemy = "--Enemy--";
    const string enemyEnd = "--Enemy End--";

    string pos, posX, posZ;
    bool x = false;

    string line = "";
    ifstream myfile("Level2.txt");

    while (myfile.peek() != EOF)
    {
        getline(myfile, line);

        if (linePos == 1 && line == object)
        {
            checkType = OBJECT;
        }
        else if (linePos == 1 && line == enemy)
        {
            checkType = ENEMY;
        }

        if (checkType == OBJECT)
        {
            if (linePos == 2)
            {
                string pos, posX, posZ;
                bool x = false;
                pos = line;

                for (unsigned i = 0; i < pos.size(); i++)
                {
                    if (pos[i] != ',' && x == false)
                    {
                        posX += pos[i];
                    }
                    if (pos[i] == ',')
                    {
                        x = true;
                        objectPosX.push_back(posX);
                        continue;
                    }
                    if (x == true)
                    {
                        posZ += pos[i];
                    }
                }
                objectPosZ.push_back(posZ);
                posX = "";
                posZ = "";
            }
            else if (linePos == 3)
            {
                objectTexture.push_back(line);

            }

            else if (linePos == 4)
            {
                objectName.push_back(line);

            }

            else if (linePos == 5)
            {

            }
            linePos++;

            if (linePos > 5)
            {
                linePos = 1;
                checkType = NOTHING;
            }
        }
        else if (checkType == ENEMY)
        {
            if (linePos == 2)
            {
                //Robot or Spider, speed, damage, hp, range and state of enemy.
                string getSettings = "",
                    type = "",
                    speed = "",
                    damage = "",
                    hp = "",
                    range = "",
                    state = "";

                bool savedType = false,
                    savedSpeed = false,
                    savedDamage = false,
                    savedHP = false,
                    savedRange = false,
                    savedState = false;

                getSettings = line;

                for (unsigned i = 0; i < getSettings.size(); i++)
                {
                    if (getSettings[i] != ',' && savedType == false)
                    {
                        type += getSettings[i];
                        continue;
                    }
                    if (getSettings[i] == ',' && savedType == false)
                    {
                        savedType = true;
                        enemyType.push_back(type);
                        continue;
                    }

                    if (getSettings[i] != ',' && savedSpeed == false)
                    {
                        speed += getSettings[i];
                        continue;
                    }
                    if (getSettings[i] == ',' && savedSpeed == false)
                    {
                        savedSpeed = true;
                        enemySpeed.push_back(speed);
                        continue;
                    }

                    if (getSettings[i] != ',' &&
                        savedDamage == false &&
                        savedSpeed == true)
                    {
                        damage += getSettings[i];
                        continue;
                    }
                    if (getSettings[i] == ',' &&
                        savedDamage == false &&
                        savedSpeed == true)
                    {
                        savedDamage = true;
                        enemyDamage.push_back(damage);
                        continue;
                    }

                    if (getSettings[i] != ',' &&
                        savedHP == false &&
                        savedDamage == true)
                    {
                        hp += getSettings[i];
                        continue;
                    }
                    if (getSettings[i] == ',' &&
                        savedHP == false &&
                        savedDamage == true)
                    {
                        savedHP = true;
                        enemyHP.push_back(hp);
                        continue;
                    }

                    if (getSettings[i] != ',' &&
                        savedRange == false &&
                        savedHP == true)
                    {
                        range += getSettings[i];
                        continue;
                    }
                    if (getSettings[i] == ',' &&
                        savedRange == false &&
                        savedHP == true)
                    {
                        savedRange = true;
                        enemyRange.push_back(range);
                        continue;
                    }

                    if (getSettings[i] != ',' &&
                        savedState == false &&
                        savedRange == true)
                    {
                        state += getSettings[i];
                    }
                }
                enemyState.push_back(state);

            }
            else if (linePos == 3)
            {
                numOfWaypoints.push_back(line);
                waypoints = stoi(numOfWaypoints.back());

            }

            else if (linePos > 3 && linePos < (4 + waypoints))
            {
                x = false;
                posX = "";
                posZ = "";

                pos = line;

                for (unsigned j = 0; j < pos.size(); j++)
                {
                    if (pos[j] != ',' && x == false)
                    {
                        posX += pos[j];
                    }
                    else if (pos[j] == ',' && x == false)
                    {
                        x = true;
                        enemyX.push_back(posX);
                        continue;
                    }

                    if (x == true)
                    {
                        posZ += pos[j];
                    }
                }
                enemyZ.push_back(posZ);
            }

            linePos++;

            if (linePos > (4 + waypoints))
            {
                linePos = 1;
                checkType = NOTHING;
            }
        }
    }
}

void Level2::initEnemy()
{
    bool speed = false,
        damage = false,
        hp = false,
        range = false,
        state = false,
        numPoint = false,
        way = false,
        x = false,
        z = false;

    unsigned bind = 0;
    string sAmountOfPoints;
    int amountOfPoints = 0;
    unsigned pairCoordiantesWithEnemy = 0;

    Enemy* createEnemy[100];

    for (vector<string>::iterator typeIt = enemyType.begin(); typeIt != enemyType.end(); typeIt++)
    {
        cout << typeIt->data() << endl;
        for (vector<string>::iterator speedIt = enemySpeed.begin() + bind; speedIt != enemySpeed.end() && speed == false; speedIt++)
        {
            cout << speedIt->data() << endl;
            speed = true;
            for (vector<string>::iterator damageIt = enemyDamage.begin() + bind; damageIt != enemyDamage.end() && damage == false; damageIt++)
            {
                cout << damageIt->data() << endl;
                damage = true;
                for (vector<string>::iterator hpIt = enemyHP.begin() + bind; hpIt != enemyHP.end() && hp == false; hpIt++)
                {
                    cout << hpIt->data() << endl;
                    hp = true;
                    for (vector<string>::iterator rangeIt = enemyRange.begin() + bind; rangeIt != enemyRange.end() && range == false; rangeIt++)
                    {
                        cout << rangeIt->data() << endl;
                        range = true;

                        for (vector<string>::iterator stateIt = enemyState.begin() + bind; stateIt != enemyState.end() && state == false; stateIt++)
                        {
                            cout << stateIt->data() << endl;


                            createEnemy[bind] = new Robot(stoi(speedIt->data()),
                                stoi(damageIt->data()),
                                stoi(hpIt->data()),
                                stoi(rangeIt->data()),
                                stoi(stateIt->data()));


                            state = true;

                            for (vector<string>::iterator numPointIt = numOfWaypoints.begin() + bind;
                                numPointIt != numOfWaypoints.end() &&
                                numPoint == false; numPointIt++)
                            {
                                sAmountOfPoints = numPointIt->data();
                                amountOfPoints = stoi(sAmountOfPoints);

                                cout << "Amount Of Points Integer: " << amountOfPoints << endl;
                                cout << "Num Points It: " << numPointIt->data() << endl;
                                cout << "Total Waypoints: " << pairCoordiantesWithEnemy << endl;
                                numPoint = true;
                                break;
                            }

                            for (unsigned i = 0; i < amountOfPoints; i++)
                            {
                                for (vector<string>::iterator xIt = enemyX.begin() + pairCoordiantesWithEnemy; xIt != enemyX.end() && x == false; xIt++)
                                {
                                    x = true;
                                    for (vector<string>::iterator zIt = enemyZ.begin() + pairCoordiantesWithEnemy; zIt != enemyZ.end() && z == false; zIt++)
                                    {
                                        cout << "X: " << xIt->data() << " Z: " << zIt->data() << endl;
                                        createEnemy[bind]->addWaypoint(Vector3(stof(xIt->data()), 0, stof(zIt->data())));
                                        z = true;
                                        break;
                                    }
                                    break;
                                }
                                pairCoordiantesWithEnemy++;
                                x = false;
                                z = false;
                            }
                            break;
                        }
                        break;
                    }
                    break;
                }
                break;
            }
            break;
        }
        enemyStorage.push_back(createEnemy[bind]);
        speed = false;
        damage = false;
        hp = false;
        range = false;
        state = false;
        numPoint = false;
        bind++;
    }
}

void Level2::renderObjects()
{
    float x, z;
    int generateObjects = 0;
    bool nextStep = false;
    //To check and enable collision according to objects.
    const string mountain = "OBJ//mountain.obj";
    const string mountain90 = "OBJ//mountain90.obj";
    const string mountain180 = "OBJ//mountain180.obj";
    const string mountain270 = "OBJ//mountain270.obj";

    const string lamp = "OBJ//lamp.obj";
    const string lamp90 = "OBJ//lamp90.obj";
    const string lamp180 = "OBJ//lamp180.obj";
    const string lamp270 = "OBJ//lamp270.obj";

    const string lantern = "OBJ//lantern.obj";
    const string lantern90 = "OBJ//lantern90.obj";
    const string lantern180 = "OBJ//lantern180.obj";
    const string lantern270 = "OBJ//lantern270.obj";

    const string tombstone = "OBJ//tombstone.obj";
    const string tombstone90 = "OBJ//tombstone90.obj";
    const string tombstone180 = "OBJ//tombstone180.obj";
    const string tombstone270 = "OBJ//tombstone270.obj";

    const string tree = "OBJ//tree.obj";
    const string tree90 = "OBJ//tree90.obj";
    const string tree180 = "OBJ//tree180.obj";
    const string tree270 = "OBJ//tree270.obj";

    const string statue1 = "OBJ//statue1.obj";
    const string statue190 = "OBJ//statue190.obj";
    const string statue1180 = "OBJ//statue1180.obj";
    const string statue1270 = "OBJ//statue1270.obj";

    const string statue2 = "OBJ//statue2.obj";
    const string statue290 = "OBJ//statue290.obj";
    const string statue2180 = "OBJ//statue2180.obj";
    const string statue2270 = "OBJ//statue2270.obj";

    for (vector<string>::reverse_iterator objectrItName = objectName.rbegin(); objectrItName != objectName.rend(); objectrItName++)
    {
        for (vector<string>::reverse_iterator objectrItX = objectPosX.rbegin() + generateObjects; objectrItX != objectPosX.rend() && nextStep == false; objectrItX++)
        {
            nextStep = true;
            for (vector<string>::reverse_iterator objectrItZ = objectPosZ.rbegin() + generateObjects; objectrItZ != objectPosZ.rend(); objectrItZ++)
            {
                x = stof(objectrItX->data());
                z = stof(objectrItZ->data());
                modelStack.PushMatrix();

                if (objectrItName->data() == mountain ||
                    objectrItName->data() == mountain90 ||
                    objectrItName->data() == mountain180 ||
                    objectrItName->data() == mountain270)
                {

                    modelStack.Translate(x, -30.f, z);
                    if (objectrItName->data() == mountain90)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == mountain180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == mountain270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(100.f, 50.f, 100.f);
                    RenderMesh(meshListPredefined[MOUNTAIN], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == lamp ||
                    objectrItName->data() == lamp90 ||
                    objectrItName->data() == lamp180 ||
                    objectrItName->data() == lamp270)
                {

                    modelStack.Translate(x, -30.f, z);
                    if (objectrItName->data() == lamp90)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == lamp180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == lamp270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(10.f, 10.f, 10.f);
                    RenderMesh(meshListPredefined[LAMP], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == lantern ||
                    objectrItName->data() == lantern90 ||
                    objectrItName->data() == lantern180 ||
                    objectrItName->data() == lantern270)
                {

                    modelStack.Translate(x, -30.f, z);
                    if (objectrItName->data() == lantern90)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == lantern180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == lantern270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(10.f, 10.f, 10.f);
                    RenderMesh(meshListPredefined[LANTERN], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == tombstone ||
                    objectrItName->data() == tombstone90 ||
                    objectrItName->data() == tombstone180 ||
                    objectrItName->data() == tombstone270)
                {

                    modelStack.Translate(x, -30.f, z);
                    if (objectrItName->data() == tombstone90)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == tombstone180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == tombstone270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(10.f, 10.f, 10.f);
                    RenderMesh(meshListPredefined[TOMBSTONE], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == tree ||
                    objectrItName->data() == tree90 ||
                    objectrItName->data() == tree180 ||
                    objectrItName->data() == tree270)
                {
                    modelStack.Translate(x, -30.f, z);
                    if (objectrItName->data() == tree90)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == tree180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == tree270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(10.f, 10.f, 10.f);
                    RenderMesh(meshListPredefined[TREE], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == statue1 ||
                    objectrItName->data() == statue190 ||
                    objectrItName->data() == statue1180 ||
                    objectrItName->data() == statue1270)
                {
                    modelStack.Translate(x, 0.f, z);
                    if (objectrItName->data() == statue190)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == statue1180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == statue1270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Scale(20.f, 20.f, 20.f);
                    RenderMesh(meshListPredefined[STATUE1], true);
                    modelStack.PopMatrix();
                    break;

                }

                if (objectrItName->data() == statue2 ||
                    objectrItName->data() == statue290 ||
                    objectrItName->data() == statue2180 ||
                    objectrItName->data() == statue2270)
                {

                    modelStack.Translate(x, 0.f, z);
                    if (objectrItName->data() == statue290)
                    {
                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == statue2180)
                    {
                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    }
                    else if (objectrItName->data() == statue2270)
                    {
                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
                    }
                    modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
                    modelStack.Scale(10.f, 10.f, 10.f);
                    RenderMesh(meshListPredefined[STATUE2], true);
                    modelStack.PopMatrix();
                    break;

                }
            }
        }
        nextStep = false;
        generateObjects++;
    }
}

void Level2::Init()
{
  
    loadConfig();
    objectsInit();
    initEnemy();
	
    // Set background color to black 
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Init VBO here

    // Generate a default VAO for now
    glGenVertexArrays(1, &m_vertexArrayID);
    glBindVertexArray(m_vertexArrayID);

    //Load vertex and fragment shaders
    m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");

    //Get a handle for our "MVP" uniform
    m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
    m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
    m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
    m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
    m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
    m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
    m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");

    m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
    m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
    m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
    m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
    m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
    m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
    m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
    m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
    m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
    m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
    m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

    m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
    m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
    m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
    m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
    m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
    m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
    m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
    m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
    m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
    m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
    m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

    m_parameters[U_LIGHT2_POSITION] = glGetUniformLocation(m_programID, "lights[2].position_cameraspace");
    m_parameters[U_LIGHT2_COLOR] = glGetUniformLocation(m_programID, "lights[2].color");
    m_parameters[U_LIGHT2_POWER] = glGetUniformLocation(m_programID, "lights[2].power");
    m_parameters[U_LIGHT2_KC] = glGetUniformLocation(m_programID, "lights[2].kC");
    m_parameters[U_LIGHT2_KL] = glGetUniformLocation(m_programID, "lights[2].kL");
    m_parameters[U_LIGHT2_KQ] = glGetUniformLocation(m_programID, "lights[2].kQ");
    m_parameters[U_LIGHT2_TYPE] = glGetUniformLocation(m_programID, "lights[2].type");
    m_parameters[U_LIGHT2_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[2].spotDirection");
    m_parameters[U_LIGHT2_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[2].cosCutoff");
    m_parameters[U_LIGHT2_COSINNER] = glGetUniformLocation(m_programID, "lights[2].cosInner");
    m_parameters[U_LIGHT2_EXPONENT] = glGetUniformLocation(m_programID, "lights[2].exponent");

    m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");

    m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

    // Get a handle for our "colorTexture" uniform
    m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
    m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

    // Get a handle for our "textColor" uniform
    m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
    m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");


    // Use our shader
    glUseProgram(m_programID);


    for (int i = 0; i < totalObjects; i++)
    {
        meshListPredefined[i] = NULL;
    }


    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        meshList[i] = NULL;
    }

    int generateMesh = 0;
    for (vector<string>::iterator objIt = initName.begin(); objIt != initName.end(); objIt++)
    {
        for (vector<string>::iterator texIt = initTexture.begin() + generateMesh; texIt != initTexture.end(); texIt++)
        {
            meshListPredefined[generateMesh] = MeshBuilder::GenerateOBJ(objIt->data(), objIt->data());
            meshListPredefined[generateMesh]->textureID = LoadTGA(texIt->data());
            break;
        }
        generateMesh++;
    }

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);

    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 20, 20, 1);
    meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball_2", Color(0.5f, 0.f, 0.f), 20, 20, 1);
    meshList[GEO_LIGHTBALL3] = MeshBuilder::GenerateSphere("lightball_3", Color(1.f, 1.f, 0.f), 20, 20, 1);

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("grass", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image//MetalFloor3.tga");

    //Skybox Day
    meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_FRONT]->textureID = LoadTGA("Image//front.tga");

    meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BACK]->textureID = LoadTGA("Image//back.tga");

    meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_TOP]->textureID = LoadTGA("Image//top.tga");

    meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//bottom.tga");

    meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_LEFT]->textureID = LoadTGA("Image//left.tga");

    meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_RIGHT]->textureID = LoadTGA("Image//right.tga");

    //Skybox Night
    meshList[GEO_FRONTn] = MeshBuilder::GenerateQuad("frontn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_FRONTn]->textureID = LoadTGA("Image//frontn.tga");

    meshList[GEO_BACKn] = MeshBuilder::GenerateQuad("backn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BACKn]->textureID = LoadTGA("Image//backn.tga");

    meshList[GEO_TOPn] = MeshBuilder::GenerateQuad("topn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_TOPn]->textureID = LoadTGA("Image//topn.tga");

    meshList[GEO_BOTTOMn] = MeshBuilder::GenerateQuad("bottomn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_BOTTOMn]->textureID = LoadTGA("Image//bottomn.tga");

    meshList[GEO_LEFTn] = MeshBuilder::GenerateQuad("leftn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_LEFTn]->textureID = LoadTGA("Image//leftn.tga");

    meshList[GEO_RIGHTn] = MeshBuilder::GenerateQuad("rightn", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_RIGHTn]->textureID = LoadTGA("Image//rightn.tga");

    //Text
    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_X] = MeshBuilder::GenerateText("x", 16, 16);
    meshList[GEO_X]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_Y] = MeshBuilder::GenerateText("y", 16, 16);
    meshList[GEO_Y]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_Z] = MeshBuilder::GenerateText("z", 16, 16);
    meshList[GEO_Z]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_FPS] = MeshBuilder::GenerateText("FPS", 16, 16);
    meshList[GEO_FPS]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_COUNTDOWN] = MeshBuilder::GenerateText("Time", 16, 16);
    meshList[GEO_COUNTDOWN]->textureID = LoadTGA("Image//comic.tga");

    //Environment
    meshList[GEO_MOUNTAIN] = MeshBuilder::GenerateOBJ("mountain", "OBJ//mountain.obj");
    meshList[GEO_MOUNTAIN]->textureID = LoadTGA("Image//objects.tga");

    meshList[GEO_MOUNTAIN_2] = MeshBuilder::GenerateOBJ("mountain_2", "OBJ//mountain.obj");
    meshList[GEO_MOUNTAIN_2]->textureID = LoadTGA("Image//objects.tga");

    meshList[GEO_MESSAGES] = MeshBuilder::GenerateText("messages", 16, 16);
    meshList[GEO_MESSAGES]->textureID = LoadTGA("Image//comic.tga");

    //Sprites
    meshList[GEO_HANDL1] = MeshBuilder::GenerateOBJ("handL1", "OBJ//quad.obj");
    meshList[GEO_HANDL1]->textureID = LoadTGA("Image//Sprites//HandL1.tga");
    meshList[GEO_HANDR1] = MeshBuilder::GenerateOBJ("handR1", "OBJ//quad.obj");
    meshList[GEO_HANDR1]->textureID = LoadTGA("Image//Sprites//HandR1.tga");
    meshList[GEO_HANDL2] = MeshBuilder::GenerateOBJ("handL2", "OBJ//quad.obj");
    meshList[GEO_HANDL2]->textureID = LoadTGA("Image//Sprites//HandL2.tga");
    meshList[GEO_HANDR2] = MeshBuilder::GenerateOBJ("handR2", "OBJ//quad.obj");
    meshList[GEO_HANDR2]->textureID = LoadTGA("Image//Sprites//HandR2.tga");
    meshList[GEO_HANDI] = MeshBuilder::GenerateOBJ("handI", "OBJ//quad.obj");
    meshList[GEO_HANDI]->textureID = LoadTGA("Image//Sprites//HandI.tga");
    meshList[GEO_GUN1] = MeshBuilder::GenerateOBJ("gun1", "OBJ//quad.obj");
    meshList[GEO_GUN1]->textureID = LoadTGA("Image//Sprites//Gun1.tga");
    meshList[GEO_GUN2] = MeshBuilder::GenerateOBJ("gun2", "OBJ//quad.obj");
    meshList[GEO_GUN2]->textureID = LoadTGA("Image//Sprites//Gun2.tga");
    meshList[GEO_GUN3] = MeshBuilder::GenerateOBJ("gun3", "OBJ//quad.obj");
    meshList[GEO_GUN3]->textureID = LoadTGA("Image//Sprites//Gun3.tga");
    meshList[GEO_GUNI] = MeshBuilder::GenerateOBJ("gunI", "OBJ//quad.obj");
    meshList[GEO_GUNI]->textureID = LoadTGA("Image//Sprites//GunI.tga");
    meshList[GEO_SWORD1] = MeshBuilder::GenerateOBJ("sword1", "OBJ//quad.obj");
    meshList[GEO_SWORD1]->textureID = LoadTGA("Image//Sprites//Sword1.tga");
    meshList[GEO_SWORD2] = MeshBuilder::GenerateOBJ("sword2", "OBJ//quad.obj");
    meshList[GEO_SWORD2]->textureID = LoadTGA("Image//Sprites//Sword2.tga");
    meshList[GEO_SWORD3] = MeshBuilder::GenerateOBJ("sword3", "OBJ//quad.obj");
    meshList[GEO_SWORD3]->textureID = LoadTGA("Image//Sprites//Sword3.tga");
    meshList[GEO_SWORDI] = MeshBuilder::GenerateOBJ("swordI", "OBJ//quad.obj");
    meshList[GEO_SWORDI]->textureID = LoadTGA("Image//Sprites//SwordI.tga");
    meshList[GEO_TORCH] = MeshBuilder::GenerateOBJ("torch", "OBJ//quad.obj");
    meshList[GEO_TORCH]->textureID = LoadTGA("Image//Sprites//Torch.tga");
    meshList[GEO_TORCHI] = MeshBuilder::GenerateOBJ("torchI", "OBJ//quad.obj");
    meshList[GEO_TORCHI]->textureID = LoadTGA("Image//Sprites//TorchI.tga");
    meshList[GEO_HEALTHPOTION] = MeshBuilder::GenerateOBJ("healthPotion", "OBJ//quad.obj");
    meshList[GEO_HEALTHPOTION]->textureID = LoadTGA("Image//Sprites//HealthPotion.tga");
    meshList[GEO_ANTIDOTE] = MeshBuilder::GenerateOBJ("antidote", "OBJ//quad.obj");
    meshList[GEO_ANTIDOTE]->textureID = LoadTGA("Image//Sprites//Antidote.tga");

    //Electric Field
    meshList[GEO_ELECTRIC_FIELD] = MeshBuilder::GenerateOBJ("electricField", "OBJ//Wall.obj");
    meshList[GEO_ELECTRIC_FIELD]->textureID = LoadTGA("Image//ElectricField.tga");

    //Item
    meshList[GEO_INVENTORY] = MeshBuilder::GenerateOBJ("Inventory", "OBJ//inventory.obj");
    meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventory.tga");

    meshList[GEO_MINI_PLAYER] = MeshBuilder::GenerateOBJ("mini_player", "OBJ//inventory.obj");
    meshList[GEO_MINI_PLAYER]->textureID = LoadTGA("Image//mini_player.tga");

    meshList[GEO_MINI_GROUND] = MeshBuilder::GenerateOBJ("mini_ground", "OBJ//inventory.obj");
    meshList[GEO_MINI_GROUND]->textureID = LoadTGA("Image//MetalFloor3.tga");

    meshList[GEO_MINI_ENEMY] = MeshBuilder::GenerateOBJ("mini_enemy", "OBJ//inventory.obj");
    meshList[GEO_MINI_ENEMY]->textureID = LoadTGA("Image//mini_enemy.tga");

    meshList[GEO_GAME_HUD] = MeshBuilder::GenerateOBJ("game_hud", "OBJ//inventory.obj");
    meshList[GEO_GAME_HUD]->textureID = LoadTGA("Image//game_hud.tga");

    meshList[GEO_HP] = MeshBuilder::GenerateOBJ("game_hp", "OBJ//inventory.obj");
    meshList[GEO_HP]->textureID = LoadTGA("Image//hp_bar.tga");

    meshList[GEO_EN] = MeshBuilder::GenerateOBJ("game_en", "OBJ//inventory.obj");
    meshList[GEO_EN]->textureID = LoadTGA("Image//en_bar.tga");

    //Enemy 
    meshList[GEO_ENEMYHEAD] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyHead.obj");
    meshList[GEO_ENEMYHEAD]->textureID = LoadTGA("Image//Enemy//EnemyHead.tga");

    meshList[GEO_ENEMYRIGHTARM] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyRightArm.obj");
    meshList[GEO_ENEMYRIGHTARM]->textureID = LoadTGA("Image//Enemy//EnemyBodyParts.tga");

    meshList[GEO_ENEMYLEFTARM] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyLeftArm.obj");
    meshList[GEO_ENEMYLEFTARM]->textureID = LoadTGA("Image//Enemy//EnemyBodyParts.tga");

    meshList[GEO_ENEMYLEFTLEG] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyLeftLeg.obj");
    meshList[GEO_ENEMYLEFTLEG]->textureID = LoadTGA("Image//Enemy//EnemyBodyParts.tga");

    meshList[GEO_ENEMYRIGHTLEG] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyRightLeg.obj");
    meshList[GEO_ENEMYRIGHTLEG]->textureID = LoadTGA("Image//Enemy//EnemyBodyParts.tga");

    meshList[GEO_ENEMYTORSO] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//EnemyTorso.obj");
    meshList[GEO_ENEMYTORSO]->textureID = LoadTGA("Image//Enemy//EnemyTorso.tga");

    //Enemy 2
    meshList[GEO_ENEMYRIGHTARM2] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy2RightArm.obj");
    meshList[GEO_ENEMYRIGHTARM2]->textureID = LoadTGA("Image//Enemy//Enemy2BodyParts.tga");
    meshList[GEO_ENEMYLEFTARM2] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy2LeftArm.obj");
    meshList[GEO_ENEMYLEFTARM2]->textureID = LoadTGA("Image//Enemy//Enemy2BodyParts.tga");
    meshList[GEO_ENEMYLEFTLEG2] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy2LeftLeg.obj");
    meshList[GEO_ENEMYLEFTLEG2]->textureID = LoadTGA("Image//Enemy//Enemy2BodyParts.tga");
    meshList[GEO_ENEMYRIGHTLEG2] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy2RightLeg.obj");
    meshList[GEO_ENEMYRIGHTLEG2]->textureID = LoadTGA("Image//Enemy//Enemy2BodyParts.tga");
    meshList[GEO_ENEMYTORSO2] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy2Torso.obj");
    meshList[GEO_ENEMYTORSO2]->textureID = LoadTGA("Image//Enemy//Enemy2BodyParts.tga");

    //Enemy 3
    meshList[GEO_ENEMYRIGHTARM3] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy3RightArm.obj");
    meshList[GEO_ENEMYRIGHTARM3]->textureID = LoadTGA("Image//Enemy//Enemy3BodyParts.tga");
    meshList[GEO_ENEMYLEFTARM3] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy3LeftArm.obj");
    meshList[GEO_ENEMYLEFTARM3]->textureID = LoadTGA("Image//Enemy//Enemy3BodyParts.tga");
    meshList[GEO_ENEMYLEFTLEG3] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy3LeftLeg.obj");
    meshList[GEO_ENEMYLEFTLEG3]->textureID = LoadTGA("Image//Enemy//Enemy3BodyParts.tga");
    meshList[GEO_ENEMYRIGHTLEG3] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy3RightLeg.obj");
    meshList[GEO_ENEMYRIGHTLEG3]->textureID = LoadTGA("Image//Enemy//Enemy3BodyParts.tga");
    meshList[GEO_ENEMYTORSO3] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy3Torso.obj");
    meshList[GEO_ENEMYTORSO3]->textureID = LoadTGA("Image//Enemy//Enemy3BodyParts.tga");

    //Enemy 4
    meshList[GEO_ENEMYRIGHTARM4] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy4RightArm.obj");
    meshList[GEO_ENEMYRIGHTARM4]->textureID = LoadTGA("Image//Enemy//Enemy4BodyParts.tga");
    meshList[GEO_ENEMYLEFTARM4] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy4LeftArm.obj");
    meshList[GEO_ENEMYLEFTARM4]->textureID = LoadTGA("Image//Enemy//Enemy4BodyParts.tga");
    meshList[GEO_ENEMYLEFTLEG4] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy4LeftLeg.obj");
    meshList[GEO_ENEMYLEFTLEG4]->textureID = LoadTGA("Image//Enemy//Enemy4BodyParts.tga");
    meshList[GEO_ENEMYRIGHTLEG4] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy4RighLeg.obj");
    meshList[GEO_ENEMYRIGHTLEG4]->textureID = LoadTGA("Image//Enemy//Enemy4BodyParts.tga");
    meshList[GEO_ENEMYTORSO4] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Enemy4Torso.obj");
    meshList[GEO_ENEMYTORSO4]->textureID = LoadTGA("Image//Enemy//Enemy4BodyParts.tga");

    //Flicker
    meshList[GEO_FLICKER] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//quad.obj");
    meshList[GEO_FLICKER]->textureID = LoadTGA("Image//blood//bloodflicker.tga");

    meshList[GEO_SPIDER] = MeshBuilder::GenerateOBJ("EnemyHead", "OBJ//Enemy//Spider.obj");
    meshList[GEO_SPIDER]->textureID = LoadTGA("Image//Enemy//RobotSpider.tga");

    meshList[GEO_ENEMYHEALTHBAR] = MeshBuilder::GenerateQuad("enemyHealthBar", Color(1.0f, 0.0f, 0.0f), 1, 1);

    meshList[GEO_ARROW] = MeshBuilder::GenerateOBJ("Arrow", "OBJ//inventory.obj");
    meshList[GEO_ARROW]->textureID = LoadTGA("Image//Arrow.tga");

    meshList[GEO_ACTIVE_SELECT] = MeshBuilder::GenerateOBJ("active_select", "OBJ//inventory.obj");
    meshList[GEO_ACTIVE_SELECT]->textureID = LoadTGA("Image//active_select.tga");

    meshList[GEO_SECONDARY_SELECT] = MeshBuilder::GenerateOBJ("secondary_select", "OBJ//inventory.obj");
    meshList[GEO_SECONDARY_SELECT]->textureID = LoadTGA("Image//secondary_select.tga");

    meshList[GEO_TIME] = MeshBuilder::GenerateOBJ("time_bar", "OBJ//inventory.obj");
	meshList[GEO_TIME]->textureID = LoadTGA("Image//time_bar.tga");

	

    //Prevent Jerk
    camera.Init(Vector3(0, 0, 484), Vector3(0, 0, 0), Vector3(0, 1, 0));

    //Projection
    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 4000.f);
    projectionStack.LoadMatrix(projection);

    //Spot light
    light[0].type = Light::LIGHT_SPOT;
    light[0].position.Set(0, 10, 0);// this is overidden at the update
    light[0].color.Set(1, 1, 1);
    light[0].power = 0;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;
    light[0].cosCutoff = cos(Math::DegreeToRadian(25)); // higher angle = more area covered with light
    light[0].cosInner = cos(Math::DegreeToRadian(15)); // use to toggle inner light , lower angle  = light more concentrated
    light[0].exponent = 3.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f); // this is overidden at the update

    light[1].type = Light::LIGHT_POINT;
    light[1].position.Set(962.f, 0.f, 328.f);
    light[1].color.Set(1.f, 0.f, 0.f);
    light[1].power = 1.f;
    light[1].kC = 1.f;
    light[1].kL = 0.01f;
    light[1].kQ = 0.001f;
    light[1].cosCutoff = cos(Math::DegreeToRadian(45));
    light[1].cosInner = cos(Math::DegreeToRadian(30));
    light[1].exponent = 3.f;
    light[1].spotDirection.Set(0.f, 1.f, 0.f);

	light[2].type = Light::LIGHT_POINT;
    light[2].position.Set(-1095.f, 0.f, 298.f);
    light[2].color.Set(1.f, 1.f, 0.f);
    light[2].power = 1.f;
    light[2].kC = 1.f;
    light[2].kL = 0.01f;
    light[2].kQ = 0.001f;
    light[2].cosCutoff = cos(Math::DegreeToRadian(45));
    light[2].cosInner = cos(Math::DegreeToRadian(30));
    light[2].exponent = 3.f;
    light[2].spotDirection.Set(0.f, 1.f, 0.f);



	light[3].type = Light::LIGHT_POINT;
	light[3].position.Set(-414.f, 0.f, 957.f);
	light[3].color.Set(1.f, 0.43f, 0.78f);
	light[3].power = 100.f;
	light[3].kC = 1.f;
	light[3].kL = 0.01f;
	light[3].kQ = 0.001f;
	light[3].cosCutoff = cos(Math::DegreeToRadian(45));
	light[3].cosInner = cos(Math::DegreeToRadian(30));
	light[3].exponent = 3.f;
	light[3].spotDirection.Set(0.f, 1.f, 0.f);


	light[4].type = Light::LIGHT_POINT;
	light[4].position.Set(-415.f, 0.f, 954.f);
	light[4].color.Set(0.13f, 0.37f, 0.31f);
	light[4].power = 1.f;
	light[4].kC = 1.f;
	light[4].kL = 0.01f;
	light[4].kQ = 0.001f;
	light[4].cosCutoff = cos(Math::DegreeToRadian(45));
	light[4].cosInner = cos(Math::DegreeToRadian(30));
	light[4].exponent = 3.f;
	light[4].spotDirection.Set(0.f, 1.f, 0.f);


	light[5].type = Light::LIGHT_POINT;
	light[5].position.Set(243.f, 0.f, 611.f);
	light[5].color.Set(1.f, 0.43f, 0.78f);
	light[5].power = 1.f;
	light[5].kC = 1.f;
	light[5].kL = 0.01f;
	light[5].kQ = 0.001f;
	light[5].cosCutoff = cos(Math::DegreeToRadian(45));
	light[5].cosInner = cos(Math::DegreeToRadian(30));
	light[5].exponent = 3.f;
	light[5].spotDirection.Set(0.f, 1.f, 0.f);

    // Make sure you pass uniform parameters after glUseProgram()
    glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
    glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
    glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
    glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
    glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
    glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
    glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

    glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
    glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
    glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
    glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
    glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
    glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
    glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
    glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);

    glUniform1i(m_parameters[U_LIGHT2_TYPE], light[2].type);
    glUniform3fv(m_parameters[U_LIGHT2_COLOR], 1, &light[2].color.r);
    glUniform1f(m_parameters[U_LIGHT2_POWER], light[2].power);
    glUniform1f(m_parameters[U_LIGHT2_KC], light[2].kC);
    glUniform1f(m_parameters[U_LIGHT2_KL], light[2].kL);
    glUniform1f(m_parameters[U_LIGHT2_KQ], light[2].kQ);
    glUniform1f(m_parameters[U_LIGHT2_COSCUTOFF], light[2].cosCutoff);
    glUniform1f(m_parameters[U_LIGHT2_COSINNER], light[2].cosInner);
    glUniform1f(m_parameters[U_LIGHT2_EXPONENT], light[2].exponent);

	glUniform1i(m_parameters[U_LIGHT3_TYPE], light[3].type);
	glUniform3fv(m_parameters[U_LIGHT3_COLOR], 1, &light[3].color.r);
	glUniform1f(m_parameters[U_LIGHT3_POWER], light[3].power);
	glUniform1f(m_parameters[U_LIGHT3_KC], light[3].kC);
	glUniform1f(m_parameters[U_LIGHT3_KL], light[3].kL);
	glUniform1f(m_parameters[U_LIGHT3_KQ], light[3].kQ);
	glUniform1f(m_parameters[U_LIGHT3_COSCUTOFF], light[3].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT3_COSINNER], light[3].cosInner);
	glUniform1f(m_parameters[U_LIGHT3_EXPONENT], light[3].exponent);


	glUniform1i(m_parameters[U_LIGHT4_TYPE], light[4].type);
	glUniform3fv(m_parameters[U_LIGHT4_COLOR], 1, &light[4].color.r);
	glUniform1f(m_parameters[U_LIGHT4_POWER], light[4].power);
	glUniform1f(m_parameters[U_LIGHT4_KC], light[4].kC);
	glUniform1f(m_parameters[U_LIGHT4_KL], light[4].kL);
	glUniform1f(m_parameters[U_LIGHT4_KQ], light[4].kQ);
	glUniform1f(m_parameters[U_LIGHT4_COSCUTOFF], light[4].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT4_COSINNER], light[4].cosInner);
	glUniform1f(m_parameters[U_LIGHT4_EXPONENT], light[4].exponent);


	glUniform1i(m_parameters[U_LIGHT5_TYPE], light[5].type);
	glUniform3fv(m_parameters[U_LIGHT5_COLOR], 1, &light[5].color.r);
	glUniform1f(m_parameters[U_LIGHT5_POWER], light[5].power);
	glUniform1f(m_parameters[U_LIGHT5_KC], light[5].kC);
	glUniform1f(m_parameters[U_LIGHT5_KL], light[5].kL);
	glUniform1f(m_parameters[U_LIGHT5_KQ], light[5].kQ);
	glUniform1f(m_parameters[U_LIGHT5_COSCUTOFF], light[5].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT5_COSINNER], light[5].cosInner);
	glUniform1f(m_parameters[U_LIGHT5_EXPONENT], light[5].exponent);


	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 6);


    // Param 1 - name of sound ---- Param 2 -distance sound able to travel --- Param 3 - volume of sound (0 to 1)
    soundStorage.push_back(new Sound("gunshot.mp3", 1000, 1));
    soundStorage.push_back(new Sound("splatter.mp3", 1000, 1));
    soundStorage.push_back(new Sound("run.mp3", 1000, 0.5));
    soundStorage.push_back(new Sound("background2.mp3"));
    soundStorage.push_back(new Sound("jump.wav", 1000, 0.5));
	soundStorage.push_back(new Sound("footstep.mp3", 1000, 1));


    /* vec3df somePosition = { 0, 0,0};
    soundStorage[1]->play3DSound(true, false, false, somePosition);*/

    /*soundStorage[3]->play2DSound(true, false, false); */

    /*objFactory.createFactoryObject(new Enemy(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))) );*/

    //----------------Inventory--------------
    inventoryOpen = false;

    //initialise singleton of inventory class

    if (level2_inventory == 0)
    {
        level2_inventory = new Inventory();
        level2_inventory->setdefined(1);
    }
    //type , damage , range , reload speed , attack speed
    ItemInfo* gun = new ItemInfo("gun", 20, 20, 10, 15);
    level2_inventory->addItem(gun);

    ItemInfo* torch = new ItemInfo("torch", 0, 50, 0, 10);
    level2_inventory->addItem(torch);
    // adding items to inventory
    ItemInfo* fist = new ItemInfo();
    level2_inventory->addItem(fist);

    //delay between keypresses while in menu
    startTime = 0.0f;
    cooldown = 15.0f;

    //speed ,damage ,hp, range ,this
    Enemy * newEnemy = new Spider(70.f, 5.f, 50.f, 100.f);
    newEnemy->addWaypoint(Vector3(200, 0, 200));
    newEnemy->addWaypoint(Vector3(200, 0, -200));
    newEnemy->addWaypoint(Vector3(-200, 0, -200));
    newEnemy->addWaypoint(Vector3(-200, 0, 200));
    enemyStorage.push_back(newEnemy);

    // speed , damage ,hp , range , this , int(used to identify which type of robot)
    Enemy * newEnemy2 = new Robot(70.f, 5.f, 100.f, 100.f, 1);
    newEnemy2->addWaypoint(Vector3(300, 0, -100));
    newEnemy2->addWaypoint(Vector3(400, 0, -400));
    newEnemy2->addWaypoint(Vector3(-400, 0, -400));
    newEnemy2->addWaypoint(Vector3(-400, 0, 400));
    enemyStorage.push_back(newEnemy2);

    Enemy * newEnemy3 = new Spider(70.f, 5.f, 50.f, 100.f);
    newEnemy3->addWaypoint(Vector3(-400, 0, 200));
    newEnemy3->addWaypoint(Vector3(300, 0, -300));
    newEnemy3->addWaypoint(Vector3(-2300, 0, -300));
    newEnemy3->addWaypoint(Vector3(-2300, 0, 200));
    enemyStorage.push_back(newEnemy3);

    for (vector<Enemy*>::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
    {
        (*it)->_Position = (*it)->currWaypoint->getPosition();
    }
	soundStorage[3]->play2DSound(false, false, false);

	Camera3::collisionSwitch(true, "level2.txt");
}

void Level2::objectsInit()
{
    string line = "";
    ifstream myfile("objects.txt");
    static int linePos = 1;

    while (myfile.peek() != EOF)
    {
        getline(myfile, line);

        if (linePos == 1)
        {
            initName.push_back(line);
        }
        if (linePos == 2)
        {
            initTexture.push_back(line);
        }
        if (linePos < 2)
        {
            linePos++;
        }
        else
        {
            linePos = 1;
        }
    }
}

void Level2::Update(double dt)
{
	//Delay for placing objects
	static float pressDelay = 0.f;
	//Delay for footstep sound
	static float pressDelay2 = 0.f;

    static float mouseStart = 0.f;
    mouseStart += float(dt);
    if (mouseStart > 5.f)
    {
        Camera3::loadedCheck(true);
    }
    Camera3::collisionSwitch(true, "Level2.txt");

	for (auto it1 = enemyStorage.begin(); it1 != enemyStorage.end(); it1++)
	{
		if ((*(it1 + 1)) != NULL)
		{
			for (auto it2 = it1 + 1; it2 != enemyStorage.end(); it2++)
			{
				if (((*it1)->_Position - (*it2)->_Position).Length() < 20 &&
					(*it1)->getState() != 2 && (*it2)->getState() != 2)
				{

					DistanceBetween = ((*it1)->_Position - (*it2)->_Position).Normalized();
					(*it1)->_Position += DistanceBetween;
					(*it2)->_Position -= DistanceBetween;

				}

			}
		}
	}


    vec3df camPos = { camera.getPosition().x, camera.getPosition().y, camera.getPosition().z };
    Vector3 view = camera.target - camera.position;
    vec3df dir = { view.x, view.y, view.z };
    vec3df up = { camera.up.x, camera.up.y, camera.up.z };
    vec3df zero = { 0, 0, 0 };

    for (unsigned i = 0; i < soundStorage.size(); ++i)
    {
        soundStorage[i]->getSoundEngine()->setListenerPosition(camPos, dir.normalize(), zero, up.normalize());
        soundStorage[i]->getSoundEngine()->update();
    }

    //soundStorage[3]->play2DSound(false,false,false);

    ik_f32 returnVol = SceneMainMenu::soundFromMenue();
    Sound::soundEngine->setSoundVolume(returnVol);

    Robot::dtFromScene(dt);
    Robot::positionFromCamera(camera);

    Spider::dtFromScene(dt);
    Spider::positionFromCamera(camera);

    
    _dt = (float)dt;
    _elapsedTime += _dt;

    static float rotateLimit = 1;
    static float rotateLeftLimit = 1;
    static float rotateLimitIndividual = 1;

    rotateArm += (float)(50 * rotateLimit * dt);

    if (shootArmDown == true && rotateLeftArm == 0.f)
    {
        rotateIndividualArm += (float)(50 * rotateLimitIndividual * dt);
    }

    if (robotShoot == true)
    {
        rotateIndividualArm = 0.f;
        if (rotateLeftArm < 90)
        {
            rotateLeftLimit = 1;
            rotateLeftArm += (float)(150 * rotateLeftLimit * dt);
        }
        if (rotateLeftArm >= 90.f)
        {
            rotateLeftArm = 90.f;
            robotShoot = false;
            shootArmDown = false;
        }
    }
    else if (shootArmDown == false && rotateLeftArm > 0.f)
    {
        rotateLeftLimit = -1;
        rotateLeftArm += (float)(150 * rotateLeftLimit * dt);

        if (rotateLeftArm <= 0.f)
        {
            rotateLeftArm = 0.f;
            shootArmDown = true;
            broughtDown = true;
        }
    }

    if (rotateIndividualArm > 10.f)
    {
        rotateIndividualArm = 10.f;
        rotateLimitIndividual = -1.f;
    }
    if (rotateIndividualArm < -10.f)
    {
        rotateIndividualArm = -10.f;
        rotateLimitIndividual = 1.f;
    }

    if (rotateArm > 10)
    {
        rotateArm = 10.f;
        rotateLimit = -1;
    }
    if (rotateArm < -10)
    {
        rotateArm = -10.f;
        rotateLimit = 1;
    }

    static bool canPress = true;

	if (!Application::IsKeyPressed(MK_RBUTTON))
		canPress = true;

	// Light on
	if (canPress && Application::IsKeyPressed(MK_RBUTTON)) {
		light[0].power = (light[0].power > 0) ? 0.0f : 3.0f; // Toggle Power between 0 and 2
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		canPress = false;
	}
    if (light[0].power > 0)
    {
        float ys = 10.f;
        energy -=int( ys * _dt);
    }
    if (torchDead == true)
    {
        light[0].power = 0;
        glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
    }
    if (energy < 0)
    {
        energy = 0;
    }
    if (energy > 10000)
    {
        energy = 10000;
    }
    // Flashlight position and direction
    light[0].position.Set(camera.position.x, camera.position.y, camera.position.z);
    light[0].spotDirection = camera.position - camera.target;  // move direction of light along view vector

    //Process  obj movement
    objFactory.processInteraction();

    static const float LSPEED = 10.f;
    srand((unsigned)time(NULL));

    static float rotateWorld = 1;

    float run = 1.f;



    if (Application::IsKeyPressed(MK_LBUTTON))
    {
        if (_elapsedTime >= nextBulletTime)
        {
            objFactory.createFactoryObject(new Bullet(this, camera.getPosition(), camera.getYaw(), camera.getPitch()));
            nextBulletTime = _elapsedTime + coolDown;
            soundStorage[0]->play3DSound(false, false, false, camPos);
            bulletTouch = false;
            swing = true;
        }
    }
    if (swing == true)
    {
        swingTime++;
    }

    if (Application::IsKeyPressed('I'))
    {
        if (inventoryOpen && startTime >= cooldown)
        {
            inventoryOpen = false;
            startTime = 0;
        }
        else if (startTime >= cooldown)
        {
            inventoryOpen = true;
            startTime = 0;
        }
    }

    //Poison, using antidote
    if (Application::IsKeyPressed('C') && camera.poison == false && usedAntidoteOnce == false)
        notPoisoned = true;

    if (Application::IsKeyPressed('C') && antidoteCount > 0 && camera.poison == true)
    {
        camera.poison = false;
        antidoteUsed = true;
        usedAntidoteOnce = true;
        antidoteCount--;
    }
    if (Application::IsKeyPressed('C') && antidoteCount == 0 && usedAntidoteOnce == false && camera.poison == true)
    {
        noAntidote = true;
    }
    if (antidoteUsed == true || noAntidote == true || notPoisoned == true)
        usedAntidoteTime++;
    if (usedAntidoteTime > 0 && usedAntidoteTime < 150)
        usedAntidoteText = true;
    else
        usedAntidoteText = false;
    if (usedAntidoteText == false && (antidoteUsed == true || noAntidote == true || notPoisoned == true))
    {
        antidoteUsed = false;
        noAntidote = false;
        usedAntidoteOnce = false;
        notPoisoned = false;
        usedAntidoteTime = 0;
    }

    //Health, using health potion
    if (Application::IsKeyPressed('Z') && camera.health == 1000 && usedPotionOnce == false)
        notHurt = true;
    if (Application::IsKeyPressed('Z') && potionCount > 0 && camera.health < 1000)
    {
        if (_elapsedTime >= nextPotion)
        {
            camera.health += 30;
            potionUsed = true;
            usedPotionOnce = true;
            potionCount--;
            nextPotion = _elapsedTime + coolDown + 5.0f;
        }
    }
    if (Application::IsKeyPressed('Z') && potionCount == 0 && usedPotionOnce == false && camera.health < 1000)
    {
        noPotion = true;
    }
    if (potionUsed == true || noPotion == true || notHurt == true)
        usedPotionTime++;
    if (usedPotionTime > 0 && usedPotionTime < 150)
        usedPotionText = true;
    else
        usedPotionText = false;
    if (usedPotionText == false && (potionUsed == true || noPotion == true || notHurt == true))
    {
        potionUsed = false;
        noPotion = false;
        usedPotionOnce = false;
        notHurt = false;
        usedPotionTime = 0;
    }

    vec3df footPos = { camera.getPosition().x, camera.getPosition().y - 20, camera.getPosition().z };
    //FootStep Sound

	//Limit pressDelay value
	if (pressDelay > 0.5f)
	{
		pressDelay = 0.5f;
	}

	if (pressDelay2 > 1.f)
	{
		pressDelay2 = 1.f;
	}

	
	
		if (Application::IsKeyPressed('W') && pressDelay2 >= 1.f)
		{
			pressDelay2 = 0.f;
			soundStorage[5]->play3DSound(false, false, true, footPos);

		}
		if (Application::IsKeyPressed('A') && pressDelay2 >= 1.f)
		{
			pressDelay2 = 0.f;
			soundStorage[5]->play3DSound(false, false, true, footPos);

		}
		if (Application::IsKeyPressed('S') && pressDelay2 >= 1.f)
		{
			pressDelay2 = 0.f;
			soundStorage[5]->play3DSound(false, false, true, footPos);

		}
		
		if (Application::IsKeyPressed('D') && pressDelay2 >= 1.f)
		{
			pressDelay2 = 0.f;
			soundStorage[5]->play3DSound(false, false, true, footPos);
		}
		
	
	
		if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed('W') && pressDelay >= 0.5f)
		{
			pressDelay = 0.f;
			soundStorage[2]->play3DSound(false, false, true, footPos);
		}
		if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed('A') && pressDelay >= 0.5f)
		{
			pressDelay = 0.f;
			soundStorage[2]->play3DSound(false, false, true, footPos);
		}
		if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed('S') && pressDelay >= 0.5f)
		{
			pressDelay = 0.f;
			soundStorage[2]->play3DSound(false, false, true, footPos);
		}
		if (Application::IsKeyPressed(VK_SHIFT) && Application::IsKeyPressed('D') && pressDelay >= 0.5f)
		{
			pressDelay = 0.f;
			soundStorage[2]->play3DSound(false, false, true, footPos);
		}
	

	//soundStorage[2]->play3DSound(false, false, true, footPos);
	

	if (Application::IsKeyPressed(VK_SPACE) && (_elapsedTime >= nextJump))
	{
		nextJump = _elapsedTime + coolDown;
		soundStorage[4]->play3DSound(false, false, true, footPos);
	}

    //Skybox Rotation
    rotateSkybox += (float)(1 * rotateWorld * dt);

    //Skybox
    if (rotateSkybox > 360)
    {
        rotateSkybox = 0;
    }

    if (Application::IsKeyPressed(VK_NUMPAD5))
    {
        run = 3.f;
    }

    //Reset
    if (Application::IsKeyPressed('R'))
    {
        resetAll();
    }

    camera.Update(dt);

    //FPS Counter
    FPS = 1 / (float)dt;

    startTime++;

    enemyUpdate(dt);

    timeleft--;
}

void Level2::enemyUpdate(double dt)
{
    //-------This function is called in Level2::Update(double dt) to process Enemy interactions-----------------

    for (vector<Enemy*>::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
    {
        (*it)->update();

        
        for (vector<Object*>::iterator factoryIt = factoryIt = objFactory.Container.begin(); factoryIt != objFactory.Container.end(); factoryIt++)
        {
			if ((*factoryIt)->type == Scene::GEO_LIGHTBALL3)   //BULLET INTERACTION WITH ENEMY
            {
                if (((*factoryIt)->position_ - (*it)->_Position).Length() < 20 && bulletTouch == false)
                {
					if ((*it)->enemytype == 2)
					{
						(*it)->_Hp -= 10;
						(*it)->healthBarRobot1 -= 5.f;
						damaged = true;
						bulletTouch = true;


						(*it)->_Hp -= 10;
						(*it)->healthBarSpider -= 5.f;
						damaged = true;
						bulletTouch = true;

						if ((*it)->_Hp <= 0)
						{
							(*it)->setState(2);  //  death is at enum 2
						}
                    }
                }
            }
        }
    }
}
void Level2::Render()
{
    srand((unsigned)time(NULL));

    //Clear color & depth buffer every frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Mtx44 MVP;

    viewStack.LoadIdentity(); //No need camera for now, set it at World's origin
    viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
    modelStack.LoadIdentity();

    //Passing light position to shader
    Position lightPosition_cameraspace = viewStack.Top() * light[0].position;

    //Light 1
	//Light 1
	if (light[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}

    //Light 2
    if (light[1].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (light[1].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
        glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
        glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
    }

    //Light 2
    if (light[2].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[2].position.x, light[2].position.y, light[2].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (light[2].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
        glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[2].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT2_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[2].position;
        glUniform3fv(m_parameters[U_LIGHT2_POSITION], 1, &lightPosition_cameraspace.x);
    }


	if (light[3].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[3].position;
		glUniform3fv(m_parameters[U_LIGHT3_POSITION], 1, &lightPosition_cameraspace.x);
	}


	if (light[4].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[4].position;
		glUniform3fv(m_parameters[U_LIGHT4_POSITION], 1, &lightPosition_cameraspace.x);
	}


	if (light[5].type == Light::LIGHT_POINT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * light[5].position;
		glUniform3fv(m_parameters[U_LIGHT5_POSITION], 1, &lightPosition_cameraspace.x);
	}


    //Generate Axes
    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); //Remember, matrix multiplication is the other way around
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    RenderMesh(meshList[GEO_AXES], false);

    modelStack.PushMatrix();
    modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
    RenderSkybox();
    modelStack.PopMatrix();

    //Environment
    renderGround();
    renderElectricField();
    renderObjects();
    renderPosition();


    renderText();

    objFactory.renderFactoryObject();

    renderEnemy();

    renderSprites();

    renderHUD();

}


void Level2::renderEnemy()
{
    for (vector<Enemy *> ::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
    {
        if (((*it)->_Position - camera.getPosition()).Length() < 10 && (*it)->getState() != Spider::spiderState::death &&
            (*it)->getState() != Robot::robotState::death)
        {
            RenderMeshOnScreen(meshList[GEO_FLICKER], 40, 30, 80, 60, 0.f, 0.f, 0.f, 1.f);
            vec3df bloodStartingLocation = { camera.getPosition().x, camera.getPosition().y, camera.getPosition().z };
            if (_elapsedTime >= nextSplatter)
                soundStorage[1]->play3DSound(false, false, false, bloodStartingLocation);
            nextSplatter = _elapsedTime + coolDown;

            camera.health--;

			if ((*it)->enemytype == 2)
			{
                if (Math::RandIntMinMax(0, 100) <=50)
                {
                    camera.poison = true;
                }
            }
        }

        switch ((*it)->enemytype)
        {

        case   1:    // ----------------Spider ----------------------------
        {
                         if ((*it)->getState() != Spider::spiderState::death)
                         {
                             //Spider
                             modelStack.PushMatrix();
							 modelStack.Translate((*it)->_Position.x, (*it)->_Position.y - 30.f, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_SPIDER], true);
                             modelStack.PopMatrix();

                             //Health Bar
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y -5.f, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.Rotate(90, 0, 0, 1);
                             if ((*it)->healthBarSpider>0)
                                 modelStack.Scale(1, 1, (*it)->healthBarSpider);
                             else
                                 modelStack.Scale(1, 1, 1);
                             RenderMesh(meshList[GEO_ENEMYHEALTHBAR], true);
                             modelStack.PopMatrix();

                         }
        }
            break;
        case 2:  // ---------------Robot1 ----------------------
        {
                     if ((*it)->getState() != Robot::robotState::death)
                     {
                         if ((*it)->robotType == 1)
                         {
                             //Robot
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y - 30.f, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.PushMatrix();
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYTORSO], true);
                             RenderMesh(meshList[GEO_ENEMYHEAD], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 40, 0);
                             if (broughtDown == false)
                             {
                                 modelStack.Rotate(-rotateLeftArm, 0, 0, 1);
                             }
                             else if (rotateLeftArm == 0.f && broughtDown == true)
                             {
                                 modelStack.Rotate(-rotateIndividualArm, 0, 0, 1);
                             }
                             modelStack.Translate(0, -40, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTARM], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 40, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -40, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTARM], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 40, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -40, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTLEG], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 40, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -40, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTLEG], true);
                             modelStack.PopMatrix();
                             modelStack.PopMatrix();

                             //Health Bar
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y + +30.f, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.Rotate(90, 0, 0, 1);
                             if ((*it)->healthBarRobot1>0)
                                 modelStack.Scale(1, 1, (*it)->healthBarRobot1);
                             else
                                 modelStack.Scale(1, 1, 1);
                             RenderMesh(meshList[GEO_ENEMYHEALTHBAR], true);
                             modelStack.PopMatrix();
                         }

                         if ((*it)->robotType == 2)
                         {
                             //Robot 2
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.PushMatrix();
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYTORSO2], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTARM2], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTARM2], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 5, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -5, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTLEG2], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 5, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -5, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTLEG2], true);
                             modelStack.PopMatrix();
                             modelStack.PopMatrix();
                         }

                         if ((*it)->robotType == 3)
                         {
                             //Robot 3
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.PushMatrix();
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYTORSO3], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTARM3], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTARM3], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 1, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -1, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTLEG3], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 1, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -1, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTLEG3], true);
                             modelStack.PopMatrix();
                             modelStack.PopMatrix();
                         }
                         if ((*it)->robotType == 4)
                         {
                             //Robot 4
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.PushMatrix();
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYTORSO4], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTARM4], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTARM4], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(-rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYRIGHTLEG4], true);
                             modelStack.PopMatrix();
                             modelStack.PushMatrix();
                             modelStack.Translate(0, 10, 0);
                             modelStack.Rotate(rotateArm, 0, 0, 1);
                             modelStack.Translate(0, -10, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_ENEMYLEFTLEG4], true);
                             modelStack.PopMatrix();
                             modelStack.PopMatrix();
                         }
                     }
        }
            break;
        }
    }
}

void Level2::renderHUD()
{
    if (inventoryOpen)
    {
        if (startTime >= cooldown)
        {
            if (Application::IsKeyPressed(VK_DOWN)) // scroll down item list
            {
                level2_inventory->CycleDisplay(0);
                startTime = 0;
            }
            else if (Application::IsKeyPressed(VK_UP)) // scroll up item list
            {
                level2_inventory->CycleDisplay(1);
                startTime = 0;
            }
            else if (Application::IsKeyPressed('J')) // equip primary
            {
                level2_inventory->equipItem(1);
                startTime = 0;
            }
            else if (Application::IsKeyPressed('K')) // equip secondary
            {
                level2_inventory->equipItem(0);
                startTime = 0;
            }
        }
        RenderMeshOnScreen(meshList[GEO_INVENTORY], 40, 27, 80, 65, 0.f, 0.f, 0.f, 1.f);
        renderInventory();
        renderStats();
    }
    else
    {
        //in game hud
        RenderMeshOnScreen(meshList[GEO_GAME_HUD], 40, 32, 80, 65, 0.f, 0.f, 0.f, 1.f);

        //Health bar
        if (camera.health > 0)
        {
			RenderMeshOnScreen(meshList[GEO_HP], static_cast<float> (5 + 11.3 * camera.health / 500)
				, 6.1f, static_cast<float>( 105 * camera.health / 500), 9, 0.f, 0.f, 0.f, 1.f);
        }

        //Antidote 
        if (camera.poisonText == true && camera.poison == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "You have been poisoned", Color(1, 0, 1), 4, 5, 8);
        if (usedAntidoteText == true && notPoisoned == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "You are not poisoned", Color(1, 0, 1), 4, 5, 8);
        if (usedAntidoteText == true && antidoteUsed == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "Antidote used", Color(1, 0, 1), 4, 7, 8);
        if (usedAntidoteText == true && noAntidote == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "No antidotes in inventory", Color(1, 0, 1), 4, 5, 7);

        //Health Potion
        /*if (camera.potionText == true && camera.potion == true)
        RenderTextOnScreen(meshList[GEO_TEXT], "You have been poisoned", Color(1, 0, 1), 4, 5, 8);*/
        if (usedPotionText == true && notHurt == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "You are not hurt", Color(1, 0, 1), 4, 5, 10);
        if (usedPotionText == true && potionUsed == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "Potion used", Color(1, 0, 1), 4, 7, 10);
        if (usedPotionText == true && noPotion == true)
            RenderTextOnScreen(meshList[GEO_TEXT], "No potions in inventory", Color(1, 0, 1), 4, 5, 9);

        //Energy level of torch
        if (energy > 0)
        {
			RenderMeshOnScreen(meshList[GEO_EN], static_cast<float>(5 + 11.3* energy / 1000), 2.3f, 
				static_cast<float>(105 * energy / 1000), 9, 0.f, 0.f, 0.f, 1.f);
        }
        if (energy == 0)
        {
            torchDead = true;
        }
        if (timeleft > 0)
        {
			RenderMeshOnScreen(meshList[GEO_TIME], static_cast<float>(58.6 + 11.3* timeleft / 10800),
				14.5, static_cast<float>(105 * timeleft / 10800), 11, 0.f, 0.f, 0.f, 1.f);
        }
        renderRemainingTime();
        //minimap
        RenderMeshOnScreen(meshList[GEO_MINI_GROUND], 10, 50, 15, 15, 0.f, 0.f, 0.f, 1.f);
		RenderMeshOnScreen(meshList[GEO_MINI_PLAYER], static_cast<float>(10.5 + ((camera.getPosition().x / 10000) * 14))
			, static_cast<float>( 50 + ((camera.getPosition().z / 10000)* 14.4)), 6, 6, 0.f, 0.f, 0.f, 1.f);
        for (vector<Enemy*>::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
        {
            if ((*it)->getState() != Robot::robotState::death || (*it)->getState() != Spider::spiderState::death)
				RenderMeshOnScreen(meshList[GEO_MINI_ENEMY], static_cast<float>(10.5 + (((*it)->_Position.x / 3200) * 14))
				, static_cast<float>(50 + (((*it)->_Position.z / 3200) * 14.4)), 10, 10, 0.f, 0.f, 0.f, 1.f);
        }
    }
}

void Level2::renderInventory()
{
    if (!level2_inventory->pointer)
    {
        RenderMeshOnScreen(meshList[GEO_ARROW], 20, 20, 27, 30, 0.f, 0.f, 0.f, 1.f);
    }
    else
    {
        RenderMeshOnScreen(meshList[GEO_ARROW], 20, 10, 27, 30, 0.f, 0.f, 0.f, 1.f);
    }

    ItemInfo* activeItem = level2_inventory->getActiveItem();
    if (activeItem->gettype() == "gun")
    {
        RenderMeshOnScreen(meshList[GEO_GUNI], 15, 35, 130, 130, 0.f, 0.f, 0.f, 1.f);
    }
    else if (activeItem->gettype() == "fist")
    {
        RenderMeshOnScreen(meshList[GEO_HANDI], 15, 34, 50, 50, 0.f, 0.f, 0.f, 1.f);
    }
    else if (activeItem->gettype() == "torch")
    {
        RenderMeshOnScreen(meshList[GEO_TORCHI], 15, 35, 150, 150, 0.f, 0.f, 0.f, 1.f);
    }
    ItemInfo* secondaryItem = level2_inventory->getSecondaryItem();
    if (secondaryItem->gettype() == "gun")
    {
        RenderMeshOnScreen(meshList[GEO_GUNI], 33, 35, 130, 130, 0.f, 0.f, 0.f, 1.f);
    }
    else if (secondaryItem->gettype() == "fist")
    {
        RenderMeshOnScreen(meshList[GEO_HANDI], 32.5, 35, 50, 50, 0.f, 0.f, 0.f, 1.f);
    }
    else if (secondaryItem->gettype() == "torch")
    {
        RenderMeshOnScreen(meshList[GEO_TORCHI], 33, 35, 150, 150, 0.f, 0.f, 0.f, 1.f);
    }

    ItemInfo* ItemDisplay1 = level2_inventory->getDisplay1();
    if (ItemDisplay1->gettype() == "gun")
    {
        RenderMeshOnScreen(meshList[GEO_GUNI], 12, 23, 60, 60, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay1->gettype() == "fist")
    {
        RenderMeshOnScreen(meshList[GEO_HANDI], 10, 40, 5, 5, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay1->gettype() == "torch")
    {
        RenderMeshOnScreen(meshList[GEO_TORCHI], 12, 23, 70, 70, 0.f, 0.f, 0.f, 1.f);
    }
    if (ItemDisplay1 == activeItem)
    {
		RenderMeshOnScreen(meshList[GEO_ACTIVE_SELECT], 12, 23.2f, 6, 7.2f, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay1 == secondaryItem)
    {
        RenderMeshOnScreen(meshList[GEO_SECONDARY_SELECT], 12, 23.2f, 6, 7.2f, 0.f, 0.f, 0.f, 1.f);
    }
    ItemInfo* ItemDisplay2 = level2_inventory->getDisplay2();
    if (ItemDisplay2->gettype() == "gun")
    {
        RenderMeshOnScreen(meshList[GEO_GUNI], 10, 10, 50, 50, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay2->gettype() == "fist")
    {
        RenderMeshOnScreen(meshList[GEO_HANDI], 12, 13, 25, 25, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay2->gettype() == "torch")
    {
        RenderMeshOnScreen(meshList[GEO_TORCHI], 12, 13, 70, 70, 0.f, 0.f, 0.f, 1.f);
    }
    if (ItemDisplay2 == activeItem)
    {
        RenderMeshOnScreen(meshList[GEO_ACTIVE_SELECT], 12, 13.7f, 6, 7.2f, 0.f, 0.f, 0.f, 1.f);
    }
    else if (ItemDisplay2 == secondaryItem)
    {
        RenderMeshOnScreen(meshList[GEO_SECONDARY_SELECT], 12, 13.7f, 6, 7.2f, 0.f, 0.f, 0.f, 1.f);
    }

    //Potions
    RenderMeshOnScreen(meshList[GEO_HEALTHPOTION], 29, 23.5, 100, 100, 0.f, 0.f, 0.f, 1.f);
    renderPotionCount();
    RenderMeshOnScreen(meshList[GEO_ANTIDOTE], 29, 14, 100, 100, 0.f, 0.f, 0.f, 1.f);
    renderAntidoteCount();
}

void Level2::renderElectricField()
{
    if (camera.position.x > -130 && camera.position.x < -70 && camera.position.z > 300 && camera.position.z < 360)
    {
        energy += 3;
    }
    modelStack.PushMatrix();
    modelStack.Translate(-100, -30, 300);
    modelStack.Rotate(90.f, 1, 0, 0);
    modelStack.Scale(10.f, 10.f, 10.f);
    RenderMesh(meshList[GEO_ELECTRIC_FIELD], true);
    modelStack.PopMatrix();
}

void Level2::RenderSkybox()
{
    //Day
    if (cycle == false)
    {
        modelStack.PushMatrix();
        modelStack.Rotate(rotateSkybox, 0.f, 1.f, 0.f);

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 0.f, 998.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_FRONT], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 0.f, -998.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_BACK], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 998.f, 0.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_TOP], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, -998.f, 0.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_BOTTOM], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(998.f, 0.f, 0.f);
        modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_LEFT], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(-998.f, 0.f, 0.f);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_RIGHT], false);
        modelStack.PopMatrix();

        modelStack.PopMatrix();
    }

    //Night
    if (cycle == true)
    {
        modelStack.PushMatrix();
        modelStack.Rotate(rotateSkybox, 0.f, 1.f, 0.f);

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 0.f, 998.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_FRONTn], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 0.f, -998.f);
        //modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_BACKn], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, 998.f, 0.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_TOPn], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(0.f, -998.f, 0.f);
        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_BOTTOMn], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(998.f, 0.f, 0.f);
        modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_LEFTn], false);
        modelStack.PopMatrix();

        modelStack.PushMatrix();
        //scale, translate, rotate
        modelStack.Translate(-998.f, 0.f, 0.f);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
        modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

        modelStack.Scale(2000.f, 2000.f, 2000.f);
        RenderMesh(meshList[GEO_RIGHTn], false);
        modelStack.PopMatrix();

        modelStack.PopMatrix();
    }
}


void Level2::renderGround()
{
    modelStack.PushMatrix();
    modelStack.Translate(0.f, -30.f, 0.f);
    modelStack.Scale(3200.f, 3200.f, 3200.f);
    RenderMesh(meshList[GEO_GROUND], true);
    modelStack.PopMatrix();
}

void Level2::renderSprites()
{
    //Gun
    //RenderMeshOnScreen(meshList[GEO_HANDL1], 15, 5, 100, 100, 0, 0, 0, 1);
    //if (swing == false)
    //{
    //	RenderMeshOnScreen(meshList[GEO_GUN1], 65, 9, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime < 15)
    //{
    //	RenderMeshOnScreen(meshList[GEO_GUN2], 65, 12, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime > 15 && swingTime < 30)
    //{
    //	RenderMeshOnScreen(meshList[GEO_GUN3], 65, 11, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime > 30)
    //{
    //	swing = false;
    //	swingTime = 0;
    //}

    //Cannon
    //if (swing == false)
    //{
    //RenderMeshOnScreen(meshList[GEO_CANNON1], 50, 11, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime < 20)
    //{
    //RenderMeshOnScreen(meshList[GEO_CANNON2], 50, 13, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime > 20 && swingTime < 40)
    //{
    //RenderMeshOnScreen(meshList[GEO_CANNON3], 50, 12, 100, 100, 0, 0, 0, 1);
    //}
    //if (swing == true && swingTime > 40)
    //{
    //	swing = false;
    //	swingTime = 0;
    //}

    if (level2_inventory->getActiveItem()->gettype() == "fist" || level2_inventory->getSecondaryItem()->gettype() == "fist")
    {
        if (swing == false)
        {
            //RenderMeshOnScreen(meshList[GEO_HANDL1], 15, 5, 100, 100);
            RenderMeshOnScreen(meshList[GEO_HANDR1], 65, 5, 100, 100, 0.f, 0.f, 0.f, 1.f);
        }
        //Punching hands
        if (swing == true)
        {
            //RenderMeshOnScreen(meshList[GEO_HANDL2], 15, 10, 100, 100);
            RenderMeshOnScreen(meshList[GEO_HANDR2], 65, 10, 100, 100, 0.f, 0.f, 0.f, 1.f);
            if (swingTime > 40)
            {
                swing = false;
                swingTime = 0;
            }
        }
    }
    else if (level2_inventory->getActiveItem()->gettype() == "gun" || level2_inventory->getSecondaryItem()->gettype() == "gun")
    {
        if (swing == false)
        {
            RenderMeshOnScreen(meshList[GEO_GUN1], 55, 10, 100, 100, 0.f, 0.f, 0.f, 1.f);
        }
        if (swing == true && swingTime < 15)
        {
            RenderMeshOnScreen(meshList[GEO_GUN2], 55, 10, 100, 100, 0.f, 0.f, 0.f, 1.f);
        }
        if (swing == true && swingTime > 15 && swingTime < 30)
        {
            RenderMeshOnScreen(meshList[GEO_GUN3], 55, 10, 100, 100, 0.f, 0.f, 0.f, 1.f);
        }
        if (swing == true && swingTime > 30)
        {
            swing = false;
            swingTime = 0;
        }
    }
    else if (level2_inventory->getActiveItem()->gettype() == "torch" || level2_inventory->getSecondaryItem()->gettype() == "torch")
        RenderMeshOnScreen(meshList[GEO_TORCH], 25, 5, 150, 150, 0.f, 0.f, 0.f, 1.f);
}


void Level2::renderPosition()
{
    std::string sX = "X: " + std::to_string((int)camera.position.x),
        sY = "Y: " + std::to_string((int)camera.position.y),
        sZ = "Z: " + std::to_string((int)camera.position.z);

    modelStack.PushMatrix();
    //No transform needed
    RenderTextOnScreen(meshList[GEO_X], sX, Color(0, 1, 0), 3, 23, 19);
    RenderTextOnScreen(meshList[GEO_Y], sY, Color(0, 1, 0), 3, 23, 18);
    RenderTextOnScreen(meshList[GEO_Z], sZ, Color(0, 1, 0), 3, 23, 17);

    modelStack.PopMatrix();
}


void Level2::renderText()
{
    std::string sFPS = "FPS: " + std::to_string(FPS);
    sFPS.resize(9);
    RenderTextOnScreen(meshList[GEO_FPS], sFPS, Color(1, 1, 0), 2, 0, 29);
}

void Level2::renderRemainingTime()
{

    std::string rTime = "";
    int tempTime = timeleft;
    int seconds = 0;
    int minutes = 0;
    while (tempTime >= 60)
    {
        tempTime -= 60;
        seconds += 1;
    }
    while (seconds > 60)
    {
        seconds -= 60;
        minutes += 1;
    }
    if (seconds > 9)
    {
        rTime = std::to_string(minutes) + ":" + std::to_string(seconds);
    }
    else
    {
        rTime = std::to_string(minutes) + ":0" + std::to_string(seconds);
    }
    RenderTextOnScreen(meshList[GEO_COUNTDOWN], rTime, Color(1, 0, 0), 2, 30, 7);
}

void Level2::renderPotionCount()
{
    std::string count = "";
    count = std::to_string(potionCount);
    RenderTextOnScreen(meshList[GEO_COUNTDOWN], count, Color(1, 0, 0), 2, 15.5, 10);
}

void Level2::renderAntidoteCount()
{
    std::string count = "";
    count = std::to_string(antidoteCount);
    RenderTextOnScreen(meshList[GEO_COUNTDOWN], count, Color(1, 0, 0), 2, 15.5, 5);
}

void Level2::renderStats()
{
    /*ItemInfo* active = level2_inventory->getActiveItem();
    string itemName = active->gettype();
    RenderTextOnScreen(meshList[GEO_FPS], itemName, Color(0,0,0), 2, 25, 20);*/
}

void Level2::RenderMesh(Mesh *mesh, bool enableLight) {
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
    if (enableLight) {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }

    if (mesh->textureID > 0) {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }

    mesh->Render(); //this line should only be called once

    if (mesh->textureID > 0) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

}

void Level2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
    {
        return;
    }

    glDisable(GL_DEPTH_TEST);

    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode
    modelStack.PushMatrix();
    modelStack.LoadIdentity(); //Reset modelStack
    modelStack.Scale(size, size, size);
    modelStack.Translate(x, y, 0);

    glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
    glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
    glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mesh->textureID);
    glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    for (unsigned i = 0; i < text.length(); ++i)
    {
        Mtx44 characterSpacing;
        characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //0.5f beside i is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

    //Add these code just before glEnable(GL_DEPTH_TEST);
    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();


    glEnable(GL_DEPTH_TEST);
}

void Level2::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float rotateAngle, float xAxis, float yAxis, float zAxis)
{
    glDisable(GL_DEPTH_TEST);
    Mtx44 ortho;
    ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
    projectionStack.PushMatrix();
    projectionStack.LoadMatrix(ortho);
    viewStack.PushMatrix();
    viewStack.LoadIdentity(); //No need camera for ortho mode

    modelStack.PushMatrix();
    modelStack.LoadIdentity();
    modelStack.Translate((float)x, (float)y, 0);
    modelStack.Rotate(rotateAngle, xAxis, yAxis, zAxis);
    modelStack.Scale((float)sizex, (float)sizey, 1);
    //to do: scale and translate accordingly
    RenderMesh(mesh, false); //UI should not have light
    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void Level2::resetAll()
{
    camera.position.Set(0.f, 0.f, 484.f);
}

void Level2::Exit()
{
    // Cleanup VBO here
    for (int i = 0; i < NUM_GEOMETRY; ++i)
    {
        if (meshList[i] != NULL)
        {
            delete meshList[i];
        }
    }
    glDeleteVertexArrays(1, &m_vertexArrayID);

    glDeleteProgram(m_programID);
}
