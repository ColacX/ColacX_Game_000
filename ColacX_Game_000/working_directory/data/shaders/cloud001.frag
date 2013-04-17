//implements:
//per fragment lighting
//ambient + diffuse + specular = phong shading
//shadowing

#version 330

uniform sampler2D p1_lightTexture;

in vec4 shader_lightTextureCoords;
in vec2 texCooord;

out vec4 shader_fragmentColor;

void main()
{
	//if(distance(texCooord.xy, vec2(0, 0)) > 1)
		//discard;

	vec4 lightTexel = texture2D(p1_lightTexture, shader_lightTextureCoords.xy);
	vec4 shadowTexel = 1 - lightTexel;


	if(lightTexel.a < shader_lightTextureCoords.z - 0.001)
		shader_fragmentColor = vec4(0.5, 0.5, 0.5, 0.5);
	else
		shader_fragmentColor = vec4(1.0, 1.0, 1.0, 0.5);
	
	//shader_fragmentColor = vec4(shader_lightTextureCoords.z, shader_lightTextureCoords.z, shader_lightTextureCoords.z, 1);
	//shader_fragmentColor = lightTexel;
	
	return;

	/*
	vec4 materialColor = vec4(1, 0, 0, 1);
	shader_fragmentColor = materialColor;
	vec3 shader_fragmentNormal = normalize((uniform_mMatrix * vec4(0, 0, 1, 0)).xyz);

	//light variables
		vec3 lightPosition = vec3(0, 0, 0);
		vec4 lightAmbient = vec4(0.2, 0.2, 0.2, 0.4);
		vec4 lightDiffuse = vec4(0.2, 0.2, 0.2, 1);
		vec4 lightSpecular = vec4(1, 1, 1, 1);

	//calculate ambient
	{
		vec4 ambientColor = vec4(
			(shader_fragmentColor.r + lightAmbient.r) * 0.5 * lightAmbient.a,
			(shader_fragmentColor.g + lightAmbient.g) * 0.5 * lightAmbient.a,
			(shader_fragmentColor.b + lightAmbient.b) * 0.5 * lightAmbient.a,
			shader_fragmentColor.a
		);

		shader_fragmentColor = ambientColor;
	}

	//calculate diffuse
	if(false){
		//vec3 viewPosition = vec3(5, 0, 0);
		vec3 viewPosition = uniform_cMatrix[0].xyz;
		vec3 lightVector = lightPosition - shader_fragmentPosition;
		//vec3 lightVector = lightPosition - gl_FragCoord.xyz;
		vec3 viewVector = viewPosition - shader_fragmentPosition;
		float lightDistance = abs(length(lightVector));

		if(gl_FrontFacing)
		{
			//calculate the vector between the light source and the eye
			vec3 halfVector = normalize(normalize(lightVector) + normalize(viewVector));

			//calculate shortest angle between the surface normal and the half vector
			//large angle goes to -> ambient light sortof kinda
			//small angle goes to -> specular light sortof kinda
			//float normal2halfAngle = abs(acos(dot(shader_fragmentNormal, halfVector))); //no need to divide by length since both are unitvectors
			float normal2halfAngle = acos(dot(shader_fragmentNormal, halfVector)); //no need to divide by length since both are unitvectors

			float PI = 3.14159265358979323846264;
			float angleIntensity = normal2halfAngle / PI;

			//vec4 diffuseColor = vec4(mix(shader_fragmentColor.rgb, lightDiffuse.rgb, angleIntensity), shader_fragmentColor.a);
			//shader_fragmentColor = diffuseColor;
			shader_fragmentColor = vec4((1-angleIntensity), 0, 0, 1);
		}
	}

	//calculate specular
		//vec4 specularColor = todo

	//calculate shadow
	if(true){
		if(gl_FrontFacing)
		{
			vec4 shadowPosition = (uniform_plMatrix * uniform_vlMatrix) * vec4(shader_fragmentPosition, 1);
			float visibility = texture(uniform_texture0, vec3(shadowPosition.xy, shadowPosition.z/shadowPosition.w));
			shader_fragmentColor = vec4(0.8, 0.8, 0.8, 1) * visibility;
		}
	}

	//calculate distance
	
	//apply all colors and output them

	float lightDistance = distance(shader_fragmentPosition, lightPosition);
	float specularIntensity = 2.0;

    shader_fragmentColor = vec4(
		materialColor.r/(lightDistance / specularIntensity),
		materialColor.g/(lightDistance / specularIntensity),
		materialColor.b/(lightDistance / specularIntensity),
		materialColor.a
	);
	*/
}
