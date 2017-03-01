#include "SceneEditor.h"
#include "Bullet.h"
#include "Robot.h"
#include "Spider.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"

#include "shader.hpp"
#include "Utility.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>

#include "ItemInfo.h"
#include "Inventory.h"

using std::cout;
using std::cin;

static string choosenLevel = "";
static Vector3 targetFromCamera = { 0, 0, 0 };
static Vector3 positionFromCamera = { 0, 0, 0 };
static Vector3 viewFromCamera = { 0, 0, 0 };
static float pitchFromCamera = 0.f;
static float yawFromCamera = 0.f;


SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

//Change to negative X when enemy collided

void SceneEditor::save()
{
    ofstream outputFile;
    outputFile.open(choosenLevel/*, ios::app*/);
    unsigned bindVector = 0;
    bool nextStep = false, nextNextStep = false;

    for (vector<string>::iterator objIt = objectName.begin(); objIt != objectName.end(); objIt++)
    {
        for (vector<string>::iterator textIt = objectTexture.begin() + bindVector; textIt != objectTexture.end() && nextStep == false; textIt++)
        {
            nextStep = true;
            for (vector<string>::iterator xIt = objectPosX.begin() + bindVector; xIt != objectPosX.end() && nextNextStep == false; xIt++)
            {
                nextNextStep = true;
                for (vector<string>::iterator zIt = objectPosZ.begin() + bindVector; zIt != objectPosZ.end(); zIt++)
                {
                    outputFile << "--Object--" << endl;
                    outputFile << xIt->data() << "," << zIt->data() << endl;
                    outputFile << textIt->data() << endl;
                    outputFile << objIt->data() << endl;
                    outputFile << "--Object End--" << endl;
                    bindVector++;

                    break;
                }
                break;
            }
            break;
        }
        nextStep = false;
        nextNextStep = false;
    }

    bool speed = false,
        damage = false,
        hp = false,
        range = false,
        state = false,
        numPoint = false,
        way = false,
        x = false,
        z = false;

    //Check if enemy is robot or spider.
    const string robot = "Robot";

    unsigned bind = 0;
    string sAmountOfPoints;
    int amountOfPoints = 0;
    unsigned pairCoordiantesWithEnemy = 0;


    for (vector<string>::iterator typeIt = enemyType.begin(); typeIt != enemyType.end(); typeIt++)
    {
        outputFile << "--Enemy--" << endl;
        for (vector<string>::iterator speedIt = enemySpeed.begin() + bind; speedIt != enemySpeed.end() && speed == false; speedIt++)
        {
            speed = true;
            for (vector<string>::iterator damageIt = enemyDamage.begin() + bind; damageIt != enemyDamage.end() && damage == false; damageIt++)
            {
                damage = true;
                for (vector<string>::iterator hpIt = enemyHP.begin() + bind; hpIt != enemyHP.end() && hp == false; hpIt++)
                {
                    hp = true;
                    for (vector<string>::iterator rangeIt = enemyRange.begin() + bind; rangeIt != enemyRange.end() && range == false; rangeIt++)
                    {
                        range = true;

                        for (vector<string>::iterator stateIt = enemyState.begin() + bind; stateIt != enemyState.end() && state == false; stateIt++)
                        {
                            outputFile << typeIt->data() << "," <<
                                speedIt->data() << "," <<
                                damageIt->data() << "," <<
                                hpIt->data() << "," <<
                                rangeIt->data() << "," <<
                                stateIt->data() << endl;
                            state = true;

                            for (vector<string>::iterator numPointIt = numOfWaypoints.begin() + bind;
                                numPointIt != numOfWaypoints.end() &&
                                numPoint == false; numPointIt++)
                            {
                                sAmountOfPoints = numPointIt->data();
                                amountOfPoints = stoi(sAmountOfPoints);

                                outputFile << amountOfPoints << endl;
                                numPoint = true;

                            }

                            for (unsigned i = 0; i < amountOfPoints; i++)
                            {
                                for (vector<string>::iterator xIt = enemyX.begin() + pairCoordiantesWithEnemy; xIt != enemyX.end() && x == false; xIt++)
                                {
                                    x = true;
                                    for (vector<string>::iterator zIt = enemyZ.begin() + pairCoordiantesWithEnemy; zIt != enemyZ.end() && z == false; zIt++)
                                    {
                                        outputFile << xIt->data() << "," << zIt->data() << endl;
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
        speed = false;
        damage = false;
        hp = false;
        range = false;
        state = false;
        numPoint = false;
        bind++;
        outputFile << "--Enemy End--" << endl;
    }
    outputFile.close();
    
}

void SceneEditor::loadConfig()
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
    ifstream myfile(choosenLevel);

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

        //++numOfObjects;

        //if (linePos < 3)
        //{
        //    linePos++;
        //}
        //else
        //{
        //    linePos = 1;
        //}
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
                //cout << "Waypoints: " << waypoints << endl;
            }

            else if (linePos > 3 && linePos < (4 + waypoints))
            {
                x = false;
                posX = "";
                posZ = "";
                //string pos, posX, posZ;
                //bool x = false;
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
//        //Enemy
//        //Speed, Damage, HP, Range and Type of Robot
//        //Enemy * newEnemy2 = new Robot(70.f, 5.f, 100.f, 100.f, 1);
//        //newEnemy2->addWaypoint(Vector3(300, 0, -100));
//        //newEnemy2->addWaypoint(Vector3(400, 0, -400));
//        //newEnemy2->addWaypoint(Vector3(-400, 0, -400));
//        //newEnemy2->addWaypoint(Vector3(-400, 0, 400));
//        //enemyStorage.push_back(newEnemy2);
//    }
///*
//    cout << enemyType.back() << endl;
//    cout << enemySpeed.back() << endl;
//    cout << enemyDamage.back() << endl;
//    cout << enemyHP.back() << endl;
//    cout << enemyRange.back() << endl;
//    cout << enemyState.back() << endl;
//    cout << numOfWaypoints.back() << endl;
//
//    enemyType.pop_back();
//        enemySpeed.pop_back(); 
//        enemyDamage.pop_back();
//        enemyHP.pop_back();
//        enemyRange.pop_back(); 
//        enemyState.pop_back(); 
//        numOfWaypoints.pop_back();
//
//    cout << enemyType.back() << endl;
//    cout << enemySpeed.back() << endl;
//    cout << enemyDamage.back() << endl;
//    cout << enemyHP.back() << endl;
//    cout << enemyRange.back() << endl;
//    cout << enemyState.back() << endl;
//    cout << numOfWaypoints.back() << endl*/;
//
//    unsigned bind = 0;
//    bool nextStep = false;
//    for (vector<string>::iterator xIt = enemyX.begin(); xIt != enemyX.end(); xIt++)
//    {
//        for (vector<string>::iterator zIt = enemyZ.begin() + bind; zIt != enemyZ.end() && nextStep == false; zIt++)
//        {
//            //cout << xIt->data() << "," << zIt->data() << endl;
//            nextStep = true;
//        }
//        bind++;
//        nextStep = false;
//    }
//
//    numOfObjects /= 3;
//    numOfObjects--;
}

void SceneEditor::initEnemy()
{
    //Enemy
    //Speed, Damage, HP, Range and State of Robot
    //Enemy * newEnemy2 = new Robot(70.f, 5.f, 100.f, 100.f, 1);
    //newEnemy2->addWaypoint(Vector3(300, 0, -100));
    //newEnemy2->addWaypoint(Vector3(400, 0, -400));
    //newEnemy2->addWaypoint(Vector3(-400, 0, -400));
    //newEnemy2->addWaypoint(Vector3(-400, 0, 400));
    //enemyStorage.push_back(newEnemy2);
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

        for (vector<string>::iterator speedIt = enemySpeed.begin() + bind; speedIt != enemySpeed.end() && speed == false; speedIt++)
        {

            speed = true;
            for (vector<string>::iterator damageIt = enemyDamage.begin() + bind; damageIt != enemyDamage.end() && damage == false; damageIt++)
            {

                damage = true;
                for (vector<string>::iterator hpIt = enemyHP.begin() + bind; hpIt != enemyHP.end() && hp == false; hpIt++)
                {
 
                    hp = true;
                    for (vector<string>::iterator rangeIt = enemyRange.begin() + bind; rangeIt != enemyRange.end() && range == false; rangeIt++)
                    {
           
                        range = true;

                        for (vector<string>::iterator stateIt = enemyState.begin() + bind; stateIt != enemyState.end() && state == false; stateIt++)
                        {
    

      
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

void SceneEditor::Init()
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

    for (int i = 0; i < NUM_GEOMETRY; i++)
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
    //while (objectName.size() != 0)
    //{ 
    //    meshListPredefined[generateMesh] = MeshBuilder::GenerateOBJ(objectName.back(), objectName.back());
    //    objectName.pop_back();
    //    meshListPredefined[generateMesh]->textureID = LoadTGA(objectTexture.back());
    //    objectTexture.pop_back();
    //    generateMesh++;
    //}

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);

    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 20, 20, 1);
    meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball_2", Color(0.5f, 0.f, 0.f), 20, 20, 1);
    meshList[GEO_LIGHTBALL3] = MeshBuilder::GenerateSphere("lightball_3", Color(1.f, 1.f, 0.f), 20, 20, 1);

    meshList[GEO_MOUNTAIN] = MeshBuilder::GenerateOBJ("mountain", "OBJ//mountain.obj");
    meshList[GEO_MOUNTAIN]->textureID = LoadTGA("Image//objects.tga");

    meshList[GEO_GROUND] = MeshBuilder::GenerateQuad("grass", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_GROUND]->textureID = LoadTGA("Image//grass.tga");

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
    //
    //
    //    ////Text
    //    //meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    //    //meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_X] = MeshBuilder::GenerateText("x", 16, 16);
    meshList[GEO_X]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_Y] = MeshBuilder::GenerateText("y", 16, 16);
    meshList[GEO_Y]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_Z] = MeshBuilder::GenerateText("z", 16, 16);
    meshList[GEO_Z]->textureID = LoadTGA("Image//comic.tga");

    meshList[GEO_FPS] = MeshBuilder::GenerateText("FPS", 16, 16);
    meshList[GEO_FPS]->textureID = LoadTGA("Image//comic.tga");


    //    //meshList[GEO_MESSAGES] = MeshBuilder::GenerateText("messages", 16, 16);
    //    //meshList[GEO_MESSAGES]->textureID = LoadTGA("Image//comic.tga");
    //
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

    //Sprites
    meshList[GEO_HANDL1] = MeshBuilder::GenerateOBJ("handL1", "OBJ//quad.obj");
    meshList[GEO_HANDL1]->textureID = LoadTGA("Image//Sprites//HandL1.tga");
    meshList[GEO_HANDR1] = MeshBuilder::GenerateOBJ("handR1", "OBJ//quad.obj");
    meshList[GEO_HANDR1]->textureID = LoadTGA("Image//Sprites//HandR1.tga");
    meshList[GEO_HANDL2] = MeshBuilder::GenerateOBJ("handL2", "OBJ//quad.obj");
    meshList[GEO_HANDL2]->textureID = LoadTGA("Image//Sprites//HandL2.tga");
    meshList[GEO_HANDR2] = MeshBuilder::GenerateOBJ("handR2", "OBJ//quad.obj");
    meshList[GEO_HANDR2]->textureID = LoadTGA("Image//Sprites//HandR2.tga");
    meshList[GEO_GUN1] = MeshBuilder::GenerateOBJ("gun1", "OBJ//quad.obj");
    meshList[GEO_GUN1]->textureID = LoadTGA("Image//Sprites//Gun1.tga");
    meshList[GEO_GUN2] = MeshBuilder::GenerateOBJ("gun2", "OBJ//quad.obj");
    meshList[GEO_GUN2]->textureID = LoadTGA("Image//Sprites//Gun2.tga");
    meshList[GEO_GUN3] = MeshBuilder::GenerateOBJ("gun3", "OBJ//quad.obj");
    meshList[GEO_GUN3]->textureID = LoadTGA("Image//Sprites//Gun3.tga");
    meshList[GEO_CANNON1] = MeshBuilder::GenerateOBJ("cannon1", "OBJ//quad.obj");
    meshList[GEO_CANNON1]->textureID = LoadTGA("Image//Sprites//Cannon1.tga");
    meshList[GEO_CANNON2] = MeshBuilder::GenerateOBJ("cannon2", "OBJ//quad.obj");
    meshList[GEO_CANNON2]->textureID = LoadTGA("Image//Sprites//Cannon2.tga");
    meshList[GEO_CANNON3] = MeshBuilder::GenerateOBJ("cannon3", "OBJ//quad.obj");
    meshList[GEO_CANNON3]->textureID = LoadTGA("Image//Sprites//Cannon3.tga");
    meshList[GEO_SWORD1] = MeshBuilder::GenerateOBJ("sword1", "OBJ//quad.obj");
    meshList[GEO_SWORD1]->textureID = LoadTGA("Image//Sprites//Sword1.tga");
    meshList[GEO_SWORD2] = MeshBuilder::GenerateOBJ("sword2", "OBJ//quad.obj");
    meshList[GEO_SWORD2]->textureID = LoadTGA("Image//Sprites//Sword2.tga");
    meshList[GEO_SWORD3] = MeshBuilder::GenerateOBJ("sword3", "OBJ//quad.obj");
    meshList[GEO_SWORD3]->textureID = LoadTGA("Image//Sprites//Sword3.tga");

    //Item
    meshList[GEO_SWORD] = MeshBuilder::GenerateOBJ("Sword", "OBJ//Sword.obj");
    meshList[GEO_SWORD]->textureID = LoadTGA("Image//Sword.tga");

    meshList[GEO_TORCH] = MeshBuilder::GenerateOBJ("Torch", "OBJ//Torch.obj");
    meshList[GEO_TORCH]->textureID = LoadTGA("Image//Torch.tga");

    meshList[GEO_INVENTORY] = MeshBuilder::GenerateOBJ("Inventory", "OBJ//inventory.obj");
    meshList[GEO_INVENTORY]->textureID = LoadTGA("Image//inventorytest.tga");

    meshList[GEO_MINI_PLAYER] = MeshBuilder::GenerateOBJ("mini_player", "OBJ//inventory.obj");
    meshList[GEO_MINI_PLAYER]->textureID = LoadTGA("Image//mini_player.tga");

    meshList[GEO_MINI_GROUND] = MeshBuilder::GenerateOBJ("mini_ground", "OBJ//inventory.obj");
    meshList[GEO_MINI_GROUND]->textureID = LoadTGA("Image//grass.tga");

    meshList[GEO_MINI_ENEMY] = MeshBuilder::GenerateOBJ("mini_enemy", "OBJ//inventory.obj");
    meshList[GEO_MINI_ENEMY]->textureID = LoadTGA("Image//mini_enemy.tga");

    meshList[GEO_GAME_HUD] = MeshBuilder::GenerateOBJ("game_hud", "OBJ//inventory.obj");
    meshList[GEO_GAME_HUD]->textureID = LoadTGA("Image//game_hud.tga");

    meshList[GEO_HP] = MeshBuilder::GenerateOBJ("game_hp", "OBJ//inventory.obj");
    meshList[GEO_HP]->textureID = LoadTGA("Image//hp_bar.tga");

    //Pause Menu
    meshList[PAUSE] = MeshBuilder::GenerateOBJ("splash", "OBJ//menu.obj");
    meshList[PAUSE]->textureID = LoadTGA("Image//menu/pause.tga");

    //Arrow
    meshList[ARROW] = MeshBuilder::GenerateOBJ("arrow", "OBJ//menu.obj");
    meshList[ARROW]->textureID = LoadTGA("Image//menu/arrow.tga");

    meshList[GEO_MOUNTAIN] = MeshBuilder::GenerateOBJ("mountain", "OBJ//mountain.obj");
    meshList[GEO_MOUNTAIN]->textureID = LoadTGA("Image//objects.tga");

    meshList[GEO_LAMP] = MeshBuilder::GenerateOBJ("lamp", "OBJ//lamp.obj");
    meshList[GEO_LAMP]->textureID = LoadTGA("Image//lamp.tga");

    meshList[GEO_LANTERN] = MeshBuilder::GenerateOBJ("lantern", "OBJ//lantern.obj");
    meshList[GEO_LANTERN]->textureID = LoadTGA("Image//lantern.tga");

    meshList[GEO_TOMBSTONE] = MeshBuilder::GenerateOBJ("tombstone", "OBJ//tombstone.obj");
    meshList[GEO_TOMBSTONE]->textureID = LoadTGA("Image//tombstone.tga");

    meshList[GEO_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
    meshList[GEO_TREE]->textureID = LoadTGA("Image//tree.tga");

    meshList[GEO_STATUE1] = MeshBuilder::GenerateOBJ("statue1", "OBJ//statue1.obj");
    meshList[GEO_STATUE1]->textureID = LoadTGA("Image//statue1.tga");

    meshList[GEO_STATUE2] = MeshBuilder::GenerateOBJ("statue2", "OBJ//statue2.obj");
    meshList[GEO_STATUE2]->textureID = LoadTGA("Image//statue2.tga");

    //Collided Objects
    meshList[COLLIDED_MOUNTAIN] = MeshBuilder::GenerateOBJ("mountain", "OBJ//mountain.obj");
    meshList[COLLIDED_MOUNTAIN]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_LAMP] = MeshBuilder::GenerateOBJ("lamp", "OBJ//lamp.obj");
    meshList[COLLIDED_LAMP]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_LANTERN] = MeshBuilder::GenerateOBJ("lantern", "OBJ//lantern.obj");
    meshList[COLLIDED_LANTERN]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_TOMBSTONE] = MeshBuilder::GenerateOBJ("tombstone", "OBJ//tombstone.obj");
    meshList[COLLIDED_TOMBSTONE]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_TREE] = MeshBuilder::GenerateOBJ("tree", "OBJ//tree.obj");
    meshList[COLLIDED_TREE]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_STATUE1] = MeshBuilder::GenerateOBJ("statue1", "OBJ//statue1.obj");
    meshList[COLLIDED_STATUE1]->textureID = LoadTGA("Image//red.tga");

    meshList[COLLIDED_STATUE2] = MeshBuilder::GenerateOBJ("statue2", "OBJ//statue2.obj");
    meshList[COLLIDED_STATUE2]->textureID = LoadTGA("Image//red.tga");

    meshList[LEVELEDITOR] = MeshBuilder::GenerateOBJ("leveleditor", "OBJ//menu.obj");
    meshList[LEVELEDITOR]->textureID = LoadTGA("Image//menu/leveleditor.tga");

    meshList[SELECTION] = MeshBuilder::GenerateOBJ("selection", "OBJ//menu.obj");
    meshList[SELECTION]->textureID = LoadTGA("Image//menu/selection.tga");

    meshList[COLLISIONON] = MeshBuilder::GenerateOBJ("collisionon", "OBJ//menu.obj");
    meshList[COLLISIONON]->textureID = LoadTGA("Image//menu/collisionon.tga");

    meshList[COLLISIONOFF] = MeshBuilder::GenerateOBJ("collisionoff", "OBJ//menu.obj");
    meshList[COLLISIONOFF]->textureID = LoadTGA("Image//menu/collisionoff.tga");

    meshList[SAVEON] = MeshBuilder::GenerateOBJ("saveon", "OBJ//menu.obj");
    meshList[SAVEON]->textureID = LoadTGA("Image//menu/saveon.tga");

    meshList[SAVEOFF] = MeshBuilder::GenerateOBJ("saveoff", "OBJ//menu.obj");
    meshList[SAVEOFF]->textureID = LoadTGA("Image//menu/saveoff.tga");

    meshList[ROTATEON] = MeshBuilder::GenerateOBJ("rotateon", "OBJ//menu.obj");
    meshList[ROTATEON]->textureID = LoadTGA("Image//menu/rotateon.tga");

    meshList[ROTATEOFF] = MeshBuilder::GenerateOBJ("rotateoff", "OBJ//menu.obj");
    meshList[ROTATEOFF]->textureID = LoadTGA("Image//menu/rotateoff.tga");

    meshList[WAYPOINTPROMPT] = MeshBuilder::GenerateOBJ("waypoint_message", "OBJ//menu.obj");
    meshList[WAYPOINTPROMPT]->textureID = LoadTGA("Image//menu/waypoint.tga");

    meshList[WAYPOINTON] = MeshBuilder::GenerateOBJ("waypointon", "OBJ//menu.obj");
    meshList[WAYPOINTON]->textureID = LoadTGA("Image//menu/waypointon.tga");

    meshList[WAYPOINTOFF] = MeshBuilder::GenerateOBJ("waypointoff", "OBJ//menu.obj");
    meshList[WAYPOINTOFF]->textureID = LoadTGA("Image//menu/waypointoff.tga");

    meshList[TOGGLEEN] = MeshBuilder::GenerateOBJ("toggle_enemy_button", "OBJ//menu.obj");
    meshList[TOGGLEEN]->textureID = LoadTGA("Image//menu/toggleen.tga");

    meshList[TOGGLEENON] = MeshBuilder::GenerateOBJ("rotateon", "OBJ//menu.obj");
    meshList[TOGGLEENON]->textureID = LoadTGA("Image//menu/toggleenon.tga");

    meshList[TOGGLEOBJ] = MeshBuilder::GenerateOBJ("toggle_object_button", "OBJ//menu.obj");
    meshList[TOGGLEOBJ]->textureID = LoadTGA("Image//menu/toggleobj.tga");

    meshList[TOGGLEOBJON] = MeshBuilder::GenerateOBJ("switch_object_mode_success", "OBJ//menu.obj");
    meshList[TOGGLEOBJON]->textureID = LoadTGA("Image//menu/toggleobjon.tga");

    meshList[TOGGLEOBJOFF] = MeshBuilder::GenerateOBJ("switch_object_mode_fail", "OBJ//menu.obj");
    meshList[TOGGLEOBJOFF]->textureID = LoadTGA("Image//menu/toggleobjoff.tga");

    meshList[ROTATE0] = MeshBuilder::GenerateOBJ("rotate0", "OBJ//menu.obj");
    meshList[ROTATE0]->textureID = LoadTGA("Image//menu/rotate0.tga");

    meshList[ROTATE90] = MeshBuilder::GenerateOBJ("rotate90", "OBJ//menu.obj");
    meshList[ROTATE90]->textureID = LoadTGA("Image//menu/rotate90.tga");

    meshList[ROTATE180] = MeshBuilder::GenerateOBJ("rotate180", "OBJ//menu.obj");
    meshList[ROTATE180]->textureID = LoadTGA("Image//menu/rotate180.tga");

    meshList[ROTATE270] = MeshBuilder::GenerateOBJ("rotate270", "OBJ//menu.obj");
    meshList[ROTATE270]->textureID = LoadTGA("Image//menu/rotate270.tga");

    meshList[COLLIDED] = MeshBuilder::GenerateOBJ("rotate270", "OBJ//menu.obj");
    meshList[COLLIDED]->textureID = LoadTGA("Image//menu/collided.tga");
    //Prevent Jerk
    camera.Init(Vector3(0, 0, 0), Vector3(1, 0, 0), Vector3(0, 1, 0));

    //Projection
    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 4000.f);
    projectionStack.LoadMatrix(projection);


    light[0].type = Light::LIGHT_DIRECTIONAL;
    light[0].position.Set(0, 1000, 0);
    light[0].color.Set(1, 1, 1);
    light[0].power = 1;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;
    light[0].cosCutoff = cos(Math::DegreeToRadian(45));
    light[0].cosInner = cos(Math::DegreeToRadian(30));
    light[0].exponent = 30.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f);

    light[1].type = Light::LIGHT_POINT;
    light[1].position.Set(0.f, 1000.f, 0.f);
    light[1].color.Set(1.f, 0.f, 0.f);
    light[1].power = 10;
    light[1].kC = 1.f;
    light[1].kL = 0.01f;
    light[1].kQ = 0.001f;
    light[1].cosCutoff = cos(Math::DegreeToRadian(45));
    light[1].cosInner = cos(Math::DegreeToRadian(30));
    light[1].exponent = 3.f;
    light[1].spotDirection.Set(0.f, 1.f, 0.f);

    light[2].type = Light::LIGHT_SPOT;
    light[2].position.Set(0.f, 0.f, 50.f);
    light[2].color.Set(1.f, 1.f, 0.f);
    light[2].power = 0;
    light[2].kC = 1.f;
    light[2].kL = 0.01f;
    light[2].kQ = 0.001f;
    light[2].cosCutoff = cos(Math::DegreeToRadian(45));
    light[2].cosInner = cos(Math::DegreeToRadian(30));
    light[2].exponent = 3.f;
    light[2].spotDirection.Set(0.f, 1.f, 0.f);

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

    // Make sure you pass uniform parameters after glUseProgram()
    glUniform1i(m_parameters[U_NUMLIGHTS], 3);



    //soundStorage.push_back(new Sound("gunshot.mp3", 1000));
    //soundStorage.push_back(new Sound("bleep.mp3", 100));
    //soundStorage.push_back(new Sound("Footstep.ogg"));
    //soundStorage.push_back(new Sound("backgroundmusic.mp3"));
    //soundStorage.push_back(new Sound("gunshot.mp3", 1000, 1));
    //soundStorage.push_back(new Sound("splatter.mp3", 1000, 1));
    //soundStorage.push_back(new Sound("run.mp3", 1000, 0.5));
    //soundStorage.push_back(new Sound("backgroundmusic.mp3"));
    //soundStorage.push_back(new Sound("jump.mp3", 1000, 0.5));
    //soundStorage.push_back(new Sound("footstep.mp3", 1000, 1));

    /* vec3df somePosition = { 0, 0,0};
    soundStorage[1]->play3DSound(true, false, false, somePosition);*/

    //soundStorage[3]->play2DSound(true, false, false); 

    //objFactory.createFactoryObject(new Enemy(this, Vector3(Math::RandFloatMinMax(-50, 50), 0, Math::RandFloatMinMax(-50, 50))));


    //delay between keypresses while in menu
    startTime = 0.0f;
    cooldown = 15.0f;

    //Set default collision to false
    passCol = false;

    //Set Pause Menu to off as default
    pauseActivate = false;

    checkType = OBJECT;

    //0
    soundStorage.push_back(new Sound("Sound/SFX/Cursor_Move.mp3", 1000, 1));
    //1
    soundStorage.push_back(new Sound("Sound/SFX/Success.mp3", 1000, 1));
    //2
    soundStorage.push_back(new Sound("Sound/SFX/Fail.mp3", 1000, 1));

}

void SceneEditor::Update(double dt)
{
    mouseStart += float(dt);
    if (mouseStart > 5.f)
    {
        Camera3::loadedCheck(true);
    }

    for (auto it1 = enemyStorage.begin(); it1 != enemyStorage.end(); it1++)
    {
        if ((*(it1 + 1)) != NULL)
        {
            for (auto it2 = it1 + 1; it2 != enemyStorage.end(); it2++)
            {
                if (((*it1)->_Position - (*it2)->_Position).Length() < 20 &&
                    (*it1)->getState() != 2 && (*it2)->getState() != 2)
                {
                    (*it1)->unitDistance.x = -((*it1)->unitDistance.x);
                    (*it1)->unitDistance.z = -((*it1)->unitDistance.z);

                    (*it2)->_Position = (*it2)->prevPos;

                }
                else if (((*it1)->_Position - (*it2)->_Position).Length() > 20)
                {
                    /*(*it1)->prevPos = (*it1)->_Position;*/
                    (*it2)->prevPos = (*it2)->_Position;
                }
            }
        }
    }
    //
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

    //cout << pressDelay << endl;

    _dt = (float)dt;
    _elapsedTime += _dt;

    //Process  obj movement
    objFactory.processInteraction();

    static const float LSPEED = 10.f;
    srand((unsigned)time(NULL));

    static float rotateWorld = 1;

    float run = 1.f;

    pressDelay += (float)dt;
    autoSave += (float)dt;

    //Auto Saving every 10 seconds
    if (autoSave > autoSaveCooldown)
    {
        if (waypointFinish)
        {
            save();
            autoSave = 0.f;
            saved = true;
        }
    }
    else if (autoSave > 1.f)
    {
        saved = false;
    }

    if (pressDelay > 0.5f)
    {
        pressDelay = 0.5f;
        saveEnter = 0;
        rotated = false;
        promptWaypoint = false;
        endedWaypoint = false;
        if (toggle == OBJECT_TO_ENEMY)
        {
            toggle = NEUTRAL_IN_ENEMY_MODE;
        }

        if (toggle == ENEMY_TO_OBJECT_FAIL)
        {
            toggle = NEUTRAL_IN_ENEMY_MODE;
        }

        if (toggle == ENEMY_TO_OBJECT_SUCCESS)
        {
            toggle = NEUTRAL_IN_OBJECT_MODE;
        }

        if (toggle == ENEMY_WAYPOINT_SAVED)
        {
            toggle = NEUTRAL_IN_ENEMY_MODE;
        }
    }


    if ((Application::IsKeyPressed('E') || Application::IsKeyPressed(VK_LBUTTON)) && pressDelay >= cooldownPressed)
    {
        if (checkType == OBJECT)
        {
            addObject();
        }
        if (checkType == ENEMY)
        {
            addEnemy();
        }

        if (passCol == true && waypointFinish)
        {
            save();
            autoSave = autoSaveCooldown;
        }
        pressDelay = 0.f;
    }

    if (Application::IsKeyPressed('T') && pressDelay >= cooldownPressed)
    {
        if (checkType == OBJECT)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            checkType = ENEMY;
            toggle = OBJECT_TO_ENEMY;
        }
        else if (checkType == ENEMY && waypointFinish)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            checkType = OBJECT;
            toggle = ENEMY_TO_OBJECT_SUCCESS;
        }
        else
        {
            soundStorage[2]->play3DSound(false, false, true, camPos);
            promptWaypoint = true;
            toggle = ENEMY_TO_OBJECT_FAIL;
        }
        pressDelay = 0.f;
    }

    if (Application::IsKeyPressed('P') && pressDelay >= cooldownPressed)
    {
        if (waypointCheck == true)
        {
            soundStorage[1]->play3DSound(false, false, true, camPos);
            endedWaypoint = true;
            toggle = ENEMY_WAYPOINT_SAVED;

            numOfWaypoints.push_back(to_string(totalWaypoint));
            save();
            autoSave = autoSaveCooldown;
            waypointFinish = true;
            waypointCheck = false;
            tempEnemy = false;

            bool nextStep = false;
            int bind = 0;

            tempEnemyCreate[tempEnemyCount] = new Robot(stof(enemySpeed.back()),
                stof(enemyDamage.back()),
                stof(enemyHP.back()),
                stof(enemyRange.back()),
                stof(enemyState.back()));
    
            //for (int i = 0; i < totalWaypoint; i++)
            //{
            //    cout << "Total Waypoint Amount: " << totalWaypoint << endl;
            for (vector<string>::iterator xIt = (enemyX.end() - totalWaypoint)+ bind; xIt != enemyX.end(); xIt++)
            {

                for (vector<string>::iterator zIt = (enemyZ.end() - totalWaypoint + bind); zIt != enemyZ.end() && nextStep == false; zIt++)
                {

                    tempEnemyCreate[tempEnemyCount]->addWaypoint(Vector3(stoi(xIt->data()), 0, stoi(zIt->data())));
                    nextStep = true;
                }
                nextStep = false;
                bind++;
                //break;
            }

                //}
                //enemyStorage.push_back(tempEnemyCreate[tempEnemyCount]);
                //tempEnemyCount++;
                //totalWaypoint = 0;
          
            enemyStorage.push_back(tempEnemyCreate[tempEnemyCount]);
            tempEnemyCount++;
            totalWaypoint = 0;
        }
        else
        {

        }
        pressDelay = 0.f;
    }

    if (Application::IsKeyPressed('Q') && pressDelay >= cooldownPressed)
    {
        if (checkType == OBJECT && objectName.size() != 0)
        {
            save();
            autoSave = autoSaveCooldown;
            removeObject();
        }
        else if(checkType == ENEMY && waypointFinish && enemyType.size() != 0)
        {
            save();
            autoSave = autoSaveCooldown;
            removeEnemy();
        }
        pressDelay = 0.f;
    }

    //if (Application::IsKeyPressed('P') && pressDelay >= cooldownPressed)
    //{
    //    if (pauseActivate == false)
    //    {
    //        pauseActivate = true;
    //        Camera3::pauseCheck(pauseActivate);
    //    }
    //    else
    //    {
    //        pauseActivate = false;
    //        Camera3::pauseCheck(pauseActivate);
    //    }
    //    pressDelay = 0.f;
    //}

    if (Application::IsKeyPressed(VK_TAB) && pressDelay >= 0.5f)
    {
        if (passCol == false && waypointFinish)
        {

            soundStorage[0]->play3DSound(false, false, true, camPos);
            save();
            autoSave = autoSaveCooldown;
            passCol = true;
            //Camera3::collisionSwitch(passCol, choosenLevel);

        }
        else
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            passCol = false;
            //Camera3::collisionSwitch(passCol, choosenLevel);
        }
        pressDelay = 0.f;
    }

    Camera3::collisionSwitch(passCol, choosenLevel);

    if (Application::IsKeyPressed('I') && pressDelay >= 0.5f)
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
        pressDelay = 0.f;
    }

    if (Application::IsKeyPressed(VK_RETURN) && pressDelay >= 0.5f)
    {
        if (waypointFinish)
        {
            soundStorage[1]->play3DSound(false, false, true, camPos);
            save();
            saveEnter = 1;
        }
        else
        {
            soundStorage[2]->play3DSound(false, false, true, camPos);
            promptWaypoint = true;
        }
        pressDelay = 0.f;
    }

    if (Application::IsKeyPressed('R') && pressDelay >= 0.5f)
    {
        if (checkType == OBJECT)
        {
            rotated = true;
            soundStorage[0]->play3DSound(false, false, true, camPos);

            switch (selectRotations)
            {
            case ZERO:
                selectRotations = NINETY;
                break;
            case NINETY:
                selectRotations = HUNDREDEIGHTY;
                break;
            case HUNDREDEIGHTY:
                selectRotations = TWOHUNDREDSEVENTY;
                break;
            case TWOHUNDREDSEVENTY:
                selectRotations = ZERO;
                break;
            }
            pressDelay = 0.f;
        }
    }

    vec3df footPos = { camera.getPosition().x, camera.getPosition().y - 5, camera.getPosition().z };
    //FootStep Sound
    if (Application::IsKeyPressed('W') && (_elapsedTime >= nextWalkTime))
    {
        nextWalkTime = _elapsedTime + coolDown;
        //soundStorage[2]->play3DSound(false, false, true, footPos);
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
    if (Application::IsKeyPressed('G'))
    {
        resetAll();
    }

    if (!waypointCheck)
    {
        if (Application::IsKeyPressed('1') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = MOUNTAIN;
            selectEnemies = ROBOT1;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('2') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = LAMP;
            selectEnemies = ROBOT2;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('3') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = LANTERN;
            selectEnemies = ROBOT3;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('4') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = TOMBSTONE;
            selectEnemies = ROBOT4;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('5') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = TREE;
            selectEnemies = SPIDER;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('6') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = STATUE1;
            selectEnemies = BLANK1;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('7') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = STATUE2;
            selectEnemies = BLANK2;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('8') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = EMPTY1;
            selectEnemies = BLANK3;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('9') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = EMPTY2;
            selectEnemies = BLANK4;
            pressDelay = 0.f;
        }

        if (Application::IsKeyPressed('0') && pressDelay >= cooldownPressed)
        {
            soundStorage[0]->play3DSound(false, false, true, camPos);
            selectObject = EMPTY3;
            selectEnemies = BLANK5;
            pressDelay = 0.f;
        }
    }

    camera.Update(dt);

    //FPS Counter
    FPS = 1 / (float)dt;

    startTime++;

    //If enemy is in range of player, damage HP
    //if (enemy->damageDealt(camera.position.x, camera.position.z) == true)
    //{
    //    Exit();
    //}
    if (selectRotations == ZERO)
    {
        rotateSelections = 0.f;
    }
    else if (selectRotations == NINETY)
    {
        rotateSelections = 90.f;
    }
    else if (selectRotations == HUNDREDEIGHTY)
    {
        rotateSelections = 180.f;
    }
    else
    {
        rotateSelections = 270.f;
    }

    //Enemy Arm Rotation
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

    Robot::dtFromScene(dt);
    Robot::positionFromCamera(camera);
    enemyUpdate(dt);
}

