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

class Lion {

public:

	Lion(float speed, float x, float z) {
		m_lion_position = glm::vec3{ x, 0.0F, z };
		m_speed = speed;
	}

	void FollowClosestZebra(std::vector<glm::vec2>);

	void CheckZebras(std::vector<glm::vec2>);

	void Update(const std::vector<glm::vec2>& targets);

	const glm::mat4& getLionWorld() {
		return m_lion_world;
	}

	glm::vec3 getPosition() {
		return m_lion_position;
	}

	glm::vec2 getPosition2();

	float getAura() {
		return m_lion_aura;
	}

private:

	bool resting = false;
	unsigned int rest_time = 0.0F;

	float m_lion_scale = 1.0F;
	float m_lion_aura = 3.0F;

	float m_last_time = 0;
	float m_time_diff = 0.0F;

	glm::vec3 m_lion_position{ 0, 0, 0 };
	float m_speed;

	glm::mat4 m_lion_world;
	glm::vec2 target;
};