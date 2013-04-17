#version 330

in vec4 vertexPosition;
out vec4 fragmentColor;

uniform sampler2D uniform_texture0;
uniform sampler2D uniform_texture1;

void main()
{
    vec4 texel0 = texture2D( uniform_texture0, vertexPosition.xy );
	vec4 texel1 = texture2D( uniform_texture1, vertexPosition.xy );
	
	if(texel1.a > 0)
		fragmentColor = texel1;
	else
		fragmentColor = texel0;	
}
