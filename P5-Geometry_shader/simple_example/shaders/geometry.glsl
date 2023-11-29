#version 440 core
layout(triangles) in; // Тип входного примитива соответсвующий (GL_TRIANGLE)
layout(triangle_strip, max_vertices = 256) out; // Тип выходного примитива и ограничение на число выводимых вершин

// Пример использования 'Интерфейсных буферов' для обмена данными между шейдерами
// VertexData -- 'Ключевое слово' языка GLSL сам объект (массив) называется extra_data
in VertexData // В вершинном шейдере 'VertexData' имеет спецификацию <<out>> 
{
    vec3 color;
}
extra_data[]; // Здесь уже массив, т.к. геометрический шейдер может из одного примитива создать несколько

out vec3 color_f; // Отправляется во фрагментный шейдер



// К переданному треугольники формирует ещё 3 дополнительных красного, синего и зеленого цветов
void main()
{
    // gl_Postion - можно представлять как наш "курсор"

    // Изначально он пуст, после первого присваивания он перемещается в присовенную точку
    // По умолчанию у неё цвет первой точки из входных данных, но можно его изменить 
    // (присвоить новый трехмерный вектор цвета переменной color_f)
    // Наконец, эту точку нужно запомнить командой EmitVertex()

    // Выходной примитив обязан состоят из числа точек кратного 3-м (это наборы треугольников)
    // Когда эти операции сделаны для 3-х вершин нужно зафиксировать примтив командой EndPrimitive()

    
    // Построение основного треугольника, переданного как входные данные
    gl_Position = gl_in[0].gl_Position; // Доступ к 1-й вершине
    color_f = extra_data[0].color;
    EmitVertex();
    
    gl_Position = gl_in[1].gl_Position;
    color_f = extra_data[1].color;
    EmitVertex();

    gl_Position = gl_in[2].gl_Position;
    color_f = extra_data[2].color;
    EmitVertex();

    EndPrimitive();
    

    // Построение отзеркалленого треугольника (каждой из трех вершин задан свой цвет)
    // (4-я компонента любой вершнины в этом шейдере всегда дложна быть равна 1.0)
    vec4 newPoint = -1.0 * gl_in[0].gl_Position; 
    newPoint[3] = 1.0;
    gl_Position = newPoint;
    color_f = vec3(1.0, 0.0, 0.0);
    EmitVertex();
    
    newPoint = -1.0 * gl_in[1].gl_Position; 
    newPoint[3] = 1.0;
    gl_Position = newPoint;
    color_f = vec3(0.0, 1.0, 0.0);
    EmitVertex();

    newPoint = -1.0 * gl_in[2].gl_Position; 
    newPoint[3] = 1.0;
    gl_Position = newPoint;
    color_f = vec3(0.0, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}