#shader vertex
#version 430

layout(location=0) in vec3 vertexPos;
layout(location=1) in vec2 vertexUV;
out vec2 UV;
uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = vec4(vertexPos, 1);
	mat4 MVP = projectionMatrix * viewMatrix * modelTransformMatrix;
	gl_Position =  MVP * gl_Position;
	UV = vertexUV;
};

#shader fragment
#version 430
in vec2 UV;
out vec4 Color;
uniform sampler2D myTextureSampler;
void main()
{
	Color = texture(myTextureSampler, UV);
};