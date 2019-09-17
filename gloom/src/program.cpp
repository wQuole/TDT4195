// Local headers
#include <numeric>
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vector"

using namespace std;

unsigned int NUM_OF_VERTCOORDS = 3;

GLuint setUpVAOTriangle(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> vertexColors);

GLuint setUpVAOTriangle(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> vertexColors)
{
    auto vectorSizeInBytes = vertexCoords.size() * sizeof(GLfloat);
    auto dataWhichShouldBeCopiedToTheGPU = vertexIndices.size() * sizeof(GLuint);
    auto colorSizeInBytes = vertexColors.size() * sizeof(GLfloat);

    // Bind the Vertex Array Object (VAO)
    GLuint vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create Vertex Buffer Object (VBO)
    GLuint vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vectorSizeInBytes, &vertexCoords.front(), GL_STATIC_DRAW);

    GLuint colorBufferID = 1;
    glGenBuffers(1, &colrBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBuffeData(GL_ARRAY_BUFFER, vectorSizeInBytes, &vertexColors.front(), GL_STATIC_DRAW); 

    // Set Vertex Attribute Pointer (VAP)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(1);

    // Set index buffer
    GLuint indexBufferID = 0;
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataWhichShouldBeCopiedToTheGPU, &vertexIndices.front(), GL_STATIC_DRAW);

    return vertexBufferID;
}



void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);                                     ;

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Set default colour after clearing the colour buffer
    glClearColor(0.3f, 0.5f, 0.8f, 1.0f);

    // Set up your scene here (create Vertex Array Objects, etc.)
    string vertPath = "/Users/wquole/CLionProjects/TDT4195/gloom/shaders/simple.vert"; // "../gloom/shaders/simple.vert"
    string fragPath = "/Users/wquole/CLionProjects/TDT4195/gloom/shaders/simple.frag"; // "../gloom/shaders/simple.frag"
    Gloom::Shader shader;
    shader.makeBasicShader(vertPath,
                           fragPath);
    vector<GLfloat> triangleCoordinates {
        0.6f, -0.8f, -1.2f,
        0.0f, 0.4f, 0.0f,
        -0.8f,-0.2f, 1.2f
    };

    vector<GLfloat> fiveTrianglesCoordinates {
         -1.0f, 0.2f, 0.0f,
         -0.8f, 0.0f, 0.0f,
         -0.6f, 0.2f, 0.0f,
 
         -0.6f, 0.2f, 0.0f,
         -0.4f, 0.0f, 0.0f,
         -0.2f, 0.2f, 0.0f,
 
         -0.2f, 0.4f, 0.0f,
         0.0f, -0.1f, 0.0f,
         0.2f, 0.4f, 0.0f,
 
         0.2f, 0.2f, 0.0f,
         0.4f, 0.0f, 0.0f,
         0.6f, 0.2f, 0.0f,
 
         0.6f, 0.2f, 0.0f,
         0.8f, 0.0f, 0.0f,
         1.0f, 0.2f, 0.0f
    };

    vector<GLfloat> prettyColrs {
        -0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,

         0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,

         0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,
         0.0f, 0.5f, 0.0f, 0.8f,
    }

    // Initialize the indices for the triangles
    vector<GLuint> triangleIndices(fiveTrianglesCoordinates.size());

    // Fill the indices
    iota(triangleIndices.begin(), triangleIndices.end(), 0);

    // Create "arrayID" for VAO
    GLuint triAngleVAO = setUpVAOTriangle(triagnleCoordinates, triangleIndices, prettyColors);

    // Activate shader and bind the Vertex Array Object
    shader.activate();
    glBindVertexArray(triAngleVAO);

    uint numberOfVertices = (int) fiveTrianglesCoordinates.size() / NUM_OF_VERTCOORDS;
    // Rendering Loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, numberOfVertices , GL_UNSIGNED_INT, nullptr);

        // Check if an OpenGL error occurred, if so print which
        printGLError();

        // Handle other events
        glfwPollEvents();
        handleKeyboardInput(window);

        // Flip buffers
        glfwSwapBuffers(window);

    }
    // Remove (glUseProgram(0))and "de-allocate" (glDeleteProgram(mProgram))
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
}
