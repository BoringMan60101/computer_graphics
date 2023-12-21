#include <cstdio>
#include <string>

#include <cstdarg>
#include <cstdlib>
#include <sys/stat.h>
#include <fcntl.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <ctime>
#include <cmath>
#define _USE_MATH_DEFINICES

const unsigned int WIDTH = 800, HEIGHT = 600;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

GLchar *glShaderLoad(char *path, int *length);

void getShader(GLuint *shader, char *path, int typeShader);

void getShaderProgram(GLuint *shaderProgram, int n, ...);


float myRand() {
    return  rand() / (RAND_MAX + 1.0);
}

float Fx(double u) {
    return cos(4.0 * M_PI * u) * 0.75;
    //return u - 0.5;
}
float Fy(double u) {
    return sin(4.0 * M_PI * u) * 0.75;
    //return 0.0;
}
float Fz(double u) {
    //return 0.0;
    return -0.7 + 1.4*u; // U in [-0.7 : 0.7]
}


// Global variables handels ratotion with 'callback function'
glm::vec3 rotationAxisVector = glm::vec3(0, 1, 0);
float rotationAngle = 0.0f; 

int main()
{
    // Configure GLFW (OpenGL FrameWork library)  
    // Init GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFW window object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CG_P6_Ruled_Surface", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, keyCallback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to set up the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);


    // Compile and link shaders 
    // ------------------------
    GLuint vertexShader;
    getShader(&vertexShader, (char *)"shaders/vertex.glsl", GL_VERTEX_SHADER);
    //GLuint geometryShader;
    //getShader(&geometryShader, (char *)"shaders/geometry.glsl", GL_GEOMETRY_SHADER);
    GLuint fragmentShader;
    getShader(&fragmentShader, (char *)"shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram;
    //getShaderProgram(&shaderProgram, 3, &vertexShader, &geometryShader, &fragmentShader);
    getShaderProgram(&shaderProgram, 2, &vertexShader, &fragmentShader);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    srand(time(nullptr));

    // setting up surface vertexes data
    const int Nu = 100, Nv = 100;
    const double du = 1.0 / Nu, dv = 1.0/ Nv;
    float points[3 * (Nu+1) * (Nv+1)]; 
    float colors[3 * (Nu+1) * (Nv+1)]; 
    GLuint indices[2 * 3 * Nu * Nv];

    const glm::vec3 P1 = glm::vec3(0.0, 0.0, 0.0);
    const glm::vec3 P2 = glm::vec3(0.0, 0.0, 0.3);

    // evaluating points coordinates 
    for(int i = 0; i <= Nu; i++)
        for(int j = 0; j <= Nv; j++)
        {
            double u = i*du;
            double v = j*dv;
            int idx = i*3*(Nv+1) + 3*j;
            points[idx + 0] = Fx(u) + (P1.x + (P2.x - P1.x)*v); // X_ij = X(Ui, Vj)
            points[idx + 1] = Fy(u) + (P1.y + (P2.y - P1.y)*v); // Y_ij = Y(Ui, Vj)
            points[idx + 2] = Fz(u) + (P1.z + (P2.z - P1.z)*v); // Z_ij = Z(Ui, Vj)
        }

    // setting points colors 
    for(int i = 0; i <= Nu; i++)
        for(int j = 0; j <= Nv; j++)
        {
            int idx = i*3*(Nv+1) + 3*j;
            //colors[idx + 0] = myRand(); // Red
            //colors[idx + 1] = myRand(); // Green
            //colors[idx + 2] = myRand(); // Blue
            colors[idx + 0] = 0.25; // Red
            colors[idx + 1] = 0.75; // Green
            colors[idx + 2] = 0.5; // Blue
        }
    
    
    // evaluating indices for surface's triangles  
    for(int k = 0, i = 0; i <= Nu - 1; i++)
        for(int j = 0; j <= Nv - 1; j++)
        {
            int idx = i*(Nv + 1) + j;

            indices[k + 0] = idx;
            indices[k + 1] = idx + Nv + 1;
            indices[k + 2] = idx + Nv + 2;

            indices[k + 3] = idx;
            indices[k + 4] = idx + 1;
            indices[k + 5] = idx + Nv + 2;
            
            k += 6;
        }
    

    // vertex arrays routine
    unsigned int pointsVBO, colorsVBO, surfaceVAO, surfaceTrianglesIBO;

    glGenVertexArrays(1, &surfaceVAO);
    glBindVertexArray(surfaceVAO);

    // Position attribute
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    // Color attribute
    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    glGenBuffers(1, &surfaceTrianglesIBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceTrianglesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw points
        glUseProgram(shaderProgram);


        // Diagonal matrix might be used for scaling transformations
        glm::mat4 modelMat(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f); 
        //model = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        
        // 1st variant
        //float angle = 0.0f;
        float angle = 0.5f * (float)glfwGetTime();
        modelMat = glm::rotate(modelMat, angle, glm::vec3(1.0, 0, 0));
        
        // 2nd variant
        //modelMat = glm::rotate(modelMat, rotationAngle, rotationAxisVector);
        GLint ulMatModel = glGetUniformLocation(shaderProgram, "modelMatrix"); // ul - uniform location
        glUniformMatrix4fv(ulMatModel, 1, GL_FALSE, &modelMat[0][0]);



        // Drawing with IBO indices
        glBindVertexArray(surfaceVAO);
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);

        // Alternative (IBO object and an array with indexes are not used here)
        // Automative triangles generation may be unsuitable 
        //glBindVertexArray(surfaceVAO);
        //glDrawArrays(GL_TRIANGLE_FAN, 0, (Nu+1)*(Nv+1));


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &surfaceVAO);
    glDeleteBuffers(1, &pointsVBO);
    glDeleteBuffers(1, &colorsVBO);
    glDeleteBuffers(1, &surfaceTrianglesIBO);
    
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    //std::cout<< "key= " << key << " action= " << action << " scancode=" << scancode;
    //std::cout<< " GLFW_PRESS= " << GLFW_PRESS << std::endl;

    // For choosing rotationn axis "takes" only one pressing of keys 'X, Y, Z'
    if(action == GLFW_PRESS)
    {
        if(key == GLFW_KEY_X && scancode == 53)
        {
            std::cout<< "rotation axis is X\n";
            rotationAxisVector = glm::vec3(1, 0, 0);
        }
        else if(key == GLFW_KEY_Z && scancode == 52)
        {
            std::cout<< "rotation axis is Z\n";
            rotationAxisVector = glm::vec3(0, 0, 1);
        }
        else if(key == GLFW_KEY_Y && scancode == 29)
        {
            std::cout<< "rotation axis is Y\n";
            rotationAxisVector = glm::vec3(0, 1, 0);
        }
    }
    
    // For changing variable 'rotationAngle' value
    if(action == GLFW_REPEAT)
    {
        if(key == GLFW_KEY_RIGHT && scancode == 114)
        {
            std::cout<< "Righ \n\n";
            rotationAngle += 0.02f;
        }
        else if(key == GLFW_KEY_LEFT && scancode == 113)
        {
            std::cout<< "Left \n\n";
            rotationAngle -= 0.02f;
        }
    }


    glfwSetWindowSize(window, WIDTH, HEIGHT);
}

