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

class Rocks {
public:
	Rocks() {
		m_rock_mesh = nullptr;
	}

	void Init();

	void Render(ProgramObject* m_program, const glm::mat4& viewProj);

private:
	int m_num_of_rocks = 80;

	std::unique_ptr<Mesh> m_rock_mesh;
	Texture2D			m_rockSpecularTexture;
	Texture2D			m_rockDiffuseTexture;
	Texture2D			m_rockNormalTexture;
	
	std::vector<glm::mat4> m_rock_mesh_worlds;

	std::default_random_engine gen{4};

	std::uniform_real_distribution<float> m_z_dist{ -30.0F, 30.0F };
	std::uniform_real_distribution<float> m_x_dist{-30.0F, 30.0F};
	std::uniform_real_distribution<float> m_y_dist{-0.12F, -0.01F};
	std::uniform_real_distribution<float> m_scale{ 1.5F, 2.5F };
	std::uniform_real_distribution<float> m_rotation{ 0.0F, 180.0F * (M_PI / 180.0F) };
};