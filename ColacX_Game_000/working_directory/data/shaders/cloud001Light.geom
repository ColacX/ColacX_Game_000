//creates quads from points

#version 330
#extension GL_EXT_geometry_shader4 : enable

uniform mat4 p0_lightPV;
uniform mat4 p0_modelM;

void main()
{
	float s = 1.00;

	vec4 llCorner = vec4(-s, -s, 0.0, 0.0);
	gl_Position = p0_lightPV * ((p0_modelM * llCorner) + gl_in[0].gl_Position);
    EmitVertex();

	vec4 rlCorner = vec4(+s, -s, 0.0, 0.0);
	gl_Position = p0_lightPV * ((p0_modelM * rlCorner) + gl_in[0].gl_Position);
    EmitVertex();

	vec4 luCorner = vec4(-s, +s, 0.0, 0.0);
	gl_Position = p0_lightPV * ((p0_modelM * luCorner) + gl_in[0].gl_Position);
    EmitVertex();
	
	vec4 ruCorner = vec4(+s, +s, 0.0, 0.0);
	gl_Position = p0_lightPV * ((p0_modelM * ruCorner) + gl_in[0].gl_Position);
    EmitVertex();

	EndPrimitive();
}
