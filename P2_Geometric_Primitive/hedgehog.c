#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include <stdio.h>

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const GLuint WIDTH = 800, HEIGHT = 800;
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);

GLchar *glShaderLoad(char *path);

int main(int argc, char *argv[]) {
  // Init GLFW
  glfwInit();
  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "PLEASE, ACCEPT THIS", NULL, NULL);
  glfwMakeContextCurrent(window);

  // Set the required callback functions
  glfwSetKeyCallback(window, key_callback);

  // Set this to true so GLEW knows to use a modern approach to retrieving
  // function pointers and extensions
  glewExperimental = GL_TRUE;
  // Initialize GLEW to setup the OpenGL Function pointers
  glewInit();

  // Define the viewport dimensions
  glViewport(0, 0, WIDTH, HEIGHT);


  // Build and compile shader programs
  // Vertex shader
  const GLchar *vertexShaderSource = glShaderLoad("shaders/vertex.glsl");
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Check for compile time errors
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  // Fragment shader
  const GLchar *fragmentShaderSource = glShaderLoad("shaders/fragment.glsl");
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Check for compile time errors
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  // Link shaders
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Check for linking errors
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  

  // Создание вершинных массивов для: тела ежика, иголки, грибочка

  // Сперва создаётся буфер (float массив), в котором значения что - то значат
  // Например, это могут быть координаты вершин или их цвета в (R, G, B)
  // Фактически, каждый такой массив определяет набор вершин и их свойства (коорд, цвет и др.)

  // Конкретный смысл этим значениям придаётся при обработке данных в шейдерных программах
  // Там каждому атрибуту присваивается свой индекс (при передаче данных GPU этот индекс также указывается)

  // Потом эти значения загружаются в память GPU, после чего они прогоняются по граф. конвейеру
  // В результате, меняя значения uniform переменных для конвейеров, можно из одного и того же массива
  // вершин получать разные фигуры (отражения, масштабирование, переносы...)

  // Для загрузки данных на GPU используется "технология" VBO (Vertex Buffer Object)
  // У GPU есть несколько буферов, один из них отображается на экран в данный момент
  // и есть другие, в которые можно просто записать данные 
  // Потом можно переключить "активный" буфер и получить другое изображение
  // Доступ к каждому такому буферу происходит через индекс, для этого используют
  // VAO (Vertex Array Object), по умолчанию всегда есть VAO с индексом '0'


  //Описание вершинного массива для "иголочки" ёжика
  GLfloat needleColor[3] = {0.5, 0.5, 0.5}; 
  GLfloat needle[18] = 
  {
      -1.0,           0.0,            0.0,             //v1(x, y, z)
      needleColor[0], needleColor[1], needleColor[2],  //v1(R, G, B)
      1.0,            0.0,            0.0,             //v2(x, y, z)
      needleColor[0], needleColor[1], needleColor[2],  //v2(x, y, z)
      0.0,            1.0,            0.0,
      needleColor[0], needleColor[1], needleColor[2],
  };

  GLuint needleVAO, needleVBO;

  glGenVertexArrays(1, &needleVAO);
  glBindVertexArray(needleVAO);

  glGenBuffers(1, &needleVBO); 
  glBindBuffer(GL_ARRAY_BUFFER, needleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(needle), needle, GL_STATIC_DRAW);

  // Выгрузка аттрибута: координаты вершин
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Выгрузка аттрибута: цвет вершин
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0); // Отключение текущего VAO (всё нужное уже записано)


  //Описание вершинного массива для глаза ёжика
  GLuint eyeN = 20;
  GLfloat eyeColor[3] = {0.0, 0.5, 1.0}; 
  GLfloat eye[eyeN*6 + 6];
  for(int i = 0; i <= eyeN; i++)
  {
    GLfloat eyeR = 0.1;
    GLfloat t = i * 2.0 * M_PI / eyeN;
    eye[6*i + 0] = eyeR * cos(t);
    eye[6*i + 1] = eyeR * sin(t);
    eye[6*i + 2] = 0.0;
    eye[6*i + 3] = eyeColor[0];
    eye[6*i + 4] = eyeColor[1];
    eye[6*i + 5] = eyeColor[2];
  }
  GLuint eyeVAO, eyeVBO;

  glGenVertexArrays(1, &eyeVAO);
  glBindVertexArray(eyeVAO);

  glGenBuffers(1, &eyeVBO); 
  glBindBuffer(GL_ARRAY_BUFFER, eyeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(eye), eye, GL_STATIC_DRAW);

  // Выгрузка аттрибута: координаты вершин
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Выгрузка аттрибута: цвет вершин
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0); // Отключение текущего VAO (всё нужное уже записано)



  // Описание вершинного массива для грибочка (ножка + шляпка)
  // В массиве хранятся координаты (локальные) вершин и их цвета
  // Каждые 6 значений массива соотв. 1 вершине (x, y, z, R, G, B)

  GLfloat mushroomCapColor[3] = {1.0, 0.4, 0.0};
  GLfloat mushroomLegColor[3] = {0.85, 0.3, 0.0};
  GLuint capVerticesNumber = 12; 
  GLuint mushroomBufferSize = 6 * ((capVerticesNumber + 1) + 4); 
  GLfloat mushroom[mushroomBufferSize];
  // для чистаемочти использовать отдельный файл с кодом + #include ???
  {
    GLfloat capWidth = 0.3, capHeight = 0.25;
    GLfloat legWidth = 0.1, legHeight = 0.3; 
    GLuint n0 = 4*6; //сколько ячеек массива mushroom отведено для "ножки"
    //Заполнение коориднат для ножки
    mushroom[0] =  -legWidth; mushroom[1] = 0.0;        mushroom[2] = 0.0;
    mushroom[6] =   legWidth; mushroom[7] = 0.0;        mushroom[8] = 0.0;
    mushroom[12] =  legWidth; mushroom[13] = legHeight; mushroom[14] = 0.0;
    mushroom[18] = -legWidth; mushroom[19] = legHeight; mushroom[20] = 0.0;
    //Заполнение цвета ножки
    for(GLuint i = 3; i < n0; i += 6)
      for(GLuint k = 0; k < 3; k++)
        mushroom[i+k] = mushroomLegColor[k];
    
    //Заполнение данных для шляпки (координаты и цвет одновременно)
    for(GLuint i = 0; i <= capVerticesNumber; i++)
    {
        GLfloat t = i * M_PI / ((GLfloat)capVerticesNumber);
        mushroom[n0 + 6*i + 0] = capWidth * cos(t);
        mushroom[n0 + 6*i + 1] = legHeight + capHeight * sin(t);
        mushroom[n0 + 6*i + 2] = 0.0;
        mushroom[n0 + 6*i + 3] = mushroomCapColor[0];
        mushroom[n0 + 6*i + 4] = mushroomCapColor[1];
        mushroom[n0 + 6*i + 5] = mushroomCapColor[2];
    }
  }
  
  // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and
  // attribute pointer(s).
  GLuint mushroomVBO, mushroomVAO;

  glGenVertexArrays(1, &mushroomVAO);
  glBindVertexArray(mushroomVAO);

  glGenBuffers(1, &mushroomVBO);
  glBindBuffer(GL_ARRAY_BUFFER, mushroomVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(mushroom), mushroom, GL_STATIC_DRAW);

  // Выгрузка аттрибута: координаты вершин
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Выгрузка аттрибута: цвет вершин
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0); // Отключение mushroomVAO (всё нужное уже записано)


  // Описание массива вершин для "тела ёжика" 
  // Ежик -- сверху это часть эллипса (n), снизу это отрезок (2), а модочка это треугольник (1)
  GLuint N = 20;
  GLuint hedgehodBufferSize = 6 * (N + 3);
  GLfloat hedgehog[hedgehodBufferSize];
  GLfloat hedgehogColor[3] = {0.0, 0.0, 0.0};

  //{
    GLfloat noseWidth = 0.2, yNose = 0.15;
    GLfloat width = 0.5, height = yNose + 0.3;
    GLuint n0 = 3*6;
    //Сперва коорд. 3-х точек: отрезок снизу и вершина для треугольника (самая правая)
    hedgehog[0] = -width;             hedgehog[1] = 0.0;    hedgehog[2] = 0.0;
    hedgehog[6] =  width;             hedgehog[7] = 0.0;    hedgehog[8] = 0.0;
    hedgehog[12] = width + noseWidth; hedgehog[13] = yNose; hedgehog[14] = 0.0;
    hedgehog[12] = width + noseWidth; hedgehog[13] = yNose; hedgehog[14] = 0.0;
    for(GLuint i = 3; i < n0; i += 6)
      for(GLuint j = 0; j < 3; j++)
        hedgehog[i+j] = hedgehogColor[j]; 

    //Остальная часть ёжика
    for(GLuint i = 1; i <= N; i++)
    {
        GLfloat alpha = 0.5;
        GLfloat t = alpha + i * ((M_PI - alpha) / N);
        hedgehog[n0 + 6*(i-1) + 0] = width * cos(t);
        hedgehog[n0 + 6*(i-1) + 1] = height * sin(t);
        hedgehog[n0 + 6*(i-1) + 2] = 0.0;
        hedgehog[n0 + 6*(i-1) + 3] = hedgehogColor[0];
        hedgehog[n0 + 6*(i-1) + 4] = hedgehogColor[1];
        hedgehog[n0 + 6*(i-1) + 5] = hedgehogColor[2];
    }
  
  //}

  //Создание VAO для записи данных ёжика на отдельный буфер GPU и VBO как средство для этого
  GLuint hedgehogVAO, hedgehogVBO;

  glGenVertexArrays(1, &hedgehogVAO);
  glBindVertexArray(hedgehogVAO);

  glGenBuffers(1, &hedgehogVBO); 
  glBindBuffer(GL_ARRAY_BUFFER, hedgehogVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(hedgehog), hedgehog, GL_STATIC_DRAW);

  // Выгрузка аттрибута: координаты вершин
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Выгрузка аттрибута: цвет вершин
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0); // Отключение текущего VAO (всё нужное уже записано)



  // Game loop
  while (!glfwWindowShouldClose(window)) {
    // Check if any events have been activiated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();
    // Render
    // Clear the colorbuffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    GLint translationLocation =
        glGetUniformLocation(shaderProgram, "translation");
    GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
    GLint angleLocation = glGetUniformLocation(shaderProgram, "angle");

    glUniform3f(translationLocation, 0.0f, 0.0f, 0.0f);
    glUniform3f(scaleLocation, 1.0f, 1.0f, 0.0f);
    glUniform1f(angleLocation, 0.0f * M_PI);
    
    //Отрисовка ёжика
    glBindVertexArray(hedgehogVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, N+3);
    glBindVertexArray(0); //Переход к буферу по умолчанию

    //Отрисовка глаза 
    glUniform3f(translationLocation,  0.35,  0.2, 0.0f);
    glUniform3f(scaleLocation,  0.3, 0.3, 0.0f);
    glUniform1f(angleLocation, 0.0 * M_PI);
    glBindVertexArray(eyeVAO); //Брать данные из буфера для грибочка
    glDrawArrays(GL_TRIANGLE_FAN, 0, eyeN+1);

    
    //Отрисовка колючек
    GLuint K = 7;
    GLfloat needleTraslations[K][2];
    needleTraslations[0][0] = -0.47; needleTraslations[0][1] = 0.25*height;
    needleTraslations[1][0] = -0.39; needleTraslations[1][1] = 0.55*height;
    needleTraslations[2][0] = -0.27; needleTraslations[2][1] = 0.8*height;
    needleTraslations[3][0] = -0.1;  needleTraslations[3][1] = 0.93*height;
    needleTraslations[4][0] =  0.05;  needleTraslations[4][1] = 0.95*height;
    needleTraslations[5][0] =  0.2;   needleTraslations[5][1] = 0.85*height;
    needleTraslations[6][0] =  0.31;  needleTraslations[6][1] = 0.73*height;

    GLfloat needleScales[K][2];
    needleScales[0][0] = 0.1; needleScales[0][1] = 0.1;
    needleScales[1][0] = 0.1; needleScales[1][1] = 0.1;
    needleScales[2][0] = 0.1; needleScales[2][1] = 0.1;
    needleScales[3][0] = 0.1; needleScales[3][1] = 0.1;
    needleScales[4][0] = 0.1; needleScales[4][1] = 0.1;
    needleScales[5][0] = 0.1; needleScales[5][1] = 0.1;
    needleScales[6][0] = 0.1; needleScales[6][1] = 0.1;


    GLfloat needleAngles[] = {0.4, 0.3, 0.17, 0.05, -0.02, -0.13, -0.18};

    glBindVertexArray(needleVAO);
    for(int k = 0; k < K; k++)
    { 
      glUniform3f(translationLocation,  needleTraslations[k][0],  needleTraslations[k][1], 0.0f);
      glUniform3f(scaleLocation,  needleScales[k][0], needleScales[k][1], 0.0f);
      glUniform1f(angleLocation, needleAngles[k] * M_PI);
      glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    }
    glBindVertexArray(0); //Переход к буферу по умолчанию

    //Отрисовка первого грибочка
    glUniform3f(translationLocation,  0.3,  0.45, 0.0f);
    glUniform3f(scaleLocation,  0.3, 0.3, 0.0f);
    glUniform1f(angleLocation, 0.3 * M_PI);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glBindVertexArray(mushroomVAO); //Брать данные из буфера для грибочка
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //Как отображать эти данные
    glDrawArrays(GL_TRIANGLE_FAN, 4, capVerticesNumber + 1); //Как отображать эти данные
    
    //Отрисовка второго грибочка
    glUniform3f(translationLocation,  -0.3,  0.45, 0.0f);
    glUniform3f(scaleLocation,  0.3, 0.3, 0.0f);
    glUniform1f(angleLocation, -0.3 * M_PI);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 3);
    glBindVertexArray(mushroomVAO); //Брать данные из буфера для грибочка
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4); //Как отображать эти данные
    glDrawArrays(GL_TRIANGLE_FAN, 4, capVerticesNumber + 1); //Как отображать эти данные

    glBindVertexArray(0); //Переход к буферу по умолчанию

    // Swap the screen buffers
    glfwSwapBuffers(window);
  }
  
  glDeleteVertexArrays(1, &eyeVAO);
  glDeleteBuffers(1, &eyeVBO);  
  glDeleteVertexArrays(1, &hedgehogVAO);
  glDeleteBuffers(1, &hedgehogVBO);  
  glDeleteVertexArrays(1, &needleVAO);
  glDeleteBuffers(1, &needleVBO); 
  glDeleteVertexArrays(1, &mushroomVAO);
  glDeleteBuffers(1, &mushroomVBO);

  // Terminate GLFW, clearing any resources allocated by GLFW.
  glfwTerminate();
  return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  glfwSetWindowSize(window, 600, 600);
}

// Параметры, с которыми производились расчеты, лежат в заголовочном файле
// нужно возвращать ошибку!
GLchar *glShaderLoad(char *path) {
  GLchar *shaderSource = NULL;

  int descriptor = open(path, O_RDONLY);

  if (descriptor != -1) {
    FILE *file = fdopen(descriptor, "rb");

    if (file) {
      struct stat statistics;

      if (fstat(descriptor, &statistics) != -1) {
        shaderSource = (GLchar *)malloc(statistics.st_size);
        fread(shaderSource, sizeof(char), statistics.st_size, file);
      }
      fclose(file);
    }
    close(descriptor);
  }
  printf("%s\n", path);
  printf("%s\n", shaderSource);
  return shaderSource;
}