void SceneEditor::enemyUpdate(double dt)
{
    //-------This function is called in Tutorial::Update(double dt) to process Enemy interactions-----------------

    for (vector<Enemy*>::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
    {
        (*it)->update();

        if ((*it)->enemytype == 2)
        {
            if (((*it)->_Position - camera.getPosition()).Length() < (*it)->getRange())
            {
                robotShoot = true;
                broughtDown = false;

            }
            else
            {

            }

            if (shootArmDown == false)
            {
                if (_elapsedTime >= nextRobotShoot)
                {
                /*    objFactory.createFactoryObject(new Bullet(this, { (*it)->_Position.x, (*it)->_Position.y, (*it)->_Position.z }));*/
                    camera.health -= 10;
                    nextRobotShoot = _elapsedTime + coolDown;
                }
            }
        }


        for (vector<Object*>::iterator factoryIt = factoryIt = objFactory.Container.begin(); factoryIt != objFactory.Container.end(); factoryIt++)
        {
            if ((*factoryIt)->type == SceneEditor::GEO_LIGHTBALL3)   //BULLET INTERACTION WITH ENEMY
            {
                if (((*factoryIt)->position_ - (*it)->_Position).Length() < 20 && bulletTouch == false)
                {
                    if ((*it)->enemytype == 1)
                    {
                        (*it)->_Hp -= 10;
                        (*it)->healthBarSpider -= 2.f;
                        damaged = true;
                        bulletTouch = true;

                        if ((*it)->_Hp == 0)
                        {
                            (*it)->setState(2);  //  death is at enum 2
                        }
                    }
                    if ((*it)->enemytype == 2)
                    {
                        (*it)->_Hp -= 10;
                        (*it)->healthBarRobot1 -= 2.f;
                        damaged = true;
                        bulletTouch = true;

                        if ((*it)->_Hp == 0)
                        {
                            (*it)->setState(2);  //  death is at enum 2
                        }
                    }
                }
            }
        }
    }
}

void SceneEditor::copyPositionTargetView(Vector3 position, Vector3 target, Vector3 view, float pitch, float yaw)
{
    positionFromCamera = position;
    targetFromCamera = target;
    viewFromCamera = view;
    pitchFromCamera = pitch;
    yawFromCamera = yaw;

    targetFromCamera.x = cos(Math::DegreeToRadian(pitchFromCamera)) * cos(Math::DegreeToRadian(yawFromCamera)) + positionFromCamera.x;
    targetFromCamera.y = sin(Math::DegreeToRadian(pitchFromCamera)) + positionFromCamera.y;
    targetFromCamera.z = cos(Math::DegreeToRadian(pitchFromCamera)) * sin(Math::DegreeToRadian(yawFromCamera)) + positionFromCamera.z;

    positionFromCamera += viewFromCamera * 200.f;
    positionFromCamera.y = 0.f;
    targetFromCamera = positionFromCamera + viewFromCamera;



    //cout << "X: " << targetFromCamera.x << endl;
    //cout << "Z: " << targetFromCamera.z << endl;
}

void SceneEditor::Render()
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
    if (light[0].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[0].position.x, light[0].position.y, light[0].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (light[0].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[0].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
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

    //Generate Axes
    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); //Remember, matrix multiplication is the other way around
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    RenderMesh(meshList[GEO_AXES], false);

    modelStack.PushMatrix();
    modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
    RenderSkybox();
    modelStack.PopMatrix();


    renderGround();
    renderEnemy();
    renderObjects();

    renderTempEnemy();

    renderPosition();
    renderSprites();
    renderSelectObject();
    if (promptWaypoint == true)
    {
        RenderMeshOnScreen(meshList[WAYPOINTPROMPT], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
    }
    objFactory.renderFactoryObject();

    //Activate Pause Menu if boolean is true
    if (pauseActivate == true)
    {
        pause();
    }

    ////in game hud
    //RenderMeshOnScreen(meshList[GEO_GAME_HUD], 40, 32, 80, 65, 1, 0, 0, 1);

    ////minimap
    //RenderMeshOnScreen(meshList[GEO_MINI_GROUND], 10, 50, 15, 15, 1, 0, 0, 1);
    /*       RenderMeshOnScreen(meshList[GEO_MINI_PLAYER], 10.5 + ((camera.getPosition().x / 1000) * 14), 50 + ((camera.getPosition().z / 1000)* 14.4), 6, 6, 1, 0, 0, 1);
    for (vector<Object*>::iterator it = objFactory.Container.begin(); it != objFactory.Container.end(); it++)
    {
    RenderMeshOnScreen(meshList[GEO_MINI_ENEMY], 10.5 + (((*it)->position_.x / 1000) * 14), 50 + (((*it)->position_.z / 1000) * 14.4), 10, 10, 1, 0, 0, 1);
    }*/

}

void SceneEditor::renderEnemy()
{
    for (vector<Enemy *> ::iterator it = enemyStorage.begin(); it != enemyStorage.end(); it++)
    {
        if (((*it)->_Position - camera.getPosition()).Length() < 10 && (*it)->getState() != Spider::spiderState::death &&
            (*it)->getState() != Robot::robotState::death)
        {
            RenderMeshOnScreen(meshList[GEO_FLICKER], 40, 30, 80, 60, 1.f, 0.f, 0.f, 0.f, 1.f);
            vec3df bloodStartingLocation = { camera.getPosition().x, camera.getPosition().y, camera.getPosition().z };
            if (_elapsedTime >= nextSplatter)
                soundStorage[1]->play3DSound(false, false, false, bloodStartingLocation);
            nextSplatter = _elapsedTime + coolDown;

            camera.health--;

            if ((*it)->enemytype == 1)
            {
                if (Math::RandIntMinMax(0, 100) < 10)
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
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y, (*it)->_Position.z);
                             modelStack.Rotate((*it)->_Rotation, 0, 1, 0);
                             modelStack.Scale(10, 10, 10);
                             RenderMesh(meshList[GEO_SPIDER], true);
                             modelStack.PopMatrix();

                             //Health Bar
                             modelStack.PushMatrix();
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y + 1.3f, (*it)->_Position.z);
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

                         if ((*it)->robotType == 5)
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
                             modelStack.Translate((*it)->_Position.x, (*it)->_Position.y + 1.3f -30.f, (*it)->_Position.z);
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
        }
            break;
        }
    }
}

