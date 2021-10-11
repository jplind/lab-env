#shader vertex
#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	TexCoords = aTexCoords;
};

#shader fragment
#version 430
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;

void main()
{ 
	FragColor = 1 - texture(screenTexture, TexCoords);
};