#ifndef FILE_SYSTEM
#define FILE_SYSTEM
#include <vector>
#include <string>
#include "..\SceneEditor.h"
#include "..\Enemy.h"

using std::string;
using std::vector;

class File
{
    enum OBJECTTYPES
    {
        NOTHING = 0,
        OBJECT,
        ENEMY,
    };
public:
    File();
    ~File();
    void save(string level,
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
        vector<string> enZ);

    void load(string level,
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
        vector<string> &enZ);

    //Check if type to load is Object or Enemy
    OBJECTTYPES checkType = NOTHING;

};

#endif