#version 330

out vec4 shader_fragmentColor;

void main()
{
	float alpha = 0.2;
	float depth = (gl_FragCoord.z / gl_FragCoord.w) * 0.005;
	shader_fragmentColor = vec4(alpha, alpha, alpha, depth);
}
