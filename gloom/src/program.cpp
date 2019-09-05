// Local headers
#include <numeric>
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
#include "vector"

using namespace std;

GLuint setUpVAOTriangle(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices);

GLuint setUpVAOTriangle(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices)
{
    auto vectorSizeInBytes = vertexCoords.size() * sizeof(GLfloat);
    auto dataWhichShouldBeCopiedToTheGPU = vertexIndices.size() * sizeof(GLuint);

    // Bind the Vertex Array Object (VAO)
    GLuint vertexArrayID = 0;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // Create Vertex Buffer Object (VBO)
    GLuint vertexBufferID = 0;
    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vectorSizeInBytes, &vertexCoords.front(), GL_STATIC_DRAW);

    // Set Vertex Attribute Pointer (VAP)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, GL_ZERO, nullptr);
    glEnableVertexAttribArray(0);

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
            0.6, -0.6, 0.3,
            -0.6, -0.6, 0.2,
            0.0, 0.6, 0.1,

            -1.0, -1.0, .0,
            1.0, -1.0, 0.0,
            0.0, 1.0, 0.0,
    };


    vector<GLuint> triangleIndices(triangleCoordinates.size());

    // Fill the indices
    iota(triangleIndices.begin(), triangleIndices.end(), 0);

    // Create "arrayID" for VAO
    GLuint triAngleVAO = setUpVAOTriangle(triangleCoordinates, triangleIndices);

    // Activate shader and bind the Vertex Array
    shader.activate();
    glBindVertexArray(triAngleVAO);

    uint numberOfVertices = (int) triangleCoordinates.size()/3;
    // Rendering Loop
    printGLError();
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, numberOfVertices , GL_UNSIGNED_INT, nullptr);

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
