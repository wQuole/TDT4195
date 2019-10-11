// Got to have some vector, you know
#include "vector"
// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "mesh.hpp"
#include "OBJLoader.hpp"
#include "toolbox.hpp"
#include "sceneGraph.hpp"
// Glm headers
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
// Because I am lazy
using namespace std;
using namespace glm;

// CONSTANTS
GLfloat X_COORD = 0.0f;
GLfloat Y_COORD = 0.0f;
GLfloat Z_COORD = 0.0f;
GLfloat X_ROT = 0.0f;
GLfloat Y_ROT = 0.0f;
GLfloat Z_ROT = 0.0f;
GLfloat COORD_SPEED = 0.663f;
GLfloat ROT_SPEED = 0.663;
GLfloat CURRENT_TIME = 0.00f;
GLfloat HELICOPTER_OFFSET = 0.5;
GLfloat HELICOPTER_ROTOR_SPEED = 100.0f;

string const VERT_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.vert)";
string const FRAG_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.frag)";
string const LUNAR_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\lunarsurface.obj)";
string const HELI_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\helicopter.obj)";


// Declaring functions
GLuint createVAO(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> colors, vector<GLfloat> surfaceNormals);
GLuint createMeshVAO(Mesh meshStruct);
mat4 createviewProjectionMatrix();
mat4 rotateForReferencePoint(SceneNode* node);
SceneNode* createSceneGraph();
void drawSceneNode(SceneNode* node, mat4 viewProjectionMatrix);
void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar);
void spinRotor(SceneNode* node, GLfloat speed, GLdouble elapsedTime, GLuint axis);
void followPath(SceneNode* node, GLfloat elapsedTime);
void basicSetup();


GLuint createVAO(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> colors, vector<GLfloat> surfaceNormals)
{
    auto dataWhichShouldBeCopiedToTheGPU = vertexIndices.size() * sizeof(GLuint);
    auto vectorSizeInBytes = vertexCoords.size() * sizeof(GLfloat);
    auto colorSizeInBytes= colors.size() * sizeof(GLfloat);
    auto normalSizeInBytes= surfaceNormals.size() * sizeof(GLfloat);

    // Vertices
    GLuint vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vectorSizeInBytes, &vertexCoords.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(0);

    // Colors
    GLuint colorBufferID;
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colorSizeInBytes, &colors.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(1);

    // Normals
    GLuint normalBufferID;
    glGenBuffers(1, &normalBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferID);
    glBufferData(GL_ARRAY_BUFFER, normalSizeInBytes, &surfaceNormals.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(2);

    // Index Buffer Object
    GLuint indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataWhichShouldBeCopiedToTheGPU, &vertexIndices.front(), GL_STATIC_DRAW);

    return vertexArrayID;
}


GLuint createMeshVAO(Mesh meshStruct){
    return createVAO(meshStruct.vertices, meshStruct.indices, meshStruct.colours, meshStruct.normals);
}


mat4 createviewProjectionMatrix() {
    mat4x4 Projection_perspective_matrix = perspective(radians(45.0f), 4.0f/3.0f, 1.0f, 10000.0f);

    mat4x4 View_translation_matrix = translate(mat4(1.0f), vec3(X_COORD, Y_COORD, Z_COORD));

    mat4x4 X_rotation_matrix = rotate(radians(X_ROT), vec3(1.0f, 0.0f, 0.0f));
    mat4x4 Y_rotation_matrix = rotate(radians(Y_ROT), vec3(0.0f, 1.0f, 0.0f));

    return Projection_perspective_matrix * X_rotation_matrix * Y_rotation_matrix * View_translation_matrix;
}


mat4 rotateForReferencePoint(SceneNode* node) {
    mat4x4 Translate_to_origin = translate(-(node->referencePoint));

    mat4 X_rotation_matrix = rotate(node->rotation.x, vec3(1.0f, 0.0f, 0.0f));
    mat4 Y_rotation_matrix = rotate(node->rotation.y, vec3(0.0f, 1.0f, 0.0f));
    mat4 Z_rotation_matrix = rotate(node->rotation.z, vec3(0.0f, 0.0f, 1.0f));

    mat4x4 Translate_back_to_referencePoint = translate((node->referencePoint));

    return Translate_back_to_referencePoint * Z_rotation_matrix * Y_rotation_matrix * X_rotation_matrix * Translate_to_origin;
}