void SceneEditor::renderTempEnemy()
{
    if (selectEnemies == ROBOT1 && checkType == ENEMY)
    {
        modelStack.PushMatrix();
        modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
        modelStack.PushMatrix();
        modelStack.Scale(10, 10, 10);
        RenderMesh(meshList[GEO_ENEMYTORSO], true);
        RenderMesh(meshList[GEO_ENEMYHEAD], true);
        modelStack.PopMatrix();
        modelStack.PushMatrix();
        modelStack.Translate(0, 40, 0);
        modelStack.Rotate(-rotateArm, 0, 0, 1);
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
    }

    else if (selectEnemies == ROBOT2 && checkType == ENEMY)
    {
        modelStack.PushMatrix();
        modelStack.Translate(targetFromCamera.x, 0.f, targetFromCamera.z);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
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

    else if (selectEnemies == ROBOT3 && checkType == ENEMY)
    {
        modelStack.PushMatrix();
        modelStack.Translate(targetFromCamera.x, 0.f, targetFromCamera.z);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
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

    else if (selectEnemies == ROBOT4 && checkType == ENEMY)
    {
        modelStack.PushMatrix();
        modelStack.Translate(targetFromCamera.x, 0.f, targetFromCamera.z);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
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

    else if (selectEnemies == SPIDER && checkType == ENEMY)
    {
        modelStack.PushMatrix();
        modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
        modelStack.Scale(10, 10, 10);
        RenderMesh(meshList[GEO_SPIDER], true);
        modelStack.PopMatrix();
    }
    if (tempEnemy == true)
    {
        unsigned bind = 0;
        bool nextStep = false;
        for (vector<string>::iterator xIt = enemyX.end() - totalWaypoint; xIt != enemyX.end(); xIt++)
        {
            for (vector<string>::iterator zIt = enemyZ.end() + bind - totalWaypoint; zIt != enemyZ.end() && nextStep == false; zIt++)
            {
                //float x = stof(xIt->data());
                //float z = stof(zIt->data());
                Vector3 position = { stof(xIt->data()), 0.f, stof(zIt->data()) };
                Vector3 distance = (position - camera.position);
                float rotation = -Math::RadianToDegree(atan2(distance.z, distance.x));

                nextStep = true;
                modelStack.PushMatrix();
                modelStack.Translate(stof(xIt->data()), -30.f, stof(zIt->data()));
                modelStack.Scale(5.f, 5.f, 5.f);
                RenderMesh(meshList[GEO_LIGHTBALL3], true);
                modelStack.PopMatrix();

                modelStack.PushMatrix();
                modelStack.Translate(stof(xIt->data()), 0.f, stof(zIt->data()));
                //modelStack.Rotate(rotation, 0.f, 1.f, 0.f);
                modelStack.Scale(80.f, 80.f, 80.f);
                RenderText(meshList[GEO_X], to_string(bind + 1), Color(1.f, 0.f, 0.f));
                modelStack.PopMatrix();
            }
            bind++;
            nextStep = false;
        }
    }
}

void SceneEditor::objectsInit()
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

void SceneEditor::selectLevel(string input)
{
    choosenLevel = input;
}

void SceneEditor::renderObjects()
{
    if (removedObject == true)
    {
        //objectName.clear();
        //objectPosX.clear();
        //objectPosZ.clear();
        //loadConfig();


        //static int linePos = 1;
        //const string object = "--Object--";
        //const string objectEnd = "--Object End--";
        //const string enemy = "--Enemy--";
        //const string enemyEnd = "--Enemy End--";

        //string line = "";
        //ifstream myfile(choosenLevel);

        //while (myfile.peek() != EOF)
        //{
        //    getline(myfile, line);

        //    if (linePos == 1 && line == object)
        //    {
        //        
        //    }
        //    else if (linePos == 2)
        //    {
        //        string pos, posX, posZ;
        //        bool x = false;
        //        pos = line;

        //        for (unsigned i = 0; i < pos.size(); i++)
        //        {
        //            if (pos[i] != ',' && x == false)
        //            {
        //                posX += pos[i];
        //            }
        //            if (pos[i] == ',')
        //            {
        //                x = true;
        //                objectPosX.push_back(posX);
        //                continue;
        //            }
        //            if (x == true)
        //            {
        //                posZ += pos[i];
        //            }
        //        }
        //        objectPosZ.push_back(posZ);
        //        posX = "";
        //        posZ = "";

        //    }
        //    else if (linePos == 3)
        //    {


        //    }
        //    else if (linePos == 4)
        //    {
        //        objectName.push_back(line);
        //    }
        //    //++numOfObjects;

        //    else if (linePos == 5)
        //    {

        //    }
        //    linePos++;

        //    if (linePos > 5)
        //    {
        //        linePos = 1;
        //    }

        //    /*if (linePos < 3)
        //    {
        //        linePos++;
        //    }
        //    else
        //    {
        //        linePos = 1;
        //    }*/

        //}

        ////To check and enable collision according to objects.
        //const string mountain = "OBJ//mountain.obj";
        //const string lamp = "OBJ//lamp.obj";
        //const string lantern = "OBJ//lantern.obj";
        //const string tombstone = "OBJ//tombstone.obj";
        //const string tree = "OBJ//tree.obj";
        //const string statue1 = "OBJ//statue1.obj";
        //const string statue2 = "OBJ//statue2.obj";

        //int generateMesh = 0;
        //for (vector<string>::reverse_iterator objrIt = objectName.rbegin(); objrIt != objectName.rend(); objrIt++)
        //{
        //    for (vector<string>::reverse_iterator texrIt = objectTexture.rbegin() + generateMesh; texrIt != objectTexture.rend(); texrIt++)
        //    {
        //        if (objrIt->data() == mountain)
        //        {
        //            meshListPredefined[MOUNTAIN] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[MOUNTAIN]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == lamp)
        //        {
        //            meshListPredefined[LAMP] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[LAMP]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == lantern)
        //        {
        //            meshListPredefined[LANTERN] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[LANTERN]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == tombstone)
        //        {
        //            meshListPredefined[TOMBSTONE] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[TOMBSTONE]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == tree)
        //        {
        //            meshListPredefined[TREE] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[TREE]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == statue1)
        //        {
        //            meshListPredefined[STATUE1] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[STATUE1]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //        if (objrIt->data() == statue2)
        //        {
        //            meshListPredefined[STATUE2] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //            meshListPredefined[STATUE2]->textureID = LoadTGA(texrIt->data());
        //            break;
        //        }
        //    }
        //    generateMesh++;
        //}

        //int generateMesh = 0;
        //for (vector<string>::reverse_iterator objrIt = objectName.rbegin(); objrIt != objectName.rend(); objrIt++)
        //{
        //	for (vector<string>::reverse_iterator texrIt = objectTexture.rbegin() + generateMesh/*generateMesh*/; texrIt != objectTexture.rend(); texrIt++)
        //	{
        //		meshListPredefined[generateMesh/*generateMesh*/] = MeshBuilder::GenerateOBJ(objrIt->data(), objrIt->data());
        //		meshListPredefined[generateMesh/*generateMesh*/]->textureID = LoadTGA(texrIt->data());
        //		break;
        //	}
        //	/* generateMesh++;*/
        //	generateMesh++;
        //}

        //    removedObject = false;
    }
    ////////////////////////////////////////////////////////////////////////////////
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

                //modelStack.PopMatrix();
                //break;
            }
        }
        nextStep = false;
        generateObjects++;
    }
}

