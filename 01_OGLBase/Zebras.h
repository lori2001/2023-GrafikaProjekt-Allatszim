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

#include "Zebra.h"
#include "Lion.h"

class Zebras
{
public:

	Zebras() {
		m_zebra_mesh = nullptr;
	}

	void Init();

	void Update(std::vector<Lion>);

	void Render(ProgramObject* m_program, const glm::mat4& viewProj);

	std::vector<glm::vec2> GeneratePath();

	std::vector<glm::vec2> GetCurrentPositions();

	std::vector<Zebra> getZebras();

private:
	int m_num_of_zebras = 5;
	std::vector<Zebra> m_zebras;

	std::unique_ptr<Mesh> m_zebra_mesh;
	Texture2D m_zebra_texture;

	glm::vec4 m_start_color = { 0.0f, 0.0f, 0.0f, 1.0f };
	glm::vec4 m_end_color = { 2.0f, 0.0f, 0.0f, 1.0f };
	unsigned num_of_color_transitions = 200;
	std::vector<glm::vec4> m_color_shades;
	std::vector<unsigned> m_color_texture_indices;

	std::random_device rd;
	std::default_random_engine m_gen{4};
	std::normal_distribution<float> m_speed_gen{0.05F, 0.01F};
	std::uniform_real_distribution<float> m_z_gen{ -30.0F, 30.0F };
	std::uniform_real_distribution<float> m_x_gen{-30.0F, 30.0F};
	std::uniform_real_distribution<float> m_scale{ 1.5F, 2.5F };
	std::uniform_real_distribution<float> m_speed{0.05F, 9.0F};
	std::uniform_int_distribution<int> m_new_point{-38, 38};
};


