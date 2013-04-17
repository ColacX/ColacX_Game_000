#version 330
#extension GL_EXT_geometry_shader4 : enable

varying in vec3 ColorX[3];
out vec3 Color;

uniform mat4 uniform_world_matrix;

void main()
{
	int i;
	
    gl_Position = uniform_world_matrix * gl_in[0].gl_Position;
    Color = ColorX[0];
    EmitVertex();

    gl_Position = uniform_world_matrix * gl_in[1].gl_Position;
    Color = ColorX[1];
    EmitVertex();

    gl_Position = uniform_world_matrix * gl_in[2].gl_Position;
    Color = ColorX[2];
    EmitVertex();
		
	EndPrimitive();

    gl_Position = uniform_world_matrix * (gl_in[0].gl_Position + vec4(5.0, 0.0, 0.0, 0.0));
    Color = ColorX[0];
    EmitVertex();

    gl_Position = uniform_world_matrix * (gl_in[1].gl_Position + vec4(5.0, 0.0, 0.0, 0.0));
    Color = ColorX[1];
    EmitVertex();

    gl_Position = uniform_world_matrix * (gl_in[2].gl_Position + vec4(5.0, 0.0, 0.0, 0.0));
    Color = ColorX[2];
    EmitVertex();
		
	EndPrimitive();
}