void SceneEditor::pause()
{
    if ((Application::IsKeyPressed('W') || Application::IsKeyPressed(VK_UP)) && _elapsedTime >= pressDelay)
    {
        pressDelay = _elapsedTime + coolDown;
        if (selectOptions == BACKTOMAINMENU)
        {
            selectOptions = CONTINUE;
        }
        else if (selectOptions == CONTINUE)
        {
            selectOptions = BACKTOMAINMENU;
        }
    }

    if ((Application::IsKeyPressed('S') || Application::IsKeyPressed(VK_DOWN)) && _elapsedTime >= pressDelay)
    {
        pressDelay = _elapsedTime + coolDown;
        if (selectOptions == CONTINUE)
        {
            selectOptions = BACKTOMAINMENU;
        }
        else if (selectOptions == BACKTOMAINMENU)
        {
            selectOptions = CONTINUE;
        }
    }
    RenderMeshOnScreen(meshList[PAUSE], 40, 30, 50, 50, 1.f, 0, 0, 0, 1);

    switch (selectOptions)
    {
    case CONTINUE:
        RenderMeshOnScreen(meshList[ARROW], 38, 23, 50, 50, 1.f, 0, 0, 0, 1);
        break;
    case BACKTOMAINMENU:
        RenderMeshOnScreen(meshList[ARROW], 5, 16, 50, 50, 1.f, 0, 0, 0, 1);
        break;
    }
}

