#include "SceneText.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "Application.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "LoadOBJ.h"

#include "shader.hpp"
#include "Utility.h"

#include <iostream>

using std::cout;
using std::cin;

SceneText::SceneText()
{
}

SceneText::~SceneText()
{
}

void SceneText::Init()
{
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

    meshList[GEO_AXES] = MeshBuilder::GenerateAxes("axes", 1, 1, 1);

    meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 20, 20, 1);
    meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball", Color(0.5f, 0.f, 0.f), 20, 20, 1);

    meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f, 1.f);
    meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

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

    //meshList[GEO_ALEX] = MeshBuilder::GenerateQuad("alex", Color(1, 1, 1), 1.f, 1.f);
    //meshList[GEO_ALEX]->textureID = LoadTGA("Image//alex.tga");

    //meshList[GEO_JENNA] = MeshBuilder::GenerateQuad("jenna", Color(1, 1, 1), 1.f, 1.f);
    //meshList[GEO_JENNA]->textureID = LoadTGA("Image//jenna.tga");

    //meshList[GEO_MIA] = MeshBuilder::GenerateQuad("mia", Color(1, 1, 1), 1.f, 1.f);
    //meshList[GEO_MIA]->textureID = LoadTGA("Image//mia.tga");

    //meshList[GEO_FELIX] = MeshBuilder::GenerateQuad("felix", Color(1, 1, 1), 1.f, 1.f);
    //meshList[GEO_FELIX]->textureID = LoadTGA("Image//felix.tga");

    //meshList[GEO_CHAIR] = MeshBuilder::GenerateOBJ("chair", "OBJ//chair.obj");
    //meshList[GEO_CHAIR]->textureID = LoadTGA("Image//chair.tga");

    //meshList[GEO_DART] = MeshBuilder::GenerateOBJ("chair", "OBJ//dart.obj");
    //meshList[GEO_DART]->textureID = LoadTGA("Image//dart.tga");

    //meshList[GEO_DARTBOARD] = MeshBuilder::GenerateOBJ("chair", "OBJ//dartboard.obj");
    //meshList[GEO_DARTBOARD]->textureID = LoadTGA("Image//dartboard.tga");

    //meshList[GEO_DOORMAN] = MeshBuilder::GenerateOBJ("chair", "OBJ//doorman.obj");
    //meshList[GEO_DOORMAN]->textureID = LoadTGA("Image//doorman.tga");

    //meshList[GEO_SHOE] = MeshBuilder::GenerateOBJ("chair", "OBJ//shoe.obj");
    //meshList[GEO_SHOE]->textureID = LoadTGA("Image//shoe.tga");

    //meshList[GEO_WINEBOTTLE] = MeshBuilder::GenerateOBJ("chair", "OBJ//winebottle.obj");
    //meshList[GEO_WINEBOTTLE]->textureID = LoadTGA("Image//winebottle.tga");

    meshList[GEO_VEHICLE] = MeshBuilder::GenerateOBJ("vehicle", "OBJ//vehicle.obj");
    meshList[GEO_VEHICLE]->textureID = LoadTGA("Image//vehicle.tga");

    meshList[GEO_PORTAL_1] = MeshBuilder::GenerateOBJ("portal_1", "OBJ//portal_1.obj");
    meshList[GEO_PORTAL_1]->textureID = LoadTGA("Image//portal_1.tga");

    meshList[GEO_PORTAL_2] = MeshBuilder::GenerateOBJ("portal_2", "OBJ//portal_2.obj");
    meshList[GEO_PORTAL_2]->textureID = LoadTGA("Image//portal_2.tga");

    meshList[GEO_TRAFFIC_1] = MeshBuilder::GenerateOBJ("traffic_1", "OBJ//traffic_1.obj");
    meshList[GEO_TRAFFIC_1]->textureID = LoadTGA("Image//traffic_1.tga");

    meshList[GEO_TRAFFIC_2] = MeshBuilder::GenerateOBJ("traffic_2", "OBJ//traffic_2.obj");
    meshList[GEO_TRAFFIC_2]->textureID = LoadTGA("Image//traffic_2.tga");

    meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJ("building", "OBJ//building.obj");
    meshList[GEO_BUILDING]->textureID = LoadTGA("Image//building.tga");

    meshList[GEO_FLOOR] = MeshBuilder::GenerateOBJ("floor", "OBJ//floor.obj");
    meshList[GEO_FLOOR]->textureID = LoadTGA("Image//floor.tga");

    meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
    meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");
    //meshList[GEO_TEXT]->material.kAmbient.Set(0.f, 1.f, 0.f);
    //meshList[GEO_TEXT]->material.kDiffuse.Set(0.6f, 0.6f, 0.6f);
    //meshList[GEO_TEXT]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);

    rotateAngle = 1.0f;
    translateX = 0.0f;
    translateX2 = -40.0f;
    translateY = 28.0f;
    translateY2 = 29.0f;
    translateY3 = 30.0f;
    scaleAll = 1.0f;
    //camera.Init(Vector3(40, 30, 30), Vector3(0, 0, 0), Vector3(0, 1, 0));
    //Prevent Jerk
    camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));

    //Projection
    Mtx44 projection;
    projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
    projectionStack.LoadMatrix(projection);

    light[0].type = Light::LIGHT_SPOT;
    light[0].position.Set(0, 20, 0);
    light[0].color.Set(1, 1, 1);
    light[0].power = 1;
    light[0].kC = 1.f;
    light[0].kL = 0.01f;
    light[0].kQ = 0.001f;
    light[0].cosCutoff = cos(Math::DegreeToRadian(45));
    light[0].cosInner = cos(Math::DegreeToRadian(30));
    light[0].exponent = 3.f;
    light[0].spotDirection.Set(0.f, 1.f, 0.f);

    light[1].type = Light::LIGHT_SPOT;
    light[1].position.Set(20, 20, 0);
    light[1].color.Set(1.f, 0.f, 0.f);
    light[1].power = 1;
    light[1].kC = 1.f;
    light[1].kL = 0.01f;
    light[1].kQ = 0.001f;
    light[1].cosCutoff = cos(Math::DegreeToRadian(45));
    light[1].cosInner = cos(Math::DegreeToRadian(30));
    light[1].exponent = 3.f;
    light[1].spotDirection.Set(0.f, 1.f, 0.f);

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

    // Make sure you pass uniform parameters after glUseProgram()
    glUniform1i(m_parameters[U_NUMLIGHTS], 1);


}