GLchar * glShaderLoad(char *path, int *length)
{
    GLchar *shaderSource = nullptr;
    int descriptor = open(path, O_RDONLY);
    if (descriptor != -1)
    {
        FILE *file = fdopen(descriptor, "rb");
        if (file)
        {
            struct stat statistics;
            if (fstat(descriptor, &statistics) != -1)
            {
                shaderSource = (GLchar *)malloc(statistics.st_size);
                fread(shaderSource, sizeof(char), statistics.st_size, file);
                *length = (statistics.st_size);
            }
            fclose(file);
        }
        // close(descriptor);
    }
    //    printf("%s\n", path);
    //    printf("%s\n", shaderSource);
    return shaderSource;
}

void getShader(GLuint *shader, char *path, int typeShader)
{
    // Build and compile our shader program
    // Shader
    GLint length = 0;
    const GLchar *shaderSource = glShaderLoad(path, &length);
    *shader = glCreateShader(typeShader);
    glShaderSource(*shader, 1, &shaderSource, &length);
    glCompileShader(*shader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(*shader, 512, nullptr, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

void getShaderProgram(GLuint *shaderProgram, int n, ...)
{
    // Link shaders
    *shaderProgram = glCreateProgram();

    va_list argPtr;
    va_start(argPtr, n);
    for (int i = 0; i < n; ++i)
    {
        GLuint *shader = va_arg(argPtr, GLuint *);
        glAttachShader(*shaderProgram, *shader);
    }
    va_end(argPtr);
    glLinkProgram(*shaderProgram);

    GLint success;
    GLchar infoLog[512];
    // Check for linking errors
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(*shaderProgram, 512, nullptr, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    va_start(argPtr, n);
    for (int i = 0; i < n; ++i)
    {
        GLuint *shader = va_arg(argPtr, GLuint *);
        glDeleteShader(*shader);
    }
    va_end(argPtr);
}