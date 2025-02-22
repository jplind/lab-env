#shader vertex
#version 430

layout(location=0) in vec3 vertexPos;
layout(location=1) in vec2 vertexUV;
layout(location=2) in vec3 vertexNormal;
out vec2 UV;
out vec3 Normal;
out vec3 currentPos;
uniform mat4 modelTransformMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = vec4(vertexPos, 1);
	mat4 MVP = projectionMatrix * viewMatrix * modelTransformMatrix;
	gl_Position =  MVP * gl_Position;
	UV = vertexUV;
	currentPos = vec3(modelTransformMatrix * vec4(vertexPos, 1));
	Normal = vec3(modelTransformMatrix * vec4(vertexNormal, 0));
};

#shader fragment
#version 430
in vec2 UV;
in vec3 Normal;
in vec3 currentPos;
out vec4 Color;
uniform sampler2D myTextureSampler;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 lightPos2;
uniform vec3 lightColor2;
uniform float lightIntensity2;
uniform vec3 cameraPos;
vec3 calculateLightPoint(vec3 lightPos, vec3 lightColor, float lightIntensity)
{
	float ambient = 0.15f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - currentPos);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.4f;
	vec3 viewDirection = normalize(cameraPos - currentPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specularAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 20);
	float specular = specularAmount * specularLight;

	float distance = distance(lightPos, currentPos);
	float attenuation = clamp(lightIntensity / (distance * distance), 0.0f, 1.0f);
	return (ambient + diffuse + specular) * attenuation * lightColor;
}
void main()
{
	vec3 lightPoint1 = calculateLightPoint(lightPos, lightColor, lightIntensity);
	vec3 lightPoint2 = calculateLightPoint(lightPos2, lightColor2, lightIntensity2);

	Color = texture(myTextureSampler, UV) * vec4(lightPoint1 + lightPoint2, 1);
};