#include <GL/glew.h> // расширение функций OpenGL (просто GL)
#include <GLFW/glfw3.h> // для работы с окнами 
#include <math.h> 
#include <stdio.h>

// Unix библиотеки (нужны для автоматической компиляции шейдеров)
#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


const GLuint WIDTH = 800, HEIGHT = 800; // константы для задания размеров окна

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode);
GLchar *glShaderLoad(char *path);


int main(int argc, char *argv[]) {
  // Начальные настройки GLFW (чтобы создать окно)
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

  // Создание окна
  GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Window-Title", NULL, NULL);
  glfwMakeContextCurrent(window);

  glfwSetKeyCallback(window, key_callback);

  // Разрешить использовать GL современные функции
  glewExperimental = GL_TRUE;

  // Запуск библиотеки GLEW 
  glewInit();

  // Установка области обзора
  glViewport(0, 0, WIDTH, HEIGHT);



  // (начало) ========<<< Сборка и компиляция шейдеров 
  // Вершинный шейдер (преобразует координаты вершин: переносты, мастштабирование, повороты)
  const GLchar *vertexShaderSource = glShaderLoad("shaders/vertex.glsl");
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  // Проверка ошибок компиляции
  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  // Фрагментный шейдер (раскрашивает пиксели)
  const GLchar *fragmentShaderSource = glShaderLoad("shaders/fragment.glsl");
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  // Проверка ошибок компиляции
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  // Соеднинение (линковка) шейдеров
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  // Проверка ошибок соеднинения (линковки) шейдеров
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // Сборка и компиляция шейдеров >>>======= (конец)


  // (начало) ========<<< про вершинные массивы (см. rectange[], rectangeVAO, rectangleVBO далее)

  // Буфер - область памяти, которая поддерживает операции ввода/вывода данных

  // 
  // Сперва создаётся буфер (float массив - rectange[]), в котором значения что - то значат:
  // Например, это могут быть координаты вершин или их цвета в (R, G, B)
  // Фактически, каждый такой массив определяет набор вершин и их свойства (коорд, цвет и др.)

  // Конкретный смысл этим значениям придаётся при загрузке данных в GPU и их обработке в шейдерах
  // Каждому атрибуту присваивается свой индекс (здесь '0' для координат, '1' для цвета)

  // Потом эти значения загружаются в память GPU, после чего они прогоняются по графическому конвейеру
  // В результате, меняя значения uniform переменных для конвейеров, можно из одного и того же массива вершин 
  // получать разные фигуры (за счёт отражений, масштабирования, переносов...)

  // Для загрузки данных на GPU используется "технология" VBO (Vertex Buffer Object)
  // У GPU может быть буферов (у каждого свой индекс), один из них отображается на экран в данный момент
  // и есть другие, в которые можно просто записать данные 
  // Потом можно переключить "активный" буфер и получить другое изображение
  // Доступ к каждому такому буферу происходит через индекс, для этого используют
  // VAO (Vertex Array Object), по умолчанию всегда есть VAO с индексом '0'

  // (начало) про вершинные массивы (см. rectange[], rectangeVAO, rectangleVBO далее) >>>======= (конец)



  // (начало) ========<<< Подготовка данных 

  // Описание прямоуголника в локальных координатах 
  // Прямоугольник - набор вершин с их аттрибутами (координата, цвет)
  // На каждую вершину по 6 значений: (X, Y, Z, R, G, B)  
  const GLuint N = 4;
  GLfloat rectangle[] = 
  {
      -0.9, 0.0, 0.0, // v0(x, y, z) - координаты вершины "0"
      0.3, 0.5, 0.7,  // v0(R, G, B) - цвет вершины "0"
      0.0, 0.0, 0.0,  // v1(x, y, z)
      0.3, 0.5, 0.7,  // v1(R, G, B)
      0.0, 0.4, 0.0,
      0.3, 0.5, 0.7,
      -0.9, 0.4, 0.0,
      0.3, 0.5, 0.7,
  };
  // GLfloat - (Gl float) - вещественное число
  // GLuint - (Gl unsigned int) - целое неотрицательное число
  // Подготовка данных >>>======= (конец)



  // (начало) ========<<< Выгрузка данных (с оперативной памяти) на GPU

  GLuint rectangleVAO; // Индекс буфера на GPU 
  GLuint rectangleVBO; // Объект через который происходит запись данных из массива rectangle[] в буфер на GPU

  glGenVertexArrays(1, &rectangleVAO); // Получение номера буфера для записи данных про прямоугольник
  glBindVertexArray(rectangleVAO); // Активация этого буфера (все данные пойдут в него)

  glGenBuffers(1, &rectangleVBO); // Получение объекта VBO, через него происходит запись данных
  glBindBuffer(GL_ARRAY_BUFFER, rectangleVBO); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW); // "Разметка данных" 

  // Загрузка "координат вершин" из массива rectangle[] в буфер на GPU (его номер в переменной rectangleVAO)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
  glEnableVertexAttribArray(0);

  // Загрузка "цвета вершин" из массива rectangle[] в буфер на GPU (его номер в переменной rectangleVAO)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
      (GLvoid *)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0); // Переключение к стандартному буферу GPU 
  //(всё нужное про прямоугольник уже записали в буфер, номер которого в переменной rectangleVAO)

  // Выгрузка данных (с оперативной памяти) на GPU >>>======= (конец)



  // (начало) ========<<< цикл отрисовки данных 
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents(); // Проверить были ли нажатия кнопок 
    
    // Отрисовать фон заданного цвета
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Запуск шейдеров 
    glUseProgram(shaderProgram);
    GLint translationLocation =
        glGetUniformLocation(shaderProgram, "translation");
    GLint scaleLocation = glGetUniformLocation(shaderProgram, "scale");
    GLint angleLocation = glGetUniformLocation(shaderProgram, "angle");


    // (1) Отрисовка прямоугольника (в том виде, в каком заданы его координаты в массиве rectangle[])
    // Входные параметры для шейдеров (здесь значения вообще не изменять данные в массиве rectangle[])
    glUniform3f(translationLocation, 0.0f, 0.0f, 0.0f); // Как сместить объект 
    glUniform3f(scaleLocation, 1.0f, 1.0f, 0.0f); // Как масштабировать исходные координаты (из rectangle[])
    glUniform1f(angleLocation, 0.0f * M_PI); // На какой угол в радианах вращать
    
    glBindVertexArray(rectangleVAO); // Выбор буфера GPU откуда брать данные для передачи в шейдеры и последующей отрисовк
    glDrawArrays(GL_LINE_LOOP, 0, N); // Как "воспринимать точки" (просто соединять линией, замкнутой) 
    glBindVertexArray(0); // Переход к буферу GPU по умолчанию


    // (2) Отрисовка прямоугольника, когда входные параметры шейдеров изменены
    // Входные параметры для шейдеров (здесь значения вообще не изменять данные в массиве rectangle[])
    glUniform3f(translationLocation, 0.3f, 0.5f, 0.0f); // смещенного на 0.3 по Ox, и на 0.5 по Oy
    glUniform3f(scaleLocation, 1.0f, 1.0f, 0.0f); // Масштаб без изменения
    glUniform1f(angleLocation, 0.0f * M_PI); // Вращения нет
    
    glBindVertexArray(rectangleVAO); // Выбор буфера GPU откуда брать данные для передачи в шейдеры и последующей отрисовк
    glDrawArrays(GL_TRIANGLE_FAN, 0, N); // Как "воспринимать точки" (закрасить область) 
    glBindVertexArray(0); // Переход к буферу GPU по умолчанию


    // (3) Отрисовка прямоугольника, когда входные параметры шейдеров изменены
    // Входные параметры для шейдеров (здесь значения вообще не изменять данные в массиве rectangle[])
    glUniform3f(translationLocation, 0.3f, -0.5f, 0.0f); // смещенного на 0.3 по Ox, и на -0.5 по Oy
    glUniform3f(scaleLocation, 0.5f, 0.5f, 0.0f); // Масштаб "сжатие" в 2 раза
    glUniform1f(angleLocation, 0.25f * M_PI); // Вращения на угол 45 градусов 
    
    glBindVertexArray(rectangleVAO); // Выбор буфера GPU откуда брать данные для передачи в шейдеры и последующей отрисовк
    glDrawArrays(GL_TRIANGLE_FAN, 0, N); // Как "воспринимать точки" (закрасить область) 
    glBindVertexArray(0); // Переход к буферу GPU по умолчанию


    glfwSwapBuffers(window);
  }
  // цикл отрисовки данных >>>======= (конец)
  

  // Правильная очистка памяти 
  glDeleteVertexArrays(1, &rectangleVAO);
  glDeleteBuffers(1, &rectangleVBO);

  // Правильное закрытие окна
  glfwTerminate();
  return 0;
}



// Функция вызывается при любых нажатиях на кнопки окна GLFW
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
  glfwSetWindowSize(window, 600, 600);
}


// Параметры, с которыми производились расчеты, лежат в заголовочном файле
// нужно возвращать ошибку! (?)
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
  //GLuint - (Gl unsigned int) - целое неотрицательное число
  printf("%s\n", path);
  printf("%s\n", shaderSource);
  return shaderSource;
}