#pragma once

// GLM
#include <glm/glm.hpp>

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

#define SAVANA_FLOOR_SIZE 40.0F

class SavannaFloor {
public:
	SavannaFloor();
	~SavannaFloor();

	bool Init(float);
	void Render(ProgramObject*, glm::mat4);

private:
	VertexArrayObject	m_FloorVao;			// VAO
	IndexBuffer			m_FloorIndices;		// index buffer
	ArrayBuffer			m_FloorVertexBuffer;// VBO

	Texture2D			m_floorTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	glm::mat4 m_floor_world = glm::mat4(1.0f);
};
