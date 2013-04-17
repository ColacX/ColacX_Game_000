#version 330
#extension GL_EXT_geometry_shader4 : enable

varying in vec3 ColorX[3];

out vec4 Color;
//out vec4 gl_Position

uniform mat4 uniform_world_matrix;

void main()
{
	float s = 0.5;

    gl_Position = uniform_world_matrix * (gl_in[0].gl_Position + vec4(-s, -s, 0.0, 0.0));
	Color = vec4(1, 0, 0, 1);
    EmitVertex();

    gl_Position = uniform_world_matrix * (gl_in[0].gl_Position + vec4(+s, -s, 0.0, 0.0));
	Color = vec4(0, 1, 0, 1);
    EmitVertex();

    gl_Position = uniform_world_matrix * (gl_in[0].gl_Position + vec4(-s, +s, 0.0, 0.0));
	Color = vec4(0, 0, 1, 1);
    EmitVertex();
		
	gl_Position = uniform_world_matrix * (gl_in[0].gl_Position + vec4(+s, +s, 0.0, 0.0));
	Color = vec4(1, 1, 1, 1);
    EmitVertex();

	EndPrimitive();
}