void SceneText::Update(double dt)
{
    static const float LSPEED = 10.f;
    srand(time(NULL));
    unsigned int randspeed = rand() % 100 + 1;
    unsigned int randspeed2 = rand() % 100 + 1;
    unsigned int randspeed3 = rand() % 100 + 1;

    int randx = rand() % 40 - 40;
    int randx2 = rand() % 40 - 40;
    int randx3 = rand() % 40 - 40;

    //Static is first time called and will not be called again.
    static float translateDirection = 1;
    static float translateDown = 1;

    if (Application::IsKeyPressed('1'))
    {
        glEnable(GL_CULL_FACE);
    }

    if (Application::IsKeyPressed('2'))
    {
        glDisable(GL_CULL_FACE);
    }

    if (Application::IsKeyPressed('3'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
    }

    if (Application::IsKeyPressed('4'))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
    }

    if (Application::IsKeyPressed('5'))
    {
        lightswitch = true;
    }

    if (Application::IsKeyPressed('6'))
    {
        lightswitch = false;
    }

    if (Application::IsKeyPressed('7'))
    {
        light[0].type = Light::LIGHT_POINT;
        glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
        light[1].type = Light::LIGHT_POINT;
        glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
        //to do: switch light type to POINT and pass the information to shader
    }
    if (Application::IsKeyPressed('8'))
    {
        light[0].type = Light::LIGHT_DIRECTIONAL;
        glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
        light[1].type = Light::LIGHT_DIRECTIONAL;
        glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
        //to do: switch light type to DIRECTIONAL and pass the information to shader
    }
    if (Application::IsKeyPressed('9'))
    {
        light[0].type = Light::LIGHT_SPOT;
        glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
        light[1].type = Light::LIGHT_SPOT;
        glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
        //to do: switch light type to SPOT and pass the information to shader
    }



    if (Application::IsKeyPressed(VK_SPACE))
    {
        rotateAngle += (float)(500 / translateDirection * dt);
    }

    rotateAngle += (float)(40 * translateDirection * dt);
    if (rotateAngle > 360)
    {
        //translateDirection = -1;
        rotateAngle = -360;

    }
    else if (rotateAngle <= -360)
    {
        translateDirection = 1;
    }

    camera.Update(dt);


    if (Application::IsKeyPressed('I'))
        light[0].position.z -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('K'))
        light[0].position.z += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('J'))
        light[0].position.x -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('L'))
        light[0].position.x += (float)(LSPEED * dt);
    if (Application::IsKeyPressed('O'))
        light[0].position.y -= (float)(LSPEED * dt);
    if (Application::IsKeyPressed('P'))
        light[0].position.y += (float)(LSPEED * dt);
}

