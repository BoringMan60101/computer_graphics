#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

// An example of using 'Interface buffers' to exchange data between shaders
out VertexData{ // In Geometric shader 'VertexData' has <<in>> specification
    vec3 color; 
}extra_data; 

void main(){
    gl_Position=vec4(position,1);
    extra_data.color=color;
}