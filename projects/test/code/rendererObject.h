#pragma once

/*
Adding a vertex buffer and index buffer.
This should return a handle that can be used with the draw function.

Setting up a framebuffer with arbitrary dimensions.

Retrieving a pointer to the framebuffer.

Retrieving the size of the framebuffer.

Setting a vertex shader by providing a lambda function as an argument.

Setting a pixel shader by providing a lambda function as an argument.

Setting a model-view-projection matrix to transform the triangles with.

Setting a texture resource to texture the mesh with.

Rasterizing a triangle.

Drawing an index buffer to the framebuffer by passing the buffer handle as an argument.
This calls the RasterizeTriangle for each triangle in the index buffer.
This should use the vertex and pixel shaders that has previously been set up.
*/

struct rendererObject
{
	
};