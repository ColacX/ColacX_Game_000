#version 330

in vec3 VertexPosition;
out vec4 vertexPosition;

uniform mat4 uniform_world_matrix;
uniform int uniform_framecount;

void main()
{
	vec4 vp = vec4(VertexPosition, 1.0f);
	vp.z += sin( vp.x + vp.y + (uniform_framecount) / 60.0f * 2.0f ) * 0.1f;
	
	vertexPosition = vp;
    gl_Position = uniform_world_matrix * vp;
}
