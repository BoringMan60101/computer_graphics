#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>
#include <stdarg.h>

#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <SOIL/SOIL.h>


const int WIDTH = 960, HEIGHT = 540;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);

GLchar *glShaderLoad(char *path, int *length);

void getShader(GLuint *shader, char *path, int typeShader);

void getShaderProgram(GLuint *shaderProgram, int n, ...);

int main()
{
    // Init GLFW
    if (!glfwInit())
        exit(EXIT_FAILURE);
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFW window object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "CG_P3", NULL, NULL);
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

    GLuint vertexShader;
    getShader(&vertexShader, "shaders/vertex.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader;
    getShader(&fragmentShader, "shaders/fragment.glsl", GL_FRAGMENT_SHADER);
    GLuint shaderProgram;
    getShaderProgram(&shaderProgram, 2, &vertexShader, &fragmentShader);



    // Грибочек
    GLuint N0 = 20, mushroomVerts = 4*8 + N0*8;
    GLfloat legHeight = 0.6;
    GLfloat auxBuf[32] = {
            // Positions         // Colors           // Texture Coords
            // Аттрибуты вершин ножки
            0.25f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f,   0.3f, 0.0f,
            0.5f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   0.75f, 0.0f,
            0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.6f, -0.95f*legHeight,
            0.25f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.23f, -0.95f*legHeight
    };
    GLfloat mushroom[mushroomVerts];
    // Заполнение данных для ножки
    for(int i = 0; i < 32; i++)
        mushroom[i] = auxBuf[i];
    // Заполнение данных для шляпки
    for(int i = 0; i < N0; i++)
    {
        GLfloat t = i * M_PI / (N0 - 1.0f);
        mushroom[32 + 8*i + 0] = 0.375f + 0.25f*cos(t);
        mushroom[32 + 8*i + 1] = 0.5f + 0.35f*sin(t);
        mushroom[32 + 8*i + 2] = 0.0f;

        mushroom[32 + 8*i + 3] = 1.0f;
        mushroom[32 + 8*i + 4] = 0.0f;
        mushroom[32 + 8*i + 5] = 0.0f;

        mushroom[32 + 8*i + 6] = 0.35f + 0.25f*cos(t);
        mushroom[32 + 8*i + 7] = -legHeight - 0.3f*sin(t);
    }

    GLuint mushroomVBO, mushroomVAO;
    glGenVertexArrays(1, &mushroomVAO);
    glGenBuffers(1, &mushroomVBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(mushroomVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mushroomVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mushroom), mushroom, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Unbind VBO
    glBindVertexArray(0);



    // Загрузка текстуры для гриба
    GLuint mushroomTexture;
    glGenTextures(1, &mushroomTexture);
    glBindTexture(GL_TEXTURE_2D,
                  mushroomTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char * image = SOIL_load_image("textures/mushroom.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentally mess up our texture.



    // тестовый квадрат
    GLfloat imgW = 752.0f, imgH = 487.0f;
    GLfloat hedgehog[] = 
    {
        175.0/imgW, 390.0/imgH, 0.0,  1.0, 0.0, 0.0,  175.0/imgW, 390.0/imgH,
        430.0/imgW, 390.0/imgH, 0.0,  1.0, 0.0, 0.0,  430.0/imgW, 390.0/imgH,
        640.0/imgW, 370.0/imgH, 0.0,  1.0, 0.0, 0.0,  640.0/imgW, 370.0/imgH,
        665.0/imgW, 300.0/imgH, 0.0,  1.0, 0.0, 0.0,  665.0/imgW, 300.0/imgH,
        627.0/imgW, 170.0/imgH, 0.0,  1.0, 0.0, 0.0,  627.0/imgW, 170.0/imgH,
        520.0/imgW, 80.0/imgH, 0.0,  1.0, 0.0, 0.0,  520.0/imgW, 80.0/imgH,
        400.0/imgW, 65.0/imgH, 0.0,  1.0, 0.0, 0.0,  400.0/imgW, 65.0/imgH,
        150.0/imgW, 150.0/imgH, 0.0,  1.0, 0.0, 0.0,  150.0/imgW, 150.0/imgH,
        
        120.0/imgW, 200.0/imgH, 0.0,  1.0, 0.0, 0.0,  120.0/imgW, 200.0/imgH,
        82.0/imgW, 200.0/imgH, 0.0,  1.0, 0.0, 0.0,  82.0/imgW, 200.0/imgH,
        82.0/imgW, 230.0/imgH, 0.0,  1.0, 0.0, 0.0,  82.0/imgW, 230.0/imgH,
        120.0/imgW, 270.0/imgH, 0.0,  1.0, 0.0, 0.0,  120.0/imgW, 270.0/imgH,
        
        135.0/imgW, 360.0/imgH, 0.0,  1.0, 0.0, 0.0,  135.0/imgW, 360.0/imgH
    }; 

    GLuint hedgehogVBO, hedgehogVAO;
    glGenVertexArrays(1, &hedgehogVAO);
    glGenBuffers(1, &hedgehogVBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(hedgehogVAO);
    glBindBuffer(GL_ARRAY_BUFFER, hedgehogVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(hedgehog), hedgehog, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (GLvoid *) (6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Unbind VBO
    glBindVertexArray(0);


    // Загрузка текстуры для Ёжика
    GLuint hedgehogTexture;
    glGenTextures(1, &hedgehogTexture);
    glBindTexture(GL_TEXTURE_2D,
                  hedgehogTexture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping para3-Texture_copy/CG_P3/build$ meters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    image = SOIL_load_image("textures/hedgehog2.png", &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentally mess up our texture.
    




    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        // Render
        // Clear the colourful
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        GLint translationLocation = glGetUniformLocation(shaderProgram, "translation");
        GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
        GLint angleLocation = glGetUniformLocation(shaderProgram, "angle");

        
        // Отрисовка "фигуры" ёжика с текстурой
        glBindTexture(GL_TEXTURE_2D, hedgehogTexture); // Выбор тексутры

        glUniform3f(translationLocation, 1.0f, 0.75f, 0.0f);
        glUniform3f(scaleLocation, -2.0f, -2.0f, 1.0f); 
        glUniform1f(angleLocation, 0.0f*M_PI);

        glBindVertexArray(hedgehogVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 13); // отрисовка "фигуры"
        glBindVertexArray(0);


        // Отрисовка 1-го грибочка (верхнего)
        glBindTexture(GL_TEXTURE_2D, mushroomTexture);

        glUniform3f(translationLocation, -0.6f, 0.5f, 0.0f);
        glUniform3f(scaleLocation, 0.65f, 0.65f, 1.0f);
        glUniform1f(angleLocation, -0.37f*M_PI);

        glBindVertexArray(mushroomVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //отрисовка ножки
        glDrawArrays(GL_TRIANGLE_FAN, 3, N0+1); //отрисовка шляпки
        glBindVertexArray(0);


        // Отрисовка 2-го грибочка (справа сверху)
        glBindTexture(GL_TEXTURE_2D, mushroomTexture);

        glUniform3f(translationLocation, 0.4f, 0.0f, 0.0f);
        glUniform3f(scaleLocation, 0.45f, 0.65f, 1.0f);
        glUniform1f(angleLocation, 0.33f*M_PI);

        glBindVertexArray(mushroomVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //отрисовка ножки
        glDrawArrays(GL_TRIANGLE_FAN, 3, N0+1); //отрисовка шляпки
        glBindVertexArray(0);


        // Отрисовка 3-го грибочка (слева сбоку)
        glBindTexture(GL_TEXTURE_2D, mushroomTexture);

        glUniform3f(translationLocation, -0.9f, -0.2f, 0.0f);
        glUniform3f(scaleLocation, 0.45f, 0.65f, 1.0f);
        glUniform1f(angleLocation, -0.15f*M_PI);

        glBindVertexArray(mushroomVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //отрисовка ножки
        glDrawArrays(GL_TRIANGLE_FAN, 3, N0+1); //отрисовка шляпки
        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    // Properly de-allocate all resources once they've outlived their purpose
    // 1. VAO, VBO and IBO
    glDeleteVertexArrays(1, &mushroomVAO);
    glDeleteBuffers(1, &mushroomVBO);
    glDeleteVertexArrays(1, &hedgehogVAO);
    glDeleteBuffers(1, &hedgehogVBO);
    // 2. Textures
    glDeleteTextures(1, &hedgehogTexture);
    glDeleteTextures(1, &mushroomTexture);
    // 3. Shader Program
    glDeleteProgram(shaderProgram);
    // 4. Terminate GLFW, clearing any resources allocated by GLFW.
    glfwDestroyWindow(window);
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

//Параметры, с которыми производились расчеты, лежат в заголовочном файле
//нужно возращать ошибку!
GLchar *
glShaderLoad(char *path, int *length)
{
    GLchar *shaderSource = NULL;
    int descriptor = open(path, O_RDONLY);
    if (descriptor != -1)
    {
        FILE *file = fdopen(descriptor, "rb");
        if (file)
        {
            struct stat statistics;
            if (fstat(descriptor, &statistics) != -1)
            {
                shaderSource = (GLchar *) malloc(statistics.st_size);
                fread(shaderSource, sizeof(char), statistics.st_size, file);
                *length = (statistics.st_size);
            }
            fclose(file);
        }
        close(descriptor);
    }
//    printf("%s\n", path);
//    printf("%s\n", shaderSource);
    return shaderSource;
}


void
getShader(GLuint *shader, char *path, int typeShader)
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
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

void
getShaderProgram(GLuint *shaderProgram, int n, ...)
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
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
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