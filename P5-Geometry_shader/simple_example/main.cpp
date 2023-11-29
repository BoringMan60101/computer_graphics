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

const unsigned int WIDTH = 800, HEIGHT = 600;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

GLchar *glShaderLoad(char *path, int *length);

void getShader(GLuint *shader, char *path, int typeShader);

void getShaderProgram(GLuint *shaderProgram, int n, ...);

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
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CG_P5_Simple_Example", nullptr, nullptr);
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
    GLuint geometryShader;
    getShader(&geometryShader, (char *)"shaders/geometry.glsl", GL_GEOMETRY_SHADER);
    GLuint fragmentShader;
    getShader(&fragmentShader, (char *)"shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram;
    getShaderProgram(&shaderProgram, 3, &vertexShader, &geometryShader, &fragmentShader);


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);




    /* 
       Изначально один треугольник (points) передается в вершинный шейдер (vertex.glsl)

       Код в vertex.glsl коориднаты массива points умножает на матрицу matModel (см. цикл отрисовки ниже)
       Фактически координаты растягивают в 1 раз их, т.е. не изменяются совсем, цвета тоже не меняются. 
    
       Далее уже эти данные попадают геометрическому шейдеру (geometry.glsl)
       goemetry.glsl из переданного треугольника формирует новый примитив 
       исходный + его отражение другого цвета (красного)
       
       И уже эти данные передаются фрагментному шейдеру (fragment.glsl).
       fragment.glsl сохраняет цвета, никак не изменяя их.
    */

    // Вершины начального примитива, они будут переданы на вход шейдерам 
    // vertex.glsl ===> geometry.glsl ===> fragment.glsl
    // Изменять код нужно только в шейдере geometry.glsl
    // ------------------------------------------------------------------
    float points[] = {
       -0.25f, 0.0f, 0.0f, 0.75f, 0.5f, 0.25f, // Vx, Vy, Vz, R, G, B
        0.25f, 0.0f, 0.0f, 0.75f, 0.5f, 0.25f, 
        0.0f,  0.5f, 0.0f, 0.75f, 0.5f, 0.25f
    };

    // Здесь, кроме VAO и VBO используют также 'IBO'
    // Про 'IBO' http://www.opengl-tutorial.org/ru/intermediate-tutorials/tutorial-9-vbo-indexing/
    GLuint indices[] = { 0, 1, 2 }; 
    unsigned int VBO, VAO, IBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO); 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Аттрибут координаты
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)0);
    // Аттрибут цвета
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid *)(3 * sizeof(float)));

    glBindVertexArray(0);

    // Цикл для отрисовки
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Цвет фона
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        //float angle = 0.5f * (float)glfwGetTime(); 
        float angle = 0.0f;

        // Diagonal matrix might be used for scaling transformations
        glm::mat4 matModel(1.0f, 0.0f, 0.0f, 0.0f,
                           0.0f, 1.0f, 0.0f, 0.0f,
                           0.0f, 0.0f, 1.0f, 0.0f,
                           0.0f, 0.0f, 0.0f, 1.0f); 

        //matModel = glm::rotate(matModel, angle, glm::vec3(0, 1, 0)); // Вращение

        //GLint ulMatModel = glGetUniformLocation(shaderProgram, "matModel"); // "Индекс" uniform переменной вершинного шейдера
        //glUniformMatrix4fv(ulMatModel, 1, GL_FALSE, &matModel[0][0]); // Задание значения uniform переменной вершинного шейдера


        // Отрисовка
        // ----------
        // Выбираем буфер GPU откуда будем брать данные (в него загружены вершины массива points[])
        glBindVertexArray(VAO); 
        // Первый способ: с использованием IBO и массива indices
        glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, nullptr);
        // Второй способ: переменная IBO и массив indiciec не используются
        //glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
        

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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