#include "Trees.h"

void Trees::Init()
{

	m_trees_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/tree/aa.obj"));
	m_trees_mesh->initBuffers();

	m_treesNormalTexture.FromFile("assets/tree/texture.jpg");

	for (int i = 0; i < m_num_of_trees; i++) {
		m_trees_mesh_worlds.push_back({
			glm::translate(glm::f32vec3(m_x_dist(gen), m_y_dist(gen), m_z_dist(gen))) *
			glm::scale(glm::vec3(m_scale(gen), m_scale(gen), m_scale(gen))) *
			glm::rotate(m_rotation(gen), glm::vec3(0, 1, 0))
			});
	}
}

void Trees::Render(ProgramObject* m_program, const glm::mat4& viewProj)
{
	m_program->SetTexture("texImage", 0, m_treesNormalTexture);
	for (auto& tree_world : m_trees_mesh_worlds) {
		m_program->SetUniform("MVP", viewProj * tree_world);
		m_program->SetUniform("world", tree_world);
		m_program->SetUniform("worldIT", glm::inverse(glm::transpose(tree_world)));
		m_trees_mesh->draw();
	}
}
