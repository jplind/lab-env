#shader vertex
#version 430
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(aPos, 1);
	TexCoords = aTexCoords;
};

#shader fragment
#version 430
in vec2 TexCoords;
out vec4 FragColor;
uniform sampler2D myTextureSampler;

void main()
{ 
	FragColor = texture(myTextureSampler, TexCoords);
};