#shader vertex
#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	gl_Position = projectionMatrix * viewMatrix * gl_Position;
	TexCoords = aTexCoords;
};

#shader fragment
#version 430
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;
uniform vec3 cameraPos;

void main()
{ 
	FragColor = texture(screenTexture, TexCoords);
};