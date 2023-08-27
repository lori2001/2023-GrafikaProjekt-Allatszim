#include "Lions.h"

void Lions::Init()
{
	loader = std::async(std::launch::async, &Lions::InitData, this);

	// texturák betöltése
	m_lion_texture.FromFile("assets/lion/lion-texture.png");
}

void Lions::Update(std::vector<glm::vec2> zebraPositions)
{
	for (auto& lion : m_lions) {
		lion.Update(zebraPositions);
	}
}

std::vector<glm::vec3> Lions::getLionPositions()
{
	std::vector<glm::vec3> positions;

	for (int i = 0; i < m_num_of_lions; i++) {
		positions.push_back(m_lions[i].getPosition());
	}

	return positions;
}

bool Lions::isMeshBufferInitialized()
{
	return m_mesh_is_buffer_initilized;
}

void Lions::InitData()
{
	m_lion_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/lion/lion-lowpoly.obj"));

	for (int i = 0; i < m_num_of_lions; i++) 
	{
		// tulajdonsgok vletlenszerstse
		m_lions.push_back({ m_speed_gen(m_gen), m_x_gen(m_gen), m_z_gen(m_gen) });
	}
}

void Lions::Render(ProgramObject* m_program, const glm::mat4& viewProj)
{
	if (future_is_ready(loader)) {

		if (!m_mesh_is_buffer_initilized) {
			m_lion_mesh->initBuffers();
			m_mesh_is_buffer_initilized = true;
		}

		m_program->SetTexture("texImage", 0, m_lion_texture);

		for (auto& lion : m_lions) {
			m_program->SetUniform("MVP", viewProj * lion.getLionWorld());
			m_program->SetUniform("world", lion.getLionWorld());
			m_program->SetUniform("worldIT", glm::inverse(glm::transpose(lion.getLionWorld())));
			m_lion_mesh->draw();
		}
	}
}

