#pragma once

class NativeMesh
{
public:
	void* vertexBuffer;
	int vertexCount;
	void* indexBuffer;
	int indexCount;

	void* native = 0;
};
