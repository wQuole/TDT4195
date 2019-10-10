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
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

unsigned int NUM_OF_VERTCOORDS = 3;
GLfloat X_COORD = 0.0f;
GLfloat Y_COORD = 0.0f;
GLfloat Z_COORD = -2.0f;
GLfloat X_ROT = 0.0f;
GLfloat Y_ROT = -0.0f;

string const VERT_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.vert)";
string const FRAG_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.frag)";
string const LUNAR_PATH = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\lunarsurface.obj)";

// Declaring createVAO function
GLuint createVAO(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> colors);

// Defining createVAO function
GLuint createVAO(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> colors)
{
    auto vectorSizeInBytes = vertexCoords.size() * sizeof(GLfloat);
    auto dataWhichShouldBeCopiedToTheGPU = vertexIndices.size() * sizeof(GLuint);
    auto colorSizeInBytes= colors.size() * sizeof(GLfloat);

    // Bind the Vertex Array Object (VAO)
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create Vertex Buffer Object (VBO)
    GLuint vertexBufferID;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vectorSizeInBytes, &vertexCoords.front(), GL_STATIC_DRAW);

    // Set Vertex Attribute Pointer (VAP)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(0);

    // Set index buffer
    GLuint indexBufferID;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataWhichShouldBeCopiedToTheGPU, &vertexIndices.front(), GL_STATIC_DRAW);

    // Create Color Buffer Object
    GLuint colorBufferID;
    glGenBuffers(1, &colorBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferID);
    glBufferData(GL_ARRAY_BUFFER, colorSizeInBytes, &colors.front(), GL_STATIC_DRAW);

    // Set Color Attribute Pointer (CAP)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(1);

    return vertexBufferID;
}

mat4 rotateAndTranslate() {
    // Create Perspective Matrices
    mat4x4 Projection = perspective(radians(45.0f), 4.0f/3.0f, 1.0f, 1000.0f);

    // Translation
    mat4x4 View = translate(mat4(1.0f), vec3(X_COORD, Y_COORD, Z_COORD));
    mat4x4 ViewTransposed = transpose(View);

    // Rotation
    mat4x4 X_rotMatrix = rotate(radians(X_ROT), vec3(1.0f, 0.0f, 0.0f));
    mat4x4 Y_rotMatrix = rotate(radians(Y_ROT), vec3(0.0f, 1.0f, 0.0f));

    return X_rotMatrix * Y_rotMatrix * ViewTransposed * Projection;
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

    Mesh lunarTerrain = loadTerrainMesh(LUNAR_PATH);
    GLuint VAO_ID = createVAO(lunarTerrain.vertices, lunarTerrain.indices, lunarTerrain.colours);
    glBindVertexArray(VAO_ID);

    GLuint numberOfVertices = (int) lunarTerrain.vertices.size() / NUM_OF_VERTCOORDS;
    // Rendering Loop
    printGLError();
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transform matrix
        mat4x4 TransformedMatrix = rotateAndTranslate();

        // Send to Vertex Shader
        glUniformMatrix4fv(3, 1, GL_FALSE, value_ptr(TransformedMatrix));

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT, nullptr);

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
    // Use escape key for terminating the GLFW window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        X_COORD += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        X_COORD -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        Y_COORD -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        Y_COORD += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Z_COORD -= 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Z_COORD += 0.01f;
    }
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        X_ROT -= 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        X_ROT += 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Y_ROT += 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        Y_ROT -= 1.0f;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        X_ROT = 0.0f;
        Y_ROT = 0.0f;
        X_COORD = 0.0f;
        Y_COORD = 0.0f;
        Z_COORD = -2.0f;
    }
}