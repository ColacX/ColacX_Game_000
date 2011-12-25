uniform sampler2D myTexture;

void main(void)
{
    vec4 myV = gl_Position;
    vec2 myXY = gl_TexCoord[0].st;
    vec4 myTextureColor = texture2D(myTexture, myXY);
    gl_FragColor = myTextureColor;
}
