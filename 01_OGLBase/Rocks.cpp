#include "Rocks.h"

void Rocks::Init()
{

	// mesh betöltése
	m_rock_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/rock/rock.obj"));
	m_rock_mesh->initBuffers();

	// texturák betöltése
	m_rockSpecularTexture.FromFile("assets/rock/rockSpecular.png");
	// m_rockNormalTexture.FromFile("assets/rock/rockNormal.png");
	// m_rockDiffuseTexture.FromFile("assets/rock/rockDiffuse.png");

	// tulajdonságok véletlenszerûsítése
	for (int i = 0; i < m_num_of_rocks; i++) {
		m_rock_mesh_worlds.push_back({
			glm::translate(glm::f32vec3(m_x_dist(gen), m_y_dist(gen), m_z_dist(gen))) *
			glm::scale(glm::vec3(m_scale(gen), m_scale(gen), m_scale(gen))) *
			glm::rotate(m_rotation(gen), glm::vec3(0, 1, 0))
			});
	}
}

void Rocks::Render(ProgramObject* m_program, const glm::mat4& viewProj)
{
	m_program->SetTexture("texImage", 0, m_rockSpecularTexture);
	for (auto& rock_world : m_rock_mesh_worlds) {
		m_program->SetUniform("MVP", viewProj * rock_world);
		m_program->SetUniform("world", rock_world);
		m_program->SetUniform("worldIT", glm::inverse(glm::transpose(rock_world)));
		m_rock_mesh->draw();
	}
}
