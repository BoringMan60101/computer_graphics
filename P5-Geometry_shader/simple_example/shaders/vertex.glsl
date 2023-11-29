#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

// Пример использования 'Интерфейсных буферов' для обмена данными между шейдерами
// VertexData -- 'Ключевое слово' языка GLSL сам объект (массив) называется extra_data
out VertexData{ // В геометрическом шейдере 'VertexData' имеет спецификацию <<in>> и является массивом 
    vec3 color; 
}extra_data; 

void main(){
    gl_Position=vec4(position,1);
    extra_data.color=color;
}