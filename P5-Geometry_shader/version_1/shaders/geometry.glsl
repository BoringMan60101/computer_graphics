#version 440 core
layout(lines) in; // Input primitive type (GL_LINES in <<glDraw>> functions)
layout(line_strip, max_vertices = 256) out; // Output primitive type and vertex limitation

// An example of using 'Interface buffers' to exchange data between shaders
// Here color attribute is sent by 'Interface buffer' called 'extra_data'
// This is an array of primitive "VertexData" with only one field 'color'
in VertexData // In Vertex shader 'VertexData' has <<out>> specification
{
    vec3 color;
}
extra_data[]; // This is an array, because geometric shader can produce additional verticies

out vec3 color_f; // Sent to fragment shader


int myPow(int base, int exponent);

float myMagVec3(vec4 v);

// Only 2 points of given line is used by this geometric shader to produce fractal <<Kolbasa Minkovskogo>>
// https://server.179.ru/wiki/?page=Informatika/Arxiv/20_11/7B/20_11_04_04/Krivaja_Minkovskogo
void main()
{
    // Rotation matrix along Z-axis
    // Used to rotate a vector on 90 degrees anti-clock wise in XY-plane
    const mat4 rot90 = mat4( 0.0, -1.0, 0.0, 0.0,
                             1.0,  0.0, 0.0, 0.0,
                             0.0,  0.0, 1.0, 0.0,
                             0.0,  0.0, 0.0, 1.0 ); 

    // There is a small problem with sizes of array 'segments'
    // For this time sizes of 'segments' are set to max possible values 
    // Num of segments on the last recursion step is 8^(n-1)
    const int recSteps = 4;
    vec4 segments[recSteps][8*8*8][2]; // Each segment has start point and end point
    //[recSteps][maxElenetsOnLastRecStep][2]


    // Preparing for the first recursion step
    segments[0][0][0] = gl_in[0].gl_Position;
    segments[0][0][1] = gl_in[1].gl_Position;
    float len = myMagVec3(segments[0][0][1] - segments[0][0][0]);
    int curNumOfSegments = 1;

    // Calculating additional points of Kolbasa Minkovskogo
    for(int step = 0; step < recSteps; step++)
    {
        len /= 4.0; 
        for(int s = 0; s < curNumOfSegments; s++) // Points at stage when 'step' actually is 'step+1'
        {
            // Auxilary directional vertors (on each iteration should be updated)
            //-----------------------------------------------------------------
            // First of all It is necessary to determine two directions:
            // 1) parallel to given segment
            // 2) orthogonal to given semgent
            vec4 dirPar = segments[step][s][1] - segments[step][s][0];
            vec4 dirOrtho = dirPar * rot90;
            dirPar[3] = dirOrtho[3] = 0.0; // 4th component is not being used
            for(int i = 0; i < 3; i++) // Normalize directional vectors
            {
                dirPar[i] /= myMagVec3(dirPar);
                dirOrtho[i] /= myMagVec3(dirOrtho);
            }
            dirPar = len * dirPar;
            dirOrtho = len * dirOrtho;

            // Calculating points for new 8 segments constructed by current segment
            vec4 P[9];
            P[0] = segments[step][s][0];
            P[1] = P[0] + dirPar;
            P[2] = P[1] + dirOrtho;
            P[3] = P[2] + dirPar;
            P[4] = P[3] - dirOrtho;
            P[5] = P[4] - dirOrtho;
            P[6] = P[5] + dirPar;
            P[7] = P[6] + dirOrtho;
            P[8] = P[7] + dirPar;

            // Adding new 8 segments for next recursion step
            if(step + 1 < recSteps)
                for(int newSegm = 0; newSegm < 8; newSegm++)
                {
                    segments[step + 1][s*8 + newSegm][0] = P[newSegm];
                    segments[step + 1][s*8 + newSegm][1] = P[newSegm + 1];
                }
        }
        curNumOfSegments *= 8;
    }


    // Making new primitive LINES_STRIP with calculated points (Kolbasa is bult here)
    // Color of the whole Kolbasa is set by the color of first given point 
    color_f = extra_data[0].color; 
    const int maxSegments = myPow(8, recSteps - 1);
    for(int segm = 0; segm < maxSegments; segm++)
    {
        // For optimisation, it is recognized 
        // that end point of segment 'N-1' is start point for sement 'N'
        gl_Position = segments[recSteps - 1][segm][0];
        EmitVertex();
    }
    gl_Position = segments[recSteps - 1][maxSegments - 1][1];
    EmitVertex();
    EndPrimitive();

}

int myPow(int base, int exponent)
{
    int value = 1;
    for(int i = 1; i <= exponent; i++)
        value *= base;
    return value; 
}

float myMagVec3(vec4 v) // Only x,y,z components of given vector
{
    return sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}