32 bits
alpha channel
targa
white alpha is 1.0 in photoshop
black alpha is 0.0 in photoshop

remember that shaders will optimize away uniform variables if they are not used in the shader. thus trying to retreive a uniform variable id that is not used will always fail.