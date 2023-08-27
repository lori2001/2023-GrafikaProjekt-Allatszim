#include "Zebras.h"

void Zebras::Init()
{
	m_zebra_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/zebra/zebra.obj"));
	m_zebra_mesh->initBuffers();
	m_zebra_texture.FromFile("assets/zebra/zebra.jpg");

	for (int i = 0; i < m_num_of_zebras; i++) {
		m_zebras.push_back({ m_speed_gen(m_gen), m_x_gen(m_gen), m_z_gen(m_gen), m_scale(m_gen), GeneratePath()});
	}

	for (unsigned i = 0; i < num_of_color_transitions; i++) {
		m_color_shades.push_back(glm::mix(m_start_color, m_end_color, float(i) / (num_of_color_transitions - 1)));

		glm::vec4 color = m_color_shades[m_color_shades.size() - 1];

		std::vector<float> newTextureData(4);
		newTextureData[0] = color.r;
		newTextureData[1] = color.g;
		newTextureData[2] = color.b;
		newTextureData[3] = color.a;

		// upload the new texture data
		m_color_texture_indices.push_back({});
		glGenTextures(1, &m_color_texture_indices[i]);
		glBindTexture(GL_TEXTURE_2D, m_color_texture_indices[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1, 1, 0, GL_RGBA, GL_FLOAT, newTextureData.data());
	}
}

void Zebras::Update(std::vector<Lion> lions)
{
	m_zebras.erase(
		std::remove_if(
			m_zebras.begin(),
			m_zebras.end(),
			[](Zebra const& p) { return p.dead; }
		),
		m_zebras.end()
	);

	for (auto& zebra : m_zebras) {
		if (zebra.isEaten()) {
			// set texture parameters 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		zebra.Update();
		zebra.checkLions(lions);
	}
}

void Zebras::Render(ProgramObject* m_program, const glm::mat4& viewProj)
{
	
	for (auto& zebra : m_zebras) {
		if (zebra.isEaten()) {
			// use the new texture for rendering
			m_program->SetTexture("texImage", 1, m_color_texture_indices[zebra.colorIndex(num_of_color_transitions)]);
		}
		else {
			m_program->SetTexture("texImage", 0, m_zebra_texture);
		}
		m_program->SetUniform("MVP", viewProj * zebra.getZebraWorld());
		m_program->SetUniform("world", zebra.getZebraWorld());
		m_program->SetUniform("worldIT", glm::inverse(glm::transpose(zebra.getZebraWorld())));
		m_zebra_mesh->draw();
		// unset textures
		m_program->SetTexture("texImage", 0, 0);
		m_program->SetTexture("texImage", 1, 0);
	}
}

std::vector<Zebra> Zebras::getZebras() {
	return m_zebras;
}

std::vector<glm::vec2> Zebras::GeneratePath()
{
	std::vector<glm::vec2> path;
	std::default_random_engine gen(rd());
	for (int i = 0; i < 100; i++)
	{
		float tmpx = m_new_point(gen);
		float tmpz = m_new_point(gen);
		path.push_back(glm::vec2(tmpx, tmpz));
	}
	return path;
}

std::vector<glm::vec2> Zebras::GetCurrentPositions()
{
	std::vector<glm::vec2> pos;
	for (auto& zebra : m_zebras)
	{
		if (!zebra.isEaten()) {
			pos.push_back(zebra.getPosition());
		}
	}
	return pos;
}

