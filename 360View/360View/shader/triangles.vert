#version 440 core
layout(location = 0) in vec3 position; 
layout(location = 1) in vec2 TexCoordIn;

out vec2 TexCoord;

uniform mat4 RotationMatrix;

void main()
{
	gl_Position = RotationMatrix * vec4(position,1.0f);
	TexCoord = vec2(TexCoordIn.x,TexCoordIn.y);
}