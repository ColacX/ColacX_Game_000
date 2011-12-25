uniform int framecount; //sent from the cpu
uniform sampler2D texture0; //sent from the cpu
uniform sampler2D texture1; //sent from the cpu
//gl_TexCoord is set in the vertex shader

void main(void)
{
	/*
    vec2 myXY = gl_TexCoord[0].st;
    float mySin = sin( mod((float)framecount/100,radians(90)) );
    myXY.t += mySin;
    vec4 myTextureColor = texture2D(myTexture, myXY);
    //vec4 myTextureColor = texture2D(myTexture, vec2(gl_TexCoord[0].s,framecount));
    vec4 myTimeColor = vec4(mySin, mySin/10, mySin/100,1);
    gl_FragColor = mix(myTextureColor, myTimeColor, clamp(0.5,0.0,1.0));
	//gl_FragColor = vec4( sin(framecount), 1, 1, 1);	
	*/
	
	/*
	float x = gl_TexCoord[0].s + (float)framecount / 100;
	int number = mod( framecount, 0x00FFFFFF );
	float r = sin( x);
	float g = sin( x/2 );
	float b = sin( x/3 );
	float a = 1;
	gl_FragColor = vec4( r, g, b, a );	
	*/
	
	vec4 texel0 = texture2D( texture0, gl_TexCoord[0].xy );
	vec4 texel1 = texture2D( texture1, vec2( gl_TexCoord[0].x + framecount/60.0, gl_TexCoord[0].y) );
	gl_FragColor = ( texel0 + texel1 ) / 2;
}
