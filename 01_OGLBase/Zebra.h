#pragma once
// C++ includes
#include <memory>
#include <vector>
#include <random>

#include <GL/glew.h>
#include <GL/gl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// mesh
#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include "SavannaFloor.h"
#include "Lion.h"


class Zebra
{
public:

	Zebra(float speed, float x, float z, float scale, std::vector<glm::vec2> generatedPath) {
		m_zebra_position = glm::vec3{ x, 0.0F, z };
		m_speed = speed;
		m_zebra_scale = scale;
		it = 0;
		path = generatedPath;
	}

	void Update();

	bool isEaten();

	const glm::mat4& getZebraWorld();

	glm::vec2 getPosition();

	void checkLions(std::vector<Lion> lions);

	unsigned colorIndex(unsigned color_shades) {
		unsigned t = (unsigned)((m_time_after_death / m_color_transition_duration) * color_shades);
		t = t >= color_shades ? color_shades - 1 : t;
		return t;
	}

	bool dead = false;

private:
	float m_time_after_death = 0;
	float m_color_transition_duration = 2.0F;

	int timeOfDeath = -1;
	bool eaten = false;
	
	float m_zebra_scale = 1.0F;

	float m_last_time = 0;
	float m_time_diff = 0.0F;

	glm::vec3 m_zebra_position{ 0, 0, 0 };
	float m_speed;

	glm::mat4 m_zebra_world;
	int it = 0;
	std::vector<glm::vec2> path;

};

