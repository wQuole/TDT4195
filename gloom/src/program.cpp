#include <numeric>
#include <cmath>
#include "vector"
// Local headers
#include "program.hpp"
#include "gloom/gloom.hpp"
#include "gloom/shader.hpp"
// Glm headers
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;

unsigned int NUM_OF_VERTCOORDS = 3;
GLfloat X_COORD = 0.0f;
GLfloat Y_COORD = 0.0f;
GLfloat Z_COORD = -2.0f;
GLfloat X_ROT = 0.0f;
GLfloat Y_ROT = -0.0f;

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

glm::mat4 rotateAndTranslate() {
    // Create and perspective Matrices
    glm::mat4x4 Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 1.0f, 100.0f); // https://glm.g-truc.net/0.9.9/index.html

    // Translation
    glm::mat4x4 View = glm::translate(glm::mat4(1.0f), glm::vec3(X_COORD, Y_COORD, Z_COORD));
    glm::mat4x4 ViewTransposed = glm::transpose(View);

    // Rotation
    glm::mat4x4 X_rotMatrix = glm::rotate(glm::radians(X_ROT), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4x4 Y_rotMatrix = glm::rotate(glm::radians(Y_ROT), glm::vec3(0.0f, 1.0f, 0.0f));

    return X_rotMatrix * Y_rotMatrix * ViewTransposed * Projection;
}

void runProgram(GLFWwindow* window)
{
    // Enable depth (Z) buffer (accept "closest" fragment)
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_GREATER);

    // Configure miscellaneous OpenGL settings
//    glEnable(GL_CULL_FACE);

    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    // Set default colour after clearing the colour buffer AKA setting the background color
//    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    vector<GLfloat> threeOverlappingTriangles {
            -0.60f, -0.25f, 0.7f,
            -0.10f, -0.25f, 0.7f,
            -0.35f,  0.3f, 0.7f,

            -0.40f, -0.25f, 0.2f,
            0.10f, -0.25f, 0.2f,
            -0.15f,  0.3f, 0.2f,

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

    string vertPath = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.vert)";
    string fragPath = R"(C:\Users\wquole\code\cppCode\TDT4195\gloom\shaders\simple.frag)";

    // Fill the indices
    vector<GLuint> triangleIndices(threeOverlappingTriangles.size());
    iota(triangleIndices.begin(), triangleIndices.end(), 0);
    GLuint numberOfVertices = (int) threeOverlappingTriangles.size() / NUM_OF_VERTCOORDS;

    // Activate shader and bind the Vertex Array
    Gloom::Shader shader;
    shader.makeBasicShader(vertPath,fragPath);
    shader.activate();

    GLuint arrayID = createVAO(threeOverlappingTriangles, triangleIndices, threeOverlappingTrianglesColors);
    glBindVertexArray(arrayID);

    // Rendering Loop
    printGLError();
    GLuint increment =  0;
    while (!glfwWindowShouldClose(window))
    {
        // Clear colour and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transform matrix
        glm::mat4x4 TransformedMatrix = rotateAndTranslate();

        // Send to Vertex Shader
        glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(TransformedMatrix));

        // Modify oscillation
        increment++;
        if (increment > 360) increment = 0;
        glUniform1f(2, sin(increment*(3.14/180)));

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