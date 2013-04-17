#version 330
#extension GL_EXT_geometry_shader4 : enable

uniform mat4 p1_modelM;
uniform mat4 p1_cameraPV;
uniform mat4 p1_lightPV;

out vec4 shader_lightTextureCoords;
out vec2 texCooord;

vec4 textureNormalizedCoords(vec4 screenProjected)
{
	vec3 normalizedDeviceCoords = (screenProjected.xyz / screenProjected.w);
	return vec4( (normalizedDeviceCoords.xy + 1.0) / 2.0, screenProjected.z * 0.005, 1/screenProjected.w);
}

void main()
{
	float s = 1.00;

	vec4 llCorner = vec4(-s, -s, 0.0, 0.0);
	vec4 llWorldPosition = ((p1_modelM * llCorner) + gl_in[0].gl_Position);
	gl_Position = p1_cameraPV * llWorldPosition;
	shader_lightTextureCoords = textureNormalizedCoords(p1_lightPV * llWorldPosition);
	texCooord = vec2(-1, -1);
    EmitVertex();

	vec4 rlCorner = vec4(+s, -s, 0.0, 0.0);
	vec4 rlWorldPosition = ((p1_modelM * rlCorner) + gl_in[0].gl_Position);
	gl_Position = p1_cameraPV * rlWorldPosition;
	shader_lightTextureCoords = textureNormalizedCoords(p1_lightPV * rlWorldPosition);
	texCooord = vec2(+1, -1);
    EmitVertex();

	vec4 luCorner = vec4(-s, +s, 0.0, 0.0);
	vec4 luWorldPosition = ((p1_modelM * luCorner) + gl_in[0].gl_Position);
	gl_Position = p1_cameraPV * luWorldPosition;
	shader_lightTextureCoords = textureNormalizedCoords(p1_lightPV * luWorldPosition);
	texCooord = vec2(-1, +1);
    EmitVertex();
	
	vec4 ruCorner = vec4(+s, +s, 0.0, 0.0);
	vec4 ruWorldPosition = ((p1_modelM * ruCorner) + gl_in[0].gl_Position);
	gl_Position = p1_cameraPV * ruWorldPosition;
	shader_lightTextureCoords = textureNormalizedCoords(p1_lightPV * ruWorldPosition);
	texCooord = vec2(+1, +1);
    EmitVertex();

	EndPrimitive();
}
