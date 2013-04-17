#version 330

in vec4 vertexPosition;
out vec4 out_fragColor;

uniform int uniform_framecount;
uniform sampler2D uniform_texture0;
uniform sampler2D uniform_texture1;

void main()
{
    vec4 texel0 = texture2D( uniform_texture0, vec2( vertexPosition.x + uniform_framecount/30.0f, vertexPosition.y) );
	vec4 texel1 = texture2D( uniform_texture1, vec2( vertexPosition.x + uniform_framecount/60.0f, vertexPosition.y) );
	vec4 color = ( texel0 + texel1 ) / 2;
	color.a = 0.9f;
	out_fragColor = color;
}
