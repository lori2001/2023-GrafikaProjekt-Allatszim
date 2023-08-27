#pragma once


#include <memory>
#include <vector>
#include <random>

#include <GL/glew.h>
#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <SDL.h>
#include <SDL_opengl.h>

#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

class Trees
{
public:
	Trees() {
		m_trees_mesh = nullptr;
	}

	void Init();

	void Render(ProgramObject* m_program, const glm::mat4& viewProj);

private:
	int m_num_of_trees = 8;

	std::unique_ptr<Mesh> m_trees_mesh;
	Texture2D			m_treesNormalTexture;

	std::vector<glm::mat4> m_trees_mesh_worlds;

	std::default_random_engine gen{ 5 };

	std::uniform_real_distribution<float> m_z_dist{ -30.0F, 30.0F };
	std::uniform_real_distribution<float> m_x_dist{ -30.0F, 30.0F };
	std::uniform_real_distribution<float> m_y_dist{ 0.0F, 0.0F };
	std::uniform_real_distribution<float> m_scale{ 0.6F, 0.7F };
	std::uniform_real_distribution<float> m_rotation{ 0.0F, 0.0F };
};

