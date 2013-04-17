#version 330

in vec3 VertexPosition;
out vec4 vertexPosition;

uniform mat4 uniform_world_matrix;

void main()
{
	vertexPosition = vec4(VertexPosition, 1.0f);
    gl_Position = uniform_world_matrix * vertexPosition;
}
