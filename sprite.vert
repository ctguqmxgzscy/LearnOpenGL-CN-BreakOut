//#version 330 core
//layout (location = 0) in vec3 position;
//out vec3 TexCoords;
//
//uniform mat4 projection;
//uniform mat4 view;
//
//void main()
//{
//    gl_Position =   projection * view * vec4(position, 1.0);  
//    TexCoords = position;
//}   Sky Box

#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    TexCoords = vertex.zw;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}