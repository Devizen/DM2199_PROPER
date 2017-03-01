#include "File.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>



using std::cout;
using std::cin;
using std::endl;
using std::ofstream;
using std::ifstream;

File::File()
{

}

File::~File()
{

}

void File::save(string level,
    vector<string> objName,
    vector<string> objTex,
    vector<string> objX,
    vector<string> objZ,
    vector<string> enType,
    vector<string> enSpeed,
    vector<string> enDamage,
    vector<string> enHP,
    vector<string> enRange,
    vector<string> enState,
    vector<string> waypoint,
    vector<string> enX,
    vector<string> enZ)
{
    ofstream outputFile;
    outputFile.open(level/*, ios::app*/);
    unsigned bindVector = 0;
    bool nextStep = false, nextNextStep = false;

    for (vector<string>::iterator objIt = objName.begin(); objIt != objName.end(); objIt++)
    {
        for (vector<string>::iterator textIt = objTex.begin() + bindVector; textIt != objTex.end() && nextStep == false; textIt++)
        {
            nextStep = true;
            for (vector<string>::iterator xIt = objX.begin() + bindVector; xIt != objX.end() && nextNextStep == false; xIt++)
            {
                nextNextStep = true;
                for (vector<string>::iterator zIt = objZ.begin() + bindVector; zIt != objZ.end(); zIt++)
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

    unsigned bind = 0;
    string sAmountOfPoints;
    int amountOfPoints = 0;
    unsigned pairCoordiantesWithEnemy = 0;


    for (vector<string>::iterator typeIt = enType.begin(); typeIt != enType.end(); typeIt++)
    {
        outputFile << "--Enemy--" << endl;
        for (vector<string>::iterator speedIt = enSpeed.begin() + bind; speedIt != enSpeed.end() && speed == false; speedIt++)
        {
            speed = true;
            for (vector<string>::iterator damageIt = enDamage.begin() + bind; damageIt != enDamage.end() && damage == false; damageIt++)
            {
                damage = true;
                for (vector<string>::iterator hpIt = enHP.begin() + bind; hpIt != enHP.end() && hp == false; hpIt++)
                {
                    hp = true;
                    for (vector<string>::iterator rangeIt = enRange.begin() + bind; rangeIt != enRange.end() && range == false; rangeIt++)
                    {
                        range = true;
                        for (vector<string>::iterator stateIt = enState.begin() + bind; stateIt != enState.end() && state == false; stateIt++)
                        {
                            outputFile << typeIt->data() << "," <<
                                speedIt->data() << "," <<
                                damageIt->data() << "," <<
                                hpIt->data() << "," <<
                                rangeIt->data() << "," <<
                                stateIt->data() << endl;
                            state = true;

                            for (vector<string>::iterator numPointIt = waypoint.begin() + bind;
                                numPointIt != waypoint.end() &&
                                numPoint == false; numPointIt++)
                            {
                                sAmountOfPoints = numPointIt->data();
                                amountOfPoints = stoi(sAmountOfPoints);

                                outputFile << amountOfPoints << endl;
                                numPoint = true;

                            }

                            for (unsigned i = 0; i < amountOfPoints; i++)
                            {
                                for (vector<string>::iterator xIt = enX.begin() + pairCoordiantesWithEnemy; xIt != enX.end() && x == false; xIt++)
                                {
                                    x = true;
                                    for (vector<string>::iterator zIt = enZ.begin() + pairCoordiantesWithEnemy; zIt != enZ.end() && z == false; zIt++)
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

void File::load(string level,
    vector<string> &objName,
    vector<string> &objTex,
    vector<string> &objX,
    vector<string> &objZ,
    vector<string> &enType,
    vector<string> &enSpeed,
    vector<string> &enDamage,
    vector<string> &enHP,
    vector<string> &enRange,
    vector<string> &enState,
    vector<string> &waypoint,
    vector<string> &enX,
    vector<string> &enZ)
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
    ifstream myfile(level);

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
                        objX.push_back(posX);
                        continue;
                    }
                    if (x == true)
                    {
                        posZ += pos[i];
                    }
                }
                objZ.push_back(posZ);
                posX = "";
                posZ = "";
            }
            else if (linePos == 3)
            {
                objTex.push_back(line);

            }

            else if (linePos == 4)
            {
                objName.push_back(line);

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
                        enType.push_back(type);
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
                        enSpeed.push_back(speed);
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
                        enDamage.push_back(damage);
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
                        enHP.push_back(hp);
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
                        enRange.push_back(range);
                        continue;
                    }

                    if (getSettings[i] != ',' &&
                        savedState == false &&
                        savedRange == true)
                    {
                        state += getSettings[i];
                    }
                }
                enState.push_back(state);
            }
            else if (linePos == 3)
            {
                waypoint.push_back(line);
                waypoints = stoi(waypoint.back());
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
                        enX.push_back(posX);
                        continue;
                    }

                    if (x == true)
                    {
                        posZ += pos[j];
                    }
                }
                enZ.push_back(posZ);
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

//void File::renderObjects(Scene* sceneName)
//{
//    
//    editor = new SceneEditor();
//    float x, z;
//    int generateObjects = 0;
//    bool nextStep = false;
//    //To check and enable collision according to objects.
//    const string mountain = "OBJ//mountain.obj";
//    const string mountain90 = "OBJ//mountain90.obj";
//    const string mountain180 = "OBJ//mountain180.obj";
//    const string mountain270 = "OBJ//mountain270.obj";
//
//    const string lamp = "OBJ//lamp.obj";
//    const string lamp90 = "OBJ//lamp90.obj";
//    const string lamp180 = "OBJ//lamp180.obj";
//    const string lamp270 = "OBJ//lamp270.obj";
//
//    const string lantern = "OBJ//lantern.obj";
//    const string lantern90 = "OBJ//lantern90.obj";
//    const string lantern180 = "OBJ//lantern180.obj";
//    const string lantern270 = "OBJ//lantern270.obj";
//
//    const string tombstone = "OBJ//tombstone.obj";
//    const string tombstone90 = "OBJ//tombstone90.obj";
//    const string tombstone180 = "OBJ//tombstone180.obj";
//    const string tombstone270 = "OBJ//tombstone270.obj";
//
//    const string tree = "OBJ//tree.obj";
//    const string tree90 = "OBJ//tree90.obj";
//    const string tree180 = "OBJ//tree180.obj";
//    const string tree270 = "OBJ//tree270.obj";
//
//    const string statue1 = "OBJ//statue1.obj";
//    const string statue190 = "OBJ//statue190.obj";
//    const string statue1180 = "OBJ//statue1180.obj";
//    const string statue1270 = "OBJ//statue1270.obj";
//
//    const string statue2 = "OBJ//statue2.obj";
//    const string statue290 = "OBJ//statue290.obj";
//    const string statue2180 = "OBJ//statue2180.obj";
//    const string statue2270 = "OBJ//statue2270.obj";
//
//    for (vector<string>::reverse_iterator objectrItName = editor->objectName.rbegin(); objectrItName != objectName.rend(); objectrItName++)
//    {
//        for (vector<string>::reverse_iterator objectrItX = objectPosX.rbegin() + generateObjects; objectrItX != objectPosX.rend() && nextStep == false; objectrItX++)
//        {
//            nextStep = true;
//            for (vector<string>::reverse_iterator objectrItZ = objectPosZ.rbegin() + generateObjects; objectrItZ != objectPosZ.rend(); objectrItZ++)
//            {
//                x = stof(objectrItX->data());
//                z = stof(objectrItZ->data());
//                modelStack.PushMatrix();
//
//                if (objectrItName->data() == mountain ||
//                    objectrItName->data() == mountain90 ||
//                    objectrItName->data() == mountain180 ||
//                    objectrItName->data() == mountain270)
//                {
//
//                    modelStack.Translate(x, -30.f, z);
//                    if (objectrItName->data() == mountain90)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == mountain180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == mountain270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(100.f, 50.f, 100.f);
//                    RenderMesh(meshListPredefined[MOUNTAIN], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == lamp ||
//                    objectrItName->data() == lamp90 ||
//                    objectrItName->data() == lamp180 ||
//                    objectrItName->data() == lamp270)
//                {
//
//                    modelStack.Translate(x, -30.f, z);
//                    if (objectrItName->data() == lamp90)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == lamp180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == lamp270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(10.f, 10.f, 10.f);
//                    RenderMesh(meshListPredefined[LAMP], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == lantern ||
//                    objectrItName->data() == lantern90 ||
//                    objectrItName->data() == lantern180 ||
//                    objectrItName->data() == lantern270)
//                {
//
//                    modelStack.Translate(x, -30.f, z);
//                    if (objectrItName->data() == lantern90)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == lantern180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == lantern270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(10.f, 10.f, 10.f);
//                    RenderMesh(meshListPredefined[LANTERN], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == tombstone ||
//                    objectrItName->data() == tombstone90 ||
//                    objectrItName->data() == tombstone180 ||
//                    objectrItName->data() == tombstone270)
//                {
//
//                    modelStack.Translate(x, -30.f, z);
//                    if (objectrItName->data() == tombstone90)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == tombstone180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == tombstone270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(10.f, 10.f, 10.f);
//                    RenderMesh(meshListPredefined[TOMBSTONE], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == tree ||
//                    objectrItName->data() == tree90 ||
//                    objectrItName->data() == tree180 ||
//                    objectrItName->data() == tree270)
//                {
//                    modelStack.Translate(x, -30.f, z);
//                    if (objectrItName->data() == tree90)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == tree180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == tree270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(10.f, 10.f, 10.f);
//                    RenderMesh(meshListPredefined[TREE], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == statue1 ||
//                    objectrItName->data() == statue190 ||
//                    objectrItName->data() == statue1180 ||
//                    objectrItName->data() == statue1270)
//                {
//                    modelStack.Translate(x, 0.f, z);
//                    if (objectrItName->data() == statue190)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == statue1180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == statue1270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Scale(20.f, 20.f, 20.f);
//                    RenderMesh(meshListPredefined[STATUE1], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                if (objectrItName->data() == statue2 ||
//                    objectrItName->data() == statue290 ||
//                    objectrItName->data() == statue2180 ||
//                    objectrItName->data() == statue2270)
//                {
//
//                    modelStack.Translate(x, 0.f, z);
//                    if (objectrItName->data() == statue290)
//                    {
//                        modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == statue2180)
//                    {
//                        modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    }
//                    else if (objectrItName->data() == statue2270)
//                    {
//                        modelStack.Rotate(270.f, 0.f, 1.f, 0.f);
//                    }
//                    modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
//                    modelStack.Scale(10.f, 10.f, 10.f);
//                    editor.RenderMesh(meshListPredefined[STATUE2], true);
//                    modelStack.PopMatrix();
//                    break;
//
//                }
//
//                //modelStack.PopMatrix();
//                //break;
//            }
//        }
//        nextStep = false;
//        generateObjects++;
//    }
//}