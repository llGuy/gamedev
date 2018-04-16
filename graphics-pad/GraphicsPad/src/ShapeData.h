#pragma once
#include <GL\glew.h>
#include "vertex.h"
#include <iostream>

struct ShapeData
{
	ShapeData()
		: vertices(0),numVertices(0),indices(0),numIndices(0) 
	{
	}

	Vertex* vertices;
	GLuint numVertices;
	GLushort* indices;
	GLuint numIndices;

	GLsizeiptr vertexBufferSize() const
	{
		return numVertices * sizeof(Vertex);
	}

	GLsizeiptr indexBufferSize(void) const
	{
		return numIndices * sizeof(GLushort);
	}

	void DeleteAll(void)
	{
		delete[] vertices;
		delete[] indices;
		numVertices = numIndices = 0;
	}
};