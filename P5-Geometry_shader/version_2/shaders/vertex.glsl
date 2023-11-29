#version 330 core
layout(location=0)in vec3 position;
layout(location=1)in vec3 color;

// An example of using 'Interface buffers' to exchange data between shaders
out VertexData{ // In Geometric shader 'VertexData' has <<in>> specification
    vec3 color; 
}extra_data; 

//uniform mat4 matModel;

uniform float angle; 

void main(){

    mat4  rotationMat = mat4(
        vec4( cos(angle), -sin(angle), 0.0,  0.0 ),
        vec4( sin(angle), cos(angle),  0.0,  0.0 ),
        vec4( 0.0,        0.0,         1.0,  0.0 ),
        vec4( 0.0,        0.0,         0.0,  1.0 ) 
    ); 

    gl_Position = rotationMat * vec4(position, 1.0);
    extra_data.color = color;
}