#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

out vec3 color_f; 

uniform mat4 modelMatrix;

void main() {
    gl_Position =  vec4(position,1.0) * transpose(modelMatrix);
    //gl_Position = vec4(position, 1.0);
    color_f = color;
}