void SceneText::Render()
{
    srand(time(NULL));
    unsigned int change = rand() % 3;
    unsigned int change2 = rand() % 3;
    unsigned int change3 = rand() % 3;

    unsigned int randscale = rand() % 10 + 1;
    unsigned int randscale2 = rand() % 10 + 1;
    unsigned int randscale3 = rand() % 10 + 1;

    //Clear color & depth buffer every frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    Mtx44 MVP;

    viewStack.LoadIdentity(); //No need camera for now, set it at World's origin
    viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
    modelStack.LoadIdentity();

    //Passing light position to shader
    Position lightPosition_cameraspace = viewStack.Top() * light[0].position;

    //glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

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


    if (light[1].type == Light::LIGHT_DIRECTIONAL)
    {
        Vector3 lightDir(light[1].position.x, light[1].position.y, light[1].position.z);
        Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
    }
    else if (light[1].type == Light::LIGHT_SPOT)
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
        glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
        Vector3 spotDirection_cameraspace = viewStack.Top() * light[1].spotDirection;
        glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
    }
    else
    {
        Position lightPosition_cameraspace = viewStack.Top() * light[1].position;
        glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
    }


    //Generate Axes
    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top(); //Remember, matrix multiplication is the other way around
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    RenderMesh(meshList[GEO_AXES], false);

    modelStack.PushMatrix();
    modelStack.Translate(light[0].position.x, light[0].position.y, light[0].position.z);
    RenderMesh(meshList[GEO_LIGHTBALL], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(light[1].position.x, light[1].position.y, light[1].position.z);
    RenderMesh(meshList[GEO_LIGHTBALL2], false);
    modelStack.PopMatrix();

    //modelStack.PushMatrix(); //Quad
    //modelStack.Scale(50.f, 1, 50.f);

    //glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
    //RenderMesh(meshList[GEO_QUAD], false);
    //modelStack.PopMatrix(); //End Quad

    RenderSkybox();
    //renderAlex();
    //renderJenna();
    //renderMia();
    //renderFelix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(20.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_CHAIR], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(15.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_DART], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(10.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_DARTBOARD], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(5.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_DOORMAN], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(10.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_SHOE], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    //modelStack.Translate(15.f, 0.f, 0.f);
    //RenderMesh(meshList[GEO_WINEBOTTLE], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    //modelStack.Translate(0.f, -10.f, 0.f);

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////   modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_VEHICLE], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_PORTAL_1], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_PORTAL_2], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_TRAFFIC_1], true);
    //modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_TRAFFIC_2], true);
    //modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    //modelStack.Translate(-45.f, -130.f, -150.f);
    //modelStack.Scale(10.f, 10.f, 10.f);
    RenderMesh(meshList[GEO_BUILDING], true);
    modelStack.PopMatrix();

    //modelStack.PushMatrix();
    ////scale, translate, rotate
    ////modelStack.Translate(-45.f, -130.f, -150.f);
    ////modelStack.Scale(10.f, 10.f, 10.f);
    //RenderMesh(meshList[GEO_FLOOR], true);
    //modelStack.PopMatrix();

    //modelStack.PopMatrix();

    modelStack.PushMatrix();
    //No transform needed
    RenderTextOnScreen(meshList[GEO_TEXT], "Hello World", Color(0, 1, 0), 5, 0, 0);
    //RenderTextOnScreen(meshList[GEO_TEXT], 1/dt, Color(0, 1, 0), 5, 0, 1);


    modelStack.PopMatrix();

}