SceneNode* createSceneGraph() {

    SceneNode* root = createSceneNode();
    SceneNode* terrainNode = createSceneNode();
    SceneNode* heliBodyNode = createSceneNode();
    SceneNode* helimMainRotorNode = createSceneNode();
    SceneNode* heliTailRotorNode = createSceneNode();
    SceneNode* heliDoorNode = createSceneNode();

    addChild(root, terrainNode);
    addChild(terrainNode, heliBodyNode);
    addChild(heliBodyNode, helimMainRotorNode);
    addChild(heliBodyNode, heliTailRotorNode);
    addChild(heliBodyNode, heliDoorNode);

    // Load Objects: LunarTerrain and Helicopter
    Mesh lunarTerrain = loadTerrainMesh(LUNAR_PATH);
    Helicopter helicopter = loadHelicopterModel(HELI_PATH);

    // Set VAO_ID and VAOIndexCount for objects
    terrainNode->vertexArrayObjectID = createMeshVAO(lunarTerrain);
    terrainNode->VAOIndexCount = lunarTerrain.indices.size();

    heliBodyNode->vertexArrayObjectID = createMeshVAO(helicopter.body);
    heliBodyNode->VAOIndexCount = helicopter.body.indices.size();
    heliBodyNode->referencePoint = vec3(0.0f, 40.0f, 0.0f);

    helimMainRotorNode->vertexArrayObjectID = createMeshVAO(helicopter.mainRotor);
    helimMainRotorNode->VAOIndexCount = helicopter.mainRotor.indices.size();
    helimMainRotorNode->referencePoint = vec3(0.0f, 0.0f, 0.0f);

    heliTailRotorNode->vertexArrayObjectID = createMeshVAO(helicopter.tailRotor);
    heliTailRotorNode->VAOIndexCount = helicopter.tailRotor.indices.size();
    heliTailRotorNode->referencePoint = vec3(0.35, 2.3, 10.4);

    heliDoorNode->vertexArrayObjectID = createMeshVAO(helicopter.door);
    heliDoorNode->VAOIndexCount = helicopter.door.indices.size();

    // Pretty-print the nodes
    printNode(terrainNode);
    printNode(heliBodyNode);
    printNode(helimMainRotorNode);
    printNode(heliTailRotorNode);
    printNode(heliDoorNode);

    return root;
}


void drawSceneNode(SceneNode* node, mat4 viewProjectionMatrix) {
    // Send to Vertex Shader
    mat4x4 ModelViewProjection_Matrix = viewProjectionMatrix*(node->currentTransformationMatrix);
    glUniformMatrix4fv(3, 1, GL_FALSE, value_ptr(ModelViewProjection_Matrix));
    glUniformMatrix4fv(4, 1, GL_FALSE, value_ptr(node->currentTransformationMatrix));

    glBindVertexArray(node->vertexArrayObjectID);
    glDrawElements(GL_TRIANGLES, node->VAOIndexCount, GL_UNSIGNED_INT, nullptr);

    for (SceneNode* child : node->children) {
        drawSceneNode(child, viewProjectionMatrix);
    }
}


void updateSceneNode(SceneNode* node, glm::mat4 transformationThusFar) {
    node->currentTransformationMatrix = transformationThusFar * translate(node->position) * rotateForReferencePoint(node);

    for(SceneNode* child : node->children) {
        updateSceneNode(child, node->currentTransformationMatrix);
    }
}


void spinRotor(SceneNode* node, GLfloat speed, GLdouble elapsedTime, GLuint axis){
    GLfloat timeStep = speed * elapsedTime;

    switch(axis) {
        case 0:
            node->rotation.x += timeStep;
            break;
        case 1:
            node->rotation.y += timeStep;
            break;
        case 2:
            node->rotation.z += timeStep;
            break;
        default:
            break;
    }
}


void followPath(SceneNode* node, GLfloat elapsedTime){
    Heading heading = simpleHeadingAnimation(elapsedTime);
    node->position.x = heading.x;
    node->position.y = cos(elapsedTime)*3.37f;
    node->position.z = heading.z;
    node->rotation = vec3(heading.pitch, heading.yaw, heading.roll*cos(elapsedTime));
}


void basicSetup() {
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);
    // Enable transparency
    glEnable(GL_BLEND);
    // Set default colour after clearing the colour buffer AKA setting the background color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
}


void runProgram(GLFWwindow* window)
{
   basicSetup();

    // Activate shader and bind the Vertex Array
    Gloom::Shader shader;
    shader.makeBasicShader(VERT_PATH, FRAG_PATH);
    shader.activate();

    SceneNode* rootNode = createSceneGraph();
    SceneNode* lunarMap = rootNode->children[0];
    SceneNode* helicopterMainRotor = rootNode->children[0]->children[0]->children[0];
    SceneNode* helicopterTailRotor = rootNode->children[0]->children[0]->children[1];

    // Rendering Loop
    printGLError();
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update the ViewProjectionMatrix
        mat4x4 identityMatrix = mat4(1.0f);
        mat4x4 viewProjectionMatrix = createviewProjectionMatrix();

        GLdouble timeDelta = getTimeDeltaSeconds();
        GLfloat elapsedTime = static_cast<GLfloat>(timeDelta);
        CURRENT_TIME += elapsedTime;

        spinRotor(helicopterMainRotor, HELICOPTER_ROTOR_SPEED, elapsedTime, 1);
        spinRotor(helicopterTailRotor, HELICOPTER_ROTOR_SPEED, elapsedTime, 0);

        for (SceneNode* child : lunarMap->children){
            followPath(child, CURRENT_TIME);
            elapsedTime += HELICOPTER_OFFSET;
        }

//      Update Scene
        updateSceneNode(rootNode, identityMatrix);

        // Draw your scene here
        drawSceneNode(rootNode, viewProjectionMatrix);

        // Check if an OpenGL error occurred, if so print which
        printGLError();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);
    }
    shader.deactivate();
    shader.destroy();
}


void handleKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        X_COORD += COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        X_COORD -= COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Y_COORD -= COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Y_COORD += COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        Z_COORD += COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        Z_COORD -= COORD_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        X_ROT -= ROT_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        X_ROT += ROT_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Y_ROT += ROT_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Y_ROT -= ROT_SPEED;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        X_ROT = 0.0f;
        Y_ROT = 0.0f;
        X_COORD = 0.0f;
        Y_COORD = 0.0f;
        Z_COORD = -2.0f;
    }
}