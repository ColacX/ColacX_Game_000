//a trivial vertex shader, input is vertex position and output is the same vertex position

#version 330

in vec3 shader_vertexPosition;

void main()
{
    gl_Position = vec4(shader_vertexPosition, 1.0f);
}