void SceneText::RenderSkybox()
{
    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(0.f, 0.f, 498.f);
    modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_FRONT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(0.f, 0.f, -498.f);
    //modelStack.Rotate(180.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_BACK], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(0.f, 498.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(180.f, 1.f, 0.f, 0.f);
    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_TOP], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(0.f, -498.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_BOTTOM], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(498.f, 0.f, 0.f);
    modelStack.Rotate(-90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_LEFT], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    //scale, translate, rotate
    modelStack.Translate(-498.f, 0.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);

    modelStack.Scale(1000.f, 1000.f, 1000.f);
    RenderMesh(meshList[GEO_RIGHT], false);
    modelStack.PopMatrix();
}

void SceneText::renderAlex()
{
    modelStack.PushMatrix();
    modelStack.Translate(0.f, 0.f, -200.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_ALEX], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0.f, 0.f, -200.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(-100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_ALEX], false);
    modelStack.PopMatrix();
}

void SceneText::renderJenna()
{
    modelStack.PushMatrix();
    modelStack.Translate(0.f, 0.f, 200.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_JENNA], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(0.f, 0.f, 200.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(-100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_JENNA], false);
    modelStack.PopMatrix();
}

void SceneText::renderMia()
{
    modelStack.PushMatrix();
    modelStack.Translate(-200.f, 0.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_MIA], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(-200.f, 0.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(-100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_MIA], false);
    modelStack.PopMatrix();
}

void SceneText::renderFelix()
{
    modelStack.PushMatrix();
    modelStack.Translate(200.f, 0.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_FELIX], false);
    modelStack.PopMatrix();

    modelStack.PushMatrix();
    modelStack.Translate(200.f, 0.f, 0.f);
    modelStack.Rotate(90.f, 0.f, 1.f, 0.f);
    modelStack.Rotate(90.f, 1.f, 0.f, 0.f);
    modelStack.Scale(-100.f, 100.f, 100.f);
    RenderMesh(meshList[GEO_FELIX], false);
    modelStack.PopMatrix();
}

void SceneText::RenderMesh(Mesh *mesh, bool enableLight)
{
    Mtx44 MVP, modelView, modelView_inverse_transpose;

    MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
    modelView = viewStack.Top() * modelStack.Top();
    glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
    if (enableLight)
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 1);
        modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
        glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

        //load material
        glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
        glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
        glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
        glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
    }
    else
    {
        glUniform1i(m_parameters[U_LIGHTENABLED], 0);
    }

    if (mesh->textureID > 0)
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->textureID);
        glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
    }
    else
    {
        glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
    }

    mesh->Render(); //this line should only be called once
    if (mesh->textureID > 0)
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void SceneText::RenderText(Mesh* mesh, std::string text, Color color)
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
        characterSpacing.SetToTranslation(i * 1.0f, 0.f, 0); //1.0f is the spacing of each character, you may change this value
        Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
        glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

        mesh->Render((unsigned)text[i] * 6, 6);
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
    glEnable(GL_DEPTH_TEST);
}

void SceneText::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
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
        characterSpacing.SetToTranslation(i * 1.0f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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



void SceneText::Exit()
{
    // Cleanup VBO here
    glDeleteVertexArrays(1, &m_vertexArrayID);

    glDeleteProgram(m_programID);
}