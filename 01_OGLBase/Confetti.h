#pragma once
// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <math.h>
#include <vector>
#include <array>
#include <list>
#include <tuple>

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

class Confetti {
public:
	Confetti();
	~Confetti();

	bool Init(float);
	void Render(ProgramObject*, glm::mat4, float);

	void setPosition(glm::vec3, float);
	bool getDropConfetti();
	void setDropConfetti(bool);

private:
	VertexArrayObject	m_ConfettiVao;
	IndexBuffer			m_ConfettiIndices;
	ArrayBuffer			m_ConfettiVertexBuffer;

	std::vector<Texture2D>	m_confettiTexture;

	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	int					confetti_number;
	int					confetti_texture_number;
	std::vector<int>	confetti_color;
	std::vector<int>	confetti_rotation_velocity;
	std::vector<float>	confetti_x_eps, confetti_y_eps, confetti_z_eps;
	std::vector<int>	confetti_fall_velocity;

	int					max_dist;
	float				x, y, z;
	float				scale, len, width;

	bool				dropConfetti;
};