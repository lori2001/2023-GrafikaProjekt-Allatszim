#pragma once

// C++ includes
#include <memory>
#include <vector>
#include <random>
#include <future>

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

#include "Lion.h"

class Lions {
public:
	Lions() {
		m_lion_mesh = nullptr;
	}

	void Init();
	
	void Update(std::vector<glm::vec2>);

	void Render(ProgramObject* m_program, const glm::mat4& viewProj);

	std::vector< glm::vec3> getLionPositions();

	std::vector<Lion> getLions() {
		return m_lions;
	};

	std::vector<Lion> emptyLions() {
		std::vector<Lion> emptyvect;
		return emptyvect;
	}

	bool isMeshBufferInitialized();


private:
	std::future<void> loader;
	void InitData();

	// fuggveny ami ellenorzi, hogy egy adott thread lefutott-e mar 
	template<typename T>
	bool future_is_ready(std::future<T>& t) {
		return t.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

	int m_num_of_lions = 3;
	std::vector<Lion> m_lions;

	std::unique_ptr<Mesh> m_lion_mesh;
	bool m_mesh_is_buffer_initilized = false;
	Texture2D m_lion_texture;

	std::default_random_engine m_gen{4};
	std::normal_distribution<float> m_speed_gen{0.05F, 0.01F};
	std::uniform_real_distribution<float> m_z_gen{ -30.0F, 30.0F };
	std::uniform_real_distribution<float> m_x_gen{-30.0F, 30.0F};

};