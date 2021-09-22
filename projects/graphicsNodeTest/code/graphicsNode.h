#pragma once
#include "render/meshResource.h"
#include "render/textureResource.h"
#include "shaderObject.h"

struct graphicsNode
{
	vec3 transform = vec3(0, 0, 0);
	shared_ptr<meshResource> mesh;
	shared_ptr<textureResource> texture;
	shared_ptr<shaderObject> shader;

	graphicsNode(const vec3& pos, const shared_ptr<meshResource> mesh, const shared_ptr<textureResource> texture, const shared_ptr<shaderObject> shader)
		: transform(pos), mesh(mesh), texture(texture), shader(shader) {}

	void draw()
	{
		// bind mesh
		glBindVertexArray(mesh->vertexArrayObject);

		// bind shader
		shader->use();

		// bind texture
		texture->bind(0);
		glUniform1f(glGetUniformLocation(shader->program, "myTextureSampler"), 0);
		glGenerateMipmap(GL_TEXTURE_2D);

		// apply transform
		mat4 modelTransformMatrix = mat4(vec4(1, 0, 0, 0), vec4(0, 1, 0, 0), vec4(0, 0, 1, 0), vec4(transform.x, transform.y, transform.z, 1));
		int modelTransformMatrixUniformLocation = glGetUniformLocation(shader->program, "modelTransformMatrix");
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, 0, &modelTransformMatrix[0][0]);

		// render
		glDrawElements(GL_TRIANGLES, mesh->drawCount, GL_UNSIGNED_INT, NULL);

		// unbind
		glBindVertexArray(0);
		texture->unbind();
	}
};