void SceneEditor::renderSelectObject()
{
    if (checkType == OBJECT)
    { 
        //bool red = false;
        //bool samePos = false;

        if (objectName.size() > 0)
        {
  
            //------------------------------------Check for target collide with objects-----------------
            float xRange = 0.f, zRange = 0.f, xTempRange = 0.f, zTempRange = 0.f;
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

            //For Temporary Object that will be placed.
            if (selectObject == MOUNTAIN)
            {
                xTempRange = 70.f;
                zTempRange = 70.f;
            }

            else if (selectObject == LAMP ||
                selectObject == LANTERN)
            {
                xTempRange = 10.f;
                zTempRange = 10.f;
            }

            else if (selectObject == TREE)
            {
                xTempRange = 15.f;
                zTempRange = 15.f;
            }

            else if (selectObject == TOMBSTONE && 
                (selectRotations == ZERO || selectRotations == HUNDREDEIGHTY))
            {
                xTempRange = 30.f;
                zTempRange = 20.f;
            }

            else if (selectObject == TOMBSTONE &&
                (selectRotations == NINETY || selectRotations == TWOHUNDREDSEVENTY))
            {
                xTempRange = 20.f;
                zTempRange = 30.f;
            }

            else if (selectObject == STATUE1 &&
                (selectRotations == ZERO || selectRotations == HUNDREDEIGHTY))
            {
                xTempRange = 25.f;
                zTempRange = 10.f;
            }

            else if (selectObject == STATUE1 &&
                (selectRotations == NINETY || selectRotations == TWOHUNDREDSEVENTY))
            {
                xTempRange = 10.f;
                zTempRange = 25.f;
            }

            else if (selectObject == STATUE2 &&
                (selectRotations == ZERO || selectRotations == HUNDREDEIGHTY))
            {
                xTempRange = 40.f;
                zTempRange = 25.f;
            }

            else if (selectObject == STATUE2 &&
                (selectRotations == NINETY || selectRotations == TWOHUNDREDSEVENTY))
            {
                xTempRange = 25.f;
                zTempRange = 40.f;
            }

            if (passCol)
            {
                save();
                bool next = false, nextNext = false;
                unsigned bind = 0;

                for (vector<string>::iterator nameIt = objectName.begin(); nameIt != objectName.end(); nameIt++)
                {

                    //For Stored Objects
                    if (nameIt->data() == mountain ||
                        nameIt->data() == mountain90 ||
                        nameIt->data() == mountain180 ||
                        nameIt->data() == mountain270)
                    {
                        xRange = 70.f;
                        zRange = 70.f;
                    }

                    else if (nameIt->data() == lamp ||
                        nameIt->data() == lamp90 ||
                        nameIt->data() == lamp180 ||
                        nameIt->data() == lamp270 ||
                        nameIt->data() == lantern ||
                        nameIt->data() == lantern90 ||
                        nameIt->data() == lantern180 ||
                        nameIt->data() == lantern270)
                    {
                        xRange = 10.f;
                        zRange = 10.f;
                    }

                    else if (nameIt->data() == tree ||
                        nameIt->data() == tree90 ||
                        nameIt->data() == tree180 ||
                        nameIt->data() == tree270)
                    {
                        xRange = 15.f;
                        zRange = 15.f;
                    }

                    else if (nameIt->data() == tombstone ||
                        nameIt->data() == tombstone180)
                    {
                        xRange = 30.f;
                        zRange = 20.f;
                    }

                    else if (nameIt->data() == tombstone90 ||
                        nameIt->data() == tombstone270)
                    {
                        xRange = 20.f;
                        zRange = 30.f;
                    }

                    else if (nameIt->data() == statue1 ||
                        nameIt->data() == statue1180)
                    {
                        xRange = 25.f;
                        zRange = 10.f;
                    }

                    else if (nameIt->data() == statue190 ||
                        nameIt->data() == statue1270)
                    {
                        xRange = 10.f;
                        zRange = 25.f;
                    }

                    else if (nameIt->data() == statue2 ||
                        nameIt->data() == statue2180)
                    {
                        xRange = 40.f;
                        zRange = 25.f;
                    }

                    else if (nameIt->data() == statue290 ||
                        nameIt->data() == statue2270)
                    {
                        xRange = 25.f;
                        zRange = 40.f;
                    }

                    for (vector<string>::iterator xIt = objectPosX.begin() + bind; xIt != objectPosX.end() && next == false && samePos == false; xIt++)
                    {
                        next = true;
                        for (vector<string>::iterator zIt = objectPosZ.begin() + bind; zIt != objectPosZ.end() && nextNext == false && samePos == false; zIt++)
                        {


                            if ((((targetFromCamera.x + xTempRange) >= (stof(xIt->data()) - xRange)) &&
                                (stof(xIt->data()) + xRange) >= (targetFromCamera.x - xTempRange)) &&

                                (((targetFromCamera.z + zTempRange) >= (stof(zIt->data()) - zRange)) &&
                                (stof(zIt->data()) + zRange) >= (targetFromCamera.z - zTempRange)))
                            {
                                red = true;
                                samePos = true;
                                nextNext = true;
                                break;
                            }
                            else
                            {
    
                                samePos = false;
                                nextNext = true;
                                red = false;
                                break;
                            }
                            //break;
                        }
                        break;
                    }
                    xRange = 0.f;
                    zRange = 0.f;
                    next = false;
                    nextNext = false;
                    bind++;
                }

            }
        }
        //---------------------------------------End Check

        switch (selectObject)
        {
        case MOUNTAIN:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(100.f, 50.f, 100.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[MOUNTAIN], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_MOUNTAIN], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;

        case LAMP:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(10.f, 10.f, 10.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[LAMP], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_LAMP], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;


        case LANTERN:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(10.f, 10.f, 10.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[LANTERN], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_LANTERN], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;


        case TOMBSTONE:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(10.f, 10.f, 10.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[TOMBSTONE], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_TOMBSTONE], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;


        case TREE:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, -30.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(10.f, 10.f, 10.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[TREE], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_TREE], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;


        case STATUE1:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, 0.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Scale(20.f, 20.f, 20.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[STATUE1], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_STATUE1], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;


        case STATUE2:
            modelStack.PushMatrix();
            modelStack.Translate(targetFromCamera.x, 0.f, targetFromCamera.z);
            if (selectRotations == NINETY)
            {
                modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == HUNDREDEIGHTY)
            {
                modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            }
            else if (selectRotations == TWOHUNDREDSEVENTY)
            {
                modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
            }
            modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
            modelStack.Scale(10.f, 10.f, 10.f);
            if (!red)
            {
                RenderMesh(meshListPredefined[STATUE2], true);
            }
            else
            {
                RenderMesh(meshList[COLLIDED_STATUE2], true);
                RenderMeshOnScreen(meshList[COLLIDED], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            }
            modelStack.PopMatrix();
            break;
        }

        RenderMeshOnScreen(meshList[LEVELEDITOR], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);

        //Toggle button highlight.
        if (toggle == NEUTRAL_IN_OBJECT_MODE)
        {
            RenderMeshOnScreen(meshList[TOGGLEEN], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        else if (toggle == ENEMY_TO_OBJECT_SUCCESS)
        {
            RenderMeshOnScreen(meshList[TOGGLEOBJON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }


        if (passCol == true)
        {
            RenderMeshOnScreen(meshList[COLLISIONON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else
        {
            RenderMeshOnScreen(meshList[COLLISIONOFF], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        if (saveEnter == 1 || saved == true)
        {
            RenderMeshOnScreen(meshList[SAVEON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else
        {
            RenderMeshOnScreen(meshList[SAVEOFF], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        if (rotated == true)
        {
            RenderMeshOnScreen(meshList[ROTATEON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else
        {
            RenderMeshOnScreen(meshList[ROTATEOFF], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        if (selectRotations == ZERO)
        {
            RenderMeshOnScreen(meshList[ROTATE0], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (selectRotations == NINETY)
        {
            RenderMeshOnScreen(meshList[ROTATE90], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            RenderMeshOnScreen(meshList[ROTATE180], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (selectRotations == TWOHUNDREDSEVENTY)
        {
            RenderMeshOnScreen(meshList[ROTATE270], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        switch (selectObject)
        {
        case MOUNTAIN:
            RenderMeshOnScreen(meshList[SELECTION], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case LAMP:
            RenderMeshOnScreen(meshList[SELECTION], 48.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case LANTERN:
            RenderMeshOnScreen(meshList[SELECTION], 56.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case TOMBSTONE:
            RenderMeshOnScreen(meshList[SELECTION], 64.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case TREE:
            RenderMeshOnScreen(meshList[SELECTION], 72.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case STATUE1:
            RenderMeshOnScreen(meshList[SELECTION], 80.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case STATUE2:
            RenderMeshOnScreen(meshList[SELECTION], 88.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case EMPTY1:
            RenderMeshOnScreen(meshList[SELECTION], 96.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case EMPTY2:
            RenderMeshOnScreen(meshList[SELECTION], 104.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case EMPTY3:
            RenderMeshOnScreen(meshList[SELECTION], 112.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        }

        //1
        RenderMeshOnScreen(meshList[GEO_MOUNTAIN], 4.f, 1.5f, 5.f, 5.f, 6.f, rotateSelections, 0.f, 1.f, 0.f);
        //2
        RenderMeshOnScreen(meshList[GEO_LAMP], 12.f, 1.f, 5.f, 3.f, 4.f, rotateSelections, 0.f, 1.f, 0.f);
        //3
        RenderMeshOnScreen(meshList[GEO_LANTERN], 20.f, 1.f, 5.f, 2.8f, 4.f, rotateSelections, 0.f, 1.f, 0.f);
        //4
        RenderMeshOnScreen(meshList[GEO_TOMBSTONE], 28.f, 1.5f, 1.f, 1.f, 2.f, rotateSelections, 0.f, 1.f, 0.f);
        //5
        RenderMeshOnScreen(meshList[GEO_TREE], 36.f, 1.4f, 0.3f, 0.25f, 0.3f, rotateSelections, 0.f, 1.f, 0.f);
        //6
        RenderMeshOnScreen(meshList[GEO_STATUE1], 44.f, 4.f, 2.5f, 2.5f, 4.f, rotateSelections, 0.f, 1.f, 0.f);
        //7
        RenderMeshOnScreen(meshList[GEO_STATUE2], 51.5f, 4.f, 1.f, 1.2f, 1.f, rotateSelections, 0.f, 1.f, 0.f);

    }
    else
    {
        RenderMeshOnScreen(meshList[LEVELEDITOR], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);

        //Toggle button highlight.
        if (toggle == NEUTRAL_IN_ENEMY_MODE || toggle == ENEMY_WAYPOINT_SAVED)
        {
            RenderMeshOnScreen(meshList[TOGGLEOBJ], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (toggle == OBJECT_TO_ENEMY)
        {
            RenderMeshOnScreen(meshList[TOGGLEENON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (toggle == ENEMY_TO_OBJECT_FAIL)
        {
            RenderMeshOnScreen(meshList[TOGGLEOBJOFF], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        if (!endedWaypoint && waypointCheck)
        {
            RenderMeshOnScreen(meshList[WAYPOINTOFF], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }
        else if (endedWaypoint)
        {
            RenderMeshOnScreen(meshList[WAYPOINTON], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
        }

        switch (selectEnemies)
        {
        case ROBOT1:
            RenderMeshOnScreen(meshList[SELECTION], 40.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case ROBOT2:
            RenderMeshOnScreen(meshList[SELECTION], 48.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case ROBOT3:
            RenderMeshOnScreen(meshList[SELECTION], 56.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case ROBOT4:
            RenderMeshOnScreen(meshList[SELECTION], 64.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case SPIDER:
            RenderMeshOnScreen(meshList[SELECTION], 72.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case BLANK1:
            RenderMeshOnScreen(meshList[SELECTION], 80.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case BLANK2:
            RenderMeshOnScreen(meshList[SELECTION], 88.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case BLANK3:
            RenderMeshOnScreen(meshList[SELECTION], 96.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case BLANK4:
            RenderMeshOnScreen(meshList[SELECTION], 104.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        case BLANK5:
            RenderMeshOnScreen(meshList[SELECTION], 112.f, 30.f, 80.f, 60.f, 1.f, 0.f, 0.f, 0.f, 1.f);
            break;
        }

        //1
        RenderMeshOnScreen(meshList[GEO_ENEMYTORSO], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYHEAD], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTARM], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTARM], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTLEG], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTLEG], 4.f, 0.25f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        //2
        RenderMeshOnScreen(meshList[GEO_ENEMYTORSO2], 12.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTARM2], 12.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTARM2], 12.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTLEG2], 12.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTLEG2], 12.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        //3
        RenderMeshOnScreen(meshList[GEO_ENEMYTORSO3], 20.f, 3.5f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTARM3], 20.f, 3.5f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTARM3], 20.f, 3.5f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTLEG3], 20.f, 3.5f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTLEG3], 20.f, 3.5f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        //4
        RenderMeshOnScreen(meshList[GEO_ENEMYTORSO4], 28.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTARM4], 28.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTARM4], 28.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYRIGHTLEG4], 28.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        RenderMeshOnScreen(meshList[GEO_ENEMYLEFTLEG4], 28.f, 4.f, 1.3f, 1.3f, 1.3f, 90.f, 0.f, 1.f, 0.f);
        //5
        RenderMeshOnScreen(meshList[GEO_SPIDER], 36.f, 1.4f, 1.5f, 1.5f, 1.5f, 90.f, 0.f, 1.f, 0.f);
        //6

        //7

    }

    samePos = false;
    red = false;
}

void SceneEditor::addObject()
{
    if (selectObject == MOUNTAIN)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//mountain.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//mountain90.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//mountain180.obj");
        }
        else
        {
            objectName.push_back("OBJ//mountain270.obj");
        }

        objectTexture.push_back("Image//objects.tga");
    }
    if (selectObject == LAMP)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//lamp.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//lamp90.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//lamp180.obj");
        }
        else
        {
            objectName.push_back("OBJ//lamp270.obj");
        }
        objectTexture.push_back("Image//lamp.tga");
    }
    if (selectObject == LANTERN)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//lantern.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//lantern90.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//lantern180.obj");
        }
        else
        {
            objectName.push_back("OBJ//lantern270.obj");
        }
        objectTexture.push_back("Image//lantern.tga");
    }
    if (selectObject == TOMBSTONE)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//tombstone.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//tombstone90.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//tombstone180.obj");
        }
        else
        {
            objectName.push_back("OBJ//tombstone270.obj");
        }
        objectTexture.push_back("Image//tombstone.tga");
    }
    if (selectObject == TREE)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//tree.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//tree90.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//tree180.obj");
        }
        else
        {
            objectName.push_back("OBJ//tree270.obj");
        }
        objectTexture.push_back("Image//tree.tga");
    }
    if (selectObject == STATUE1)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//statue1.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//statue190.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//statue1180.obj");
        }
        else
        {
            objectName.push_back("OBJ//statue1270.obj");
        }
        objectTexture.push_back("Image//statue1.tga");
    }
    if (selectObject == STATUE2)
    {
        if (selectRotations == ZERO)
        {
            objectName.push_back("OBJ//statue2.obj");
        }
        else if (selectRotations == NINETY)
        {
            objectName.push_back("OBJ//statue290.obj");
        }
        else if (selectRotations == HUNDREDEIGHTY)
        {
            objectName.push_back("OBJ//statue2180.obj");
        }
        else
        {
            objectName.push_back("OBJ//statue2270.obj");
        }
        objectTexture.push_back("Image//statue2.tga");
    }
    //objectPosX.push_back(to_string((int)camera.position.x));
    //objectPosZ.push_back(to_string((int)camera.position.z));

    float xRange = 0.f, zRange = 0.f, xTempRange = 0.f, zTempRange = 0.f;
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

    //For Temporary Object that will be placed.
    if (objectName.back() == mountain ||
        objectName.back() == mountain90 ||
        objectName.back() == mountain180 ||
        objectName.back() == mountain270)
    {
        xTempRange = 70.f;
        zTempRange = 70.f;
    }

    else if (objectName.back() == lamp ||
        objectName.back() == lamp90 ||
        objectName.back() == lamp180 ||
        objectName.back() == lamp270 ||
        objectName.back() == lantern ||
        objectName.back() == lantern90 ||
        objectName.back() == lantern180 ||
        objectName.back() == lantern270)
    {
        xTempRange = 10.f;
        zTempRange = 10.f;
    }

    else if (objectName.back() == tree ||
        objectName.back() == tree90 ||
        objectName.back() == tree180 ||
        objectName.back() == tree270)
    {
        xTempRange = 15.f;
        zTempRange = 15.f;
    }

    else if (objectName.back() == tombstone ||
        objectName.back() == tombstone180)
    {
        xTempRange = 30.f;
        zTempRange = 20.f;
    }

    else if (objectName.back() == tombstone90 ||
        objectName.back() == tombstone270)
    {
        xTempRange = 20.f;
        zTempRange = 30.f;
    }

    else if (objectName.back() == statue1 ||
        objectName.back() == statue1180)
    {
        xTempRange = 25.f;
        zTempRange = 10.f;
    }

    else if (objectName.back() == statue190 ||
        objectName.back() == statue1270)
    {
        xTempRange = 10.f;
        zTempRange = 25.f;
    }

    else if (objectName.back() == statue2 ||
        objectName.back() == statue2180)
    {
        xTempRange = 40.f;
        zTempRange = 25.f;
    }

    else if (objectName.back() == statue290 ||
        objectName.back() == statue2270)
    {
        xTempRange = 25.f;
        zTempRange = 40.f;
    }

    bool samePosition = false;

    if (passCol)
    {
        save();
        if (objectName.size() > 0)
        {

            bool next = false, nextNext = false;
            unsigned bind = 0;


            for (vector<string>::iterator nameIt = objectName.begin(); nameIt != objectName.end() - 1; nameIt++)
            {

                //For Stored Objects
                if (nameIt->data() == mountain ||
                    nameIt->data() == mountain90 ||
                    nameIt->data() == mountain180 ||
                    nameIt->data() == mountain270)
                {
                    xRange = 70.f;
                    zRange = 70.f;
                }

                else if (nameIt->data() == lamp ||
                    nameIt->data() == lamp90 ||
                    nameIt->data() == lamp180 ||
                    nameIt->data() == lamp270 ||
                    nameIt->data() == lantern ||
                    nameIt->data() == lantern90 ||
                    nameIt->data() == lantern180 ||
                    nameIt->data() == lantern270)
                {
                    xRange = 10.f;
                    zRange = 10.f;
                }

                else if (nameIt->data() == tree ||
                    nameIt->data() == tree90 ||
                    nameIt->data() == tree180 ||
                    nameIt->data() == tree270)
                {
                    xRange = 15.f;
                    zRange = 15.f;
                }

                else if (nameIt->data() == tombstone ||
                    nameIt->data() == tombstone180)
                {
                    xRange = 30.f;
                    zRange = 20.f;
                }

                else if (nameIt->data() == tombstone90 ||
                    nameIt->data() == tombstone270)
                {
                    xRange = 20.f;
                    zRange = 30.f;
                }

                else if (nameIt->data() == statue1 ||
                    nameIt->data() == statue1180)
                {
                    xRange = 25.f;
                    zRange = 10.f;
                }

                else if (nameIt->data() == statue190 ||
                    nameIt->data() == statue1270)
                {
                    xRange = 10.f;
                    zRange = 25.f;
                }

                else if (nameIt->data() == statue2 ||
                    nameIt->data() == statue2180)
                {
                    xRange = 40.f;
                    zRange = 25.f;
                }

                else if (nameIt->data() == statue290 ||
                    nameIt->data() == statue2270)
                {
                    xRange = 25.f;
                    zRange = 40.f;
                }

                for (vector<string>::iterator xIt = objectPosX.begin() + bind; xIt != objectPosX.end() && next == false && samePosition == false; xIt++)
                {
                    next = true;
                    for (vector<string>::iterator zIt = objectPosZ.begin() + bind; zIt != objectPosZ.end() && nextNext == false && samePosition == false; zIt++)
                    {


                        /*       cout << "Temp Object Min X: " << (targetFromCamera.x - xTempRange) << "," << "Temp Object Max X: " << (targetFromCamera.x + xTempRange) << endl;
                               cout << "Object " << bind << " Min X: " << (stof(xIt->data()) - xRange) << "," << "Object " << bind << " Max X: " << (stof(xIt->data()) + xRange) << endl;

                               cout << "Temp Object Min Z: " << (targetFromCamera.z - zTempRange) << "," << "Temp Object Max Z: " << (targetFromCamera.z + zTempRange) << endl;
                               cout << "Object " << bind << " Min Z: " << (stof(zIt->data()) - zRange) << "," << "Object " << bind << " Max Z: " << (stof(zIt->data()) + zRange) << endl;*/

                        if ((((targetFromCamera.x + xTempRange) >= (stof(xIt->data()) - xRange)) &&
                            (stof(xIt->data()) + xRange) >= (targetFromCamera.x - xTempRange)) &&

                            (((targetFromCamera.z + zTempRange) >= (stof(zIt->data()) - zRange)) &&
                            (stof(zIt->data()) + zRange) >= (targetFromCamera.z - zTempRange)))
                        {
        
                            samePosition = true;
                            nextNext = true;
                            break;
                        }
                        else
                        {
           
                            samePosition = false;
                            nextNext = true;
                            break;
                        }
                        //break;
                    }
                    break;
                }
                xRange = 0.f;
                zRange = 0.f;
                next = false;
                nextNext = false;
                bind++;
            }
        }
    }

    //if (objectName.size() == 0)
    //{
    //    objectPosX.push_back(to_string((int)targetFromCamera.x));
    //    objectPosZ.push_back(to_string((int)targetFromCamera.z));
    //}
    //if (samePosition)
    //{
    //    objectName.pop_back();
    //}

    if (samePosition)
    {
        vec3df camPos = {camera.position.x, camera.position.y, camera.position.z};
        soundStorage[2]->play3DSound(false, false, true, camPos);
        objectName.pop_back();
    }
    else
    {

        objectPosX.push_back(to_string((int)targetFromCamera.x));
        objectPosZ.push_back(to_string((int)targetFromCamera.z));

    }

    if (objectPosX.size() >= 1)
    {

        bool checkStore = false, checkCheck = false;
        unsigned bindCheck = 0;
        for (auto it = objectName.begin(); it != objectName.end(); it++)
        {
            for (auto x = objectPosX.begin() + bindCheck; x != objectPosX.end() && checkStore == false; x++)
            {
                checkStore = true;
                for (auto z = objectPosZ.begin() + bindCheck; z != objectPosZ.end() && checkCheck == false; z++)
                {

                    checkCheck = true;
                }
            }
            bindCheck++;
            checkStore = false;
            checkCheck = false;
        }

    }

}

void SceneEditor::addEnemy()
{
    if (waypointCheck == false)
    {
        waypointFinish = false;
        if (selectEnemies == ROBOT1)
        {
            enemyType.push_back("Robot");
            enemySpeed.push_back("100");
            enemyDamage.push_back("1");
            enemyHP.push_back("30");
            enemyRange.push_back("100");
            enemyState.push_back("1");

            string x = "", z = "";
            x = to_string((int)targetFromCamera.x);
            z = to_string((int)targetFromCamera.z);

            enemyX.push_back(x);
            enemyZ.push_back(z);

            tempEnemy = true;
            waypointCheck = true;
            totalWaypoint++;
        }
        else if (selectEnemies == ROBOT2)
        {
            waypointFinish = false;

                enemyType.push_back("Robot");
                enemySpeed.push_back("100");
                enemyDamage.push_back("1");
                enemyHP.push_back("30");
                enemyRange.push_back("100");
                enemyState.push_back("2");

                string x = "", z = "";
                x = to_string((int)targetFromCamera.x);
                z = to_string((int)targetFromCamera.z);

                enemyX.push_back(x);
                enemyZ.push_back(z);


                tempEnemy = true;
                waypointCheck = true;
                totalWaypoint++;

        }
        else if (selectEnemies == ROBOT3)
        {
            waypointFinish = false;

                enemyType.push_back("Robot");
                enemySpeed.push_back("100");
                enemyDamage.push_back("1");
                enemyHP.push_back("30");
                enemyRange.push_back("100");
                enemyState.push_back("3");

                string x = "", z = "";
                x = to_string((int)targetFromCamera.x);
                z = to_string((int)targetFromCamera.z);

                enemyX.push_back(x);
                enemyZ.push_back(z);


                tempEnemy = true;
                waypointCheck = true;
                totalWaypoint++;

        }
        else if (selectEnemies == ROBOT4)
        {
            waypointFinish = false;

                enemyType.push_back("Robot");
                enemySpeed.push_back("100");
                enemyDamage.push_back("1");
                enemyHP.push_back("30");
                enemyRange.push_back("100");
                enemyState.push_back("4");

                string x = "", z = "";
                x = to_string((int)targetFromCamera.x);
                z = to_string((int)targetFromCamera.z);

                enemyX.push_back(x);
                enemyZ.push_back(z);


                tempEnemy = true;
                waypointCheck = true;
                totalWaypoint++;

        }
        else if (selectEnemies == SPIDER)
        {
            waypointFinish = false;

                enemyType.push_back("Robot");
                enemySpeed.push_back("100");
                enemyDamage.push_back("1");
                enemyHP.push_back("30");
                enemyRange.push_back("100");

                //Pushing state even though there is no state for Spider so that vectors will stay in sync.
                enemyState.push_back("5");

                string x = "", z = "";
                x = to_string((int)targetFromCamera.x);
                z = to_string((int)targetFromCamera.z);

                enemyX.push_back(x);
                enemyZ.push_back(z);


                tempEnemy = true;
                waypointCheck = true;
                totalWaypoint++;
        }
    }
    else
    {
        string x = "", z = "";
        x = to_string((int)targetFromCamera.x);
        z = to_string((int)targetFromCamera.z);


        enemyX.push_back(x);
        enemyZ.push_back(z);
        totalWaypoint++;
    }
}

void SceneEditor::removeObject()
{

    if (objectName.size() != 0)
    {
        //ofstream outputFile;
        //outputFile.open("tempLevel.txt");
        //outputFile.close();

        objectName.pop_back();
        objectTexture.pop_back();
        objectPosX.pop_back();
        objectPosZ.pop_back();

        save();
        //ofstream outputFile;
        //outputFile.open("tempLevel.txt");
        //unsigned bindVector = 0;
        //bool nextStep = false, nextNextStep = false;

        //for (vector<string>::iterator objIt = objectName.begin(); objIt != objectName.end(); objIt++)
        //{
        //    for (vector<string>::iterator textIt = objectTexture.begin() + bindVector; textIt != objectTexture.end() && nextStep == false; textIt++)
        //    {
        //        nextStep = true;
        //        for (vector<string>::iterator xIt = objectPosX.begin() + bindVector; xIt != objectPosX.end() && nextNextStep == false; xIt++)
        //        {
        //            nextNextStep = true;
        //            for (vector<string>::iterator zIt = objectPosZ.begin() + bindVector; zIt != objectPosZ.end(); zIt++)
        //            {
        //                outputFile << "--Object--" << endl;
        //                outputFile << xIt->data() << "," << zIt->data() << endl;
        //                outputFile << textIt->data() << endl;
        //                outputFile << objIt->data() << endl;
        //                outputFile << "--Object End--" << endl;
        //                bindVector++;

        //                break;
        //            }
        //            break;
        //        }
        //        break;
        //    }
        //    nextStep = false;
        //    nextNextStep = false;
        //}

        //bool speed = false,
        //    damage = false,
        //    hp = false,
        //    range = false,
        //    state = false,
        //    numPoint = false,
        //    way = false,
        //    x = false,
        //    z = false;

        //unsigned bind = 0;
        //string sAmountOfPoints;
        //int amountOfPoints = 0;
        //unsigned pairCoordiantesWithEnemy = 0;


        //for (vector<string>::iterator typeIt = enemyType.begin(); typeIt != enemyType.end(); typeIt++)
        //{
        //    outputFile << "--Enemy--" << endl;
        //    for (vector<string>::iterator speedIt = enemySpeed.begin() + bind; speedIt != enemySpeed.end() && speed == false; speedIt++)
        //    {
        //        speed = true;
        //        for (vector<string>::iterator damageIt = enemyDamage.begin() + bind; damageIt != enemyDamage.end() && damage == false; damageIt++)
        //        {
        //            damage = true;
        //            for (vector<string>::iterator hpIt = enemyHP.begin() + bind; hpIt != enemyHP.end() && hp == false; hpIt++)
        //            {
        //                hp = true;
        //                for (vector<string>::iterator rangeIt = enemyRange.begin() + bind; rangeIt != enemyRange.end() && range == false; rangeIt++)
        //                {
        //                    range = true;
        //                    for (vector<string>::iterator stateIt = enemyState.begin() + bind; stateIt != enemyState.end() && state == false; stateIt++)
        //                    {
        //                        outputFile << typeIt->data() << "," <<
        //                            speedIt->data() << "," <<
        //                            damageIt->data() << "," <<
        //                            hpIt->data() << "," <<
        //                            rangeIt->data() << "," <<
        //                            stateIt->data() << endl;
        //                        state = true;

        //                        for (vector<string>::iterator numPointIt = numOfWaypoints.begin() + bind;
        //                            numPointIt != numOfWaypoints.end() &&
        //                            numPoint == false; numPointIt++)
        //                        {
        //                            sAmountOfPoints = numPointIt->data();
        //                            amountOfPoints = stoi(sAmountOfPoints);

        //                            outputFile << amountOfPoints << endl;
        //                            break;
        //                        }

        //                        for (unsigned i = 0; i < amountOfPoints; i++)
        //                        {
        //                            for (vector<string>::iterator xIt = enemyX.begin() + pairCoordiantesWithEnemy; xIt != enemyX.end() && x == false; xIt++)
        //                            {
        //                                x = true;
        //                                for (vector<string>::iterator zIt = enemyZ.begin() + pairCoordiantesWithEnemy; zIt != enemyZ.end() && z == false; zIt++)
        //                                {
        //                                    outputFile << xIt->data() << "," << zIt->data() << endl;
        //                                    z = true;
        //                                    break;
        //                                }
        //                                break;
        //                            }
        //                            pairCoordiantesWithEnemy++;
        //                            x = false;
        //                            z = false;
        //                        }
        //                        break;
        //                    }
        //                    break;
        //                }
        //                break;
        //            }
        //            break;
        //        }
        //        break;
        //    }
        //    speed = false;
        //    damage = false;
        //    hp = false;
        //    range = false;
        //    state = false;
        //    numPoint = false;
        //    bind++;
        //    outputFile << "--Enemy End--" << endl;
        //}
        //outputFile.close();

        //int generateMesh = 0;
        //bool toTexture = false, toPosition = false;
        //for (vector<string>::iterator objIt = objectName.begin(); objIt < objectName.end(); objIt++)
        //{

        //    for (vector<string>::iterator texIt = objectTexture.begin() + generateMesh;
        //        toTexture == false;)
        //    {
        //        toTexture = true;

        //        for (vector<string>::iterator posXIt = objectPosX.begin() + generateMesh;
        //            toPosition == false;)
        //        {
        //            toPosition = true;
        //            for (vector<string>::iterator posZIt = objectPosZ.begin() + generateMesh;
        //                posZIt != objectPosZ.end();)
        //            {
        //                ofstream outputFile;
        //                outputFile.open("tempLevel.txt", ios::app);
        //                outputFile << "--Object--" << endl;
        //                outputFile << posXIt->data() << "," << posZIt->data() << endl;
        //                outputFile << texIt->data() << endl;
        //                outputFile << objIt->data() << endl;
        //                outputFile << "--Object End--" << endl;
        //                outputFile.close();
        //                break;
        //            }

        //        }

        //    }
        //    generateMesh++;
        //    toTexture = false;
        //    toPosition = false;
        //}
        //if (choosenLevel == "tutorial.txt")
        //{
        //    remove("tutorial.txt");
        //    rename("tempLevel.txt", "tutorial.txt");
        //}
        //else if (choosenLevel == "level1.txt")
        //{
        //    remove("level1.txt");
        //    rename("tempLevel.txt", "level1.txt");
        //}
        //else if (choosenLevel == "level2.txt")
        //{
        //    remove("level2.txt");
        //    rename("tempLevel.txt", "level2.txt");
        //}
        //else if (choosenLevel == "level3.txt")
        //{
        //    remove("level3.txt");
        //    rename("tempLevel.txt", "level3.txt");
        //}
        ////else if (choosenLevel == "level4.txt")
        ////{
        ////    remove("level4.txt");
        ////    rename("tempLevel.txt", "level4.txt");
        ////}
        //else if (choosenLevel == "boss.txt")
        //{
        //    remove("boss.txt");
        //    rename("tempLevel.txt", "boss.txt");
        //}
    }
    else
    {
        ofstream outputFile;
        outputFile.open(choosenLevel);
        outputFile.close();
    }
    removedObject = true;

}

void SceneEditor::removeEnemy()
{
    enemyType.pop_back();
    enemySpeed.pop_back();
    enemyHP.pop_back();
    enemyRange.pop_back();
    enemyState.pop_back();
    unsigned waypointPop = stoi(numOfWaypoints.back());
    numOfWaypoints.pop_back();
    while (waypointPop != 0)
    {
        enemyX.pop_back();
        enemyZ.pop_back();
        waypointPop--;
    }
    enemyStorage.pop_back();
    //numOfWaypoints.pop_back();
    //if (totalWaypoint)
    //{
    //    enemyX.pop_back();
    //    enemyZ.pop_back();
    //}
    //else
    //{
    //    numOfWaypoints.pop_back();
    //}
    //save();
    //bool speed = false,
    //    damage = false,
    //    hp = false,
    //    range = false,
    //    state = false,
    //    numPoint = false,
    //    way = false,
    //    x = false,
    //    z = false;
}

void SceneEditor::removeWaypoint()
{

}

void SceneEditor::RenderSkybox()
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


void SceneEditor::renderGround()
{
    modelStack.PushMatrix();
    modelStack.Translate(0.f, -30.f, 0.f);
    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_GROUND], true);
    modelStack.PopMatrix();
}
//
void SceneEditor::renderSprites()
{
    //Default hands
    RenderMeshOnScreen(meshList[GEO_HANDL1], 15, 5, 100, 100, 0, 0, 0, 0, 1);
    RenderMeshOnScreen(meshList[GEO_HANDR1], 65, 5, 100, 100, 0, 0, 0, 0, 1);

    ////Punching hands
    //RenderMeshOnScreen(meshList[GEO_HANDL2], 15, 10, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_HANDR2], 65, 10, 100, 100);

    ////Gun
    //RenderMeshOnScreen(meshList[GEO_GUN1], 65, 9, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_GUN2], 65, 12, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_GUN3], 65, 11, 100, 100);

    ////Cannon
    //RenderMeshOnScreen(meshList[GEO_CANNON1], 40, 11, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_CANNON2], 40, 13, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_CANNON3], 40, 12, 100, 100);

    ////Sword
    //RenderMeshOnScreen(meshList[GEO_SWORD1], 40, 19, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_SWORD2], 48, 6, 100, 100);
    //RenderMeshOnScreen(meshList[GEO_SWORD3], 45, 7, 100, 100);
}


void SceneEditor::renderPosition()
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


//void SceneEditor::renderText()
//{
//    std::string sFPS = "FPS: " + std::to_string(FPS);
//    sFPS.resize(9);
//    RenderTextOnScreen(meshList[GEO_FPS], sFPS, Color(1, 1, 0), 2, 0, 29);
//}
//


void SceneEditor::RenderText(Mesh* mesh, std::string text, Color color)
{
    if (!mesh || mesh->textureID <= 0) //Proper error check
    {
        return;
    }

    glDisable(GL_DEPTH_TEST);
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
        characterSpacing.SetToTranslation(i * 0.3f, 0.f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void SceneEditor::RenderMesh(Mesh *mesh, bool enableLight) {
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


void SceneEditor::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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

void SceneEditor::RenderMeshOnScreen(Mesh* mesh, float x, float y, float sizex, float sizey, float sizez, float rotateAngle, float xAxis, float yAxis, float zAxis)
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
    modelStack.Translate(x, y, 0);
    modelStack.Rotate(rotateAngle, xAxis, yAxis, zAxis);
    modelStack.Scale(sizex, sizey, sizez);
    //to do: scale and translate accordingly
    RenderMesh(mesh, false); //UI should not have light
    projectionStack.PopMatrix();
    viewStack.PopMatrix();
    modelStack.PopMatrix();
    glEnable(GL_DEPTH_TEST);
}

void SceneEditor::resetAll()
{
    camera.position.Set(0.f, 0.f, 0.f);
}

void SceneEditor::Exit()
{
    // Cleanup VBO here
    for (int i = 0; i < totalObjects; ++i)
    {
        if (meshListPredefined[i] != NULL)
        {
            delete meshListPredefined[i];
        }
    }

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
