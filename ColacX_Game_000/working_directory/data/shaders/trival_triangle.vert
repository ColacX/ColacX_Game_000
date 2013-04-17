#version 330

in vec3 VertexPosition;
in vec3 VertexColor;
varying out vec3 ColorX;

uniform int uniform_framecount;

void main()
{
    ColorX = VertexColor;
    gl_Position = vec4(VertexPosition, 1.0f);
}
