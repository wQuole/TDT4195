#include <numeric>
#include <cmath>
#include "vector"
// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"

using namespace std;

unsigned int NUM_OF_VERTCOORDS = 3;

GLuint createVAO(vector<GLfloat> vertexCoords, vector<GLuint> vertexIndices, vector<GLfloat> colors);
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


void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_GREATER);

    // Configure miscellaneous OpenGL settings
    glEnable(GL_CULL_FACE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set default colour after clearing the colour buffer AKA setting the background color
//    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    vector<GLfloat> threeOverlappingTriangles {
            -0.60f, -0.25f, -0.5f,
            -0.10f, -0.25f, -0.5f,
            -0.35f,  0.3f, -0.5f,

            -0.40f, -0.25f, 0.0f,
            0.10f, -0.25f, 0.0f,
            -0.15f,  0.3f, 0.0f,

            -0.20f, -0.25f, 0.5f,
            0.30f, -0.25f, 0.5f,
            0.05f,  0.3f, 0.5f,
    };

    vector<GLfloat> threeOverlappingTrianglesColors = {
            0.5f, 0.0f, 0.0f, 0.5f,
            0.5f, 0.0f, 0.0f, 0.5f,
            0.5f, 0.0f, 0.0f, 0.5f,

            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,

            0.0f, 0.0f, 0.5f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,

    };

    vector<GLfloat> fiveTriangles {
            -0.9f, -0.3f, 0.0f,
            -0.7f, 0.3f, 0.0f,
            -0.5f, -0.3f, 0.0f,

            -0.6f, 0.9f, 0.0f,
            -0.3f, -0.4f, 0.0f,
            -0.1f, 0.9f, 0.0f,

            -0.2f, -0.4f, 0.0f,
            0.0f, 0.3f, 0.0f,
            0.2f, -0.4f, 0.0f,

            0.1f, 0.9f, 0.0f,
            0.3f, -0.4f, 0.0f,
            0.6f, 0.9f, 0.0f,

            0.5f, -0.3f, 0.0f,
            0.7f, 0.3f, 0.0f,
            0.9f, -0.3f, 0.0f,
    };

    vector<GLfloat> fiveTrianglesColors = {
            0.5f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,

            0.5f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,

            0.5f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,

            0.5f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,

            0.5f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 0.5f,
            0.0f, 0.0f, 0.5f, 0.5f,
    };


    // Set up your scene here (create Vertex Array Objects, etc.)
    // string vertPath = "/Users/wquole/CLionProjects/TDT4195/gloom/shaders/simple.vert"; // MacOS
    // string fragPath = "/Users/wquole/CLionProjects/TDT4195/gloom/shaders/simple.frag"; // MacOS
    string vertPath = "C:\\Users\\wquole\\code\\cppCode\\TDT4195\\gloom\\shaders\\simple.vert"; // Windows
    string fragPath = "C:\\Users\\wquole\\code\\cppCode\\TDT4195\\gloom\\shaders\\simple.frag"; // Windows
    Gloom::Shader shader;
    shader.makeBasicShader(vertPath,fragPath);

    // Fill the indices
    vector<GLuint> triangleIndices(threeOverlappingTriangles.size());
    iota(triangleIndices.begin(), triangleIndices.end(), 0);

    // Create "arrayID" for VAO
    GLuint arrayID = createVAO(threeOverlappingTriangles, triangleIndices, threeOverlappingTrianglesColors);

    // Activate shader and bind the Vertex Array
    shader.activate();
    glBindVertexArray(arrayID);

    GLuint numberOfVertices = (int) threeOverlappingTriangles.size() / NUM_OF_VERTCOORDS;
    // Rendering Loop
    printGLError();
    GLuint increment =  0;
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw your scene here
        glDrawElements(GL_TRIANGLES, numberOfVertices, GL_UNSIGNED_INT, nullptr);

        // Modify oscillation
        increment++;
        if (increment > 360) increment = 0;
        glUniform1f(2, sin(increment*(3.14/180)));

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
}