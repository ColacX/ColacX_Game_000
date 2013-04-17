shaders are a bit tricky when you want to do coolstuff with them

some fast stuff you can do with shaders:
	multi-texturing, compute a interpolation between multiple textures
	animated-texture, make for example simple water effects with moving textures
	tesselation, make thoose jagged triangle models look super smooth
	
heres some notes:

the shaders are run on the gpu in the following order:
0) vertex shaders manipulate the position of each vertex
1) geometry shaders can create new vertexes and thus make more geometry
2) fragment shaders colors each geometry according the texture coordinate map or the color that code says

syntax has changed considerably much for GLSL in just a few years. to check GLSL version run code:
	const GLubyte* pVersion = glGetString(GL_VERSION);
	printf("Running GL Version: %s\n", pVersion);

	const GLubyte* pShaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	printf("Running GLSL Version: %s\n", pShaderVersion);
	
	god i fucking hate gl version 330, its fucking hard to get.

uniform variables are variables that can be sent from the CPU to the Shader on the GPU

some variables must be specified in one or more shaders type to be used in a later stage with another shader type.
	example: texture positions must be set in a vertex shader so that the fragment shader knows which color to use.

every vertex that comes into the pipe must be mapped with a texcoord position or the texture mapping will be fucked up.
	same goes for the geometry shader

http://www.opengl.org/registry/specs/EXT/geometry_shader4.txt
http://www.opengl.org/wiki/Geometry_Shader_Examples