#include "Confetti.h"

#include <math.h>
#include <vector>

Confetti::Confetti()
{
	dropConfetti = true;

	confetti_number = 500;
	confetti_texture_number = 6;

	max_dist = 30;
	len = 3.0f;
	width = 1.0f;

	x = y = z = 0.0f;
}

Confetti::~Confetti()
{
}

bool Confetti::Init(float bg_height)
{
	// Bear Grylls ketszer akkora mint eredeti merete mikor megjelennek a konfettik
	//scale = (2 * bg_height) / 100 * 1.5; 
	scale = bg_height / 100 * 1.5;

	std::srand(std::time(nullptr));

	// texture
	m_confettiTexture.resize(confetti_texture_number);
	m_confettiTexture[0].FromFile("assets/confetti/pink.jpg");
	m_confettiTexture[1].FromFile("assets/confetti/green.jpg");
	m_confettiTexture[2].FromFile("assets/confetti/yellow.jpg");
	m_confettiTexture[3].FromFile("assets/confetti/blue.jpg");
	m_confettiTexture[4].FromFile("assets/confetti/orange.jpg");
	m_confettiTexture[5].FromFile("assets/confetti/purple.jpg");

	// generate random color, rotation velocity and position
	for (int i = 0; i < confetti_number; i++) {
		confetti_color.push_back(rand() % confetti_texture_number);
		confetti_rotation_velocity.push_back(rand() % 10 + 10 * ((rand() % 2 == 1) ? 1 : -1)); // with direction
		confetti_x_eps.push_back((float)(rand() % (max_dist * 2) - max_dist) / 10.0);
		confetti_y_eps.push_back((float)(rand() % (max_dist * 2) - max_dist) / 10.0);
		confetti_z_eps.push_back((float)(rand() % (max_dist * 2) - max_dist) / 10.0);
		confetti_fall_velocity.push_back(rand() % 20 + 10);
	}

	//struct Vertex{ glm::vec3 position; glm::vec3 normals; glm::vec2 texture; };
	std::vector<Vertex> vertices;
	//top									 
	vertices.push_back({ glm::vec3(-len, 0, +width), glm::vec3(0, 1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+len, 0, +width), glm::vec3(0, 1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-len, 0, -width), glm::vec3(0, 1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+len, 0, -width), glm::vec3(0, 1, 0), glm::vec2(1, 1) });
	//bottom							 
	vertices.push_back({ glm::vec3(-len, 0, -width), glm::vec3(0, -1, 0), glm::vec2(0, 0) });
	vertices.push_back({ glm::vec3(+len, 0, -width), glm::vec3(0, -1, 0), glm::vec2(1, 0) });
	vertices.push_back({ glm::vec3(-len, 0, +width), glm::vec3(0, -1, 0), glm::vec2(0, 1) });
	vertices.push_back({ glm::vec3(+len, 0, +width), glm::vec3(0, -1, 0), glm::vec2(1, 1) });


	std::vector<int> indices(2 * 6);
	int index = 0;
	for (int i = 0; i < 2 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	m_ConfettiVertexBuffer.BufferData(vertices);

	m_ConfettiIndices.BufferData(indices);

	m_ConfettiVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(Vertex)>, m_ConfettiVertexBuffer },
			{ CreateAttribute<1, glm::vec3, (sizeof(glm::vec3)), sizeof(Vertex)>, m_ConfettiVertexBuffer },
			{ CreateAttribute<2, glm::vec2, (2 * sizeof(glm::vec3)), sizeof(Vertex)>, m_ConfettiVertexBuffer },
		},
		m_ConfettiIndices
		);

	return true;
}

void Confetti::Render(ProgramObject* m_program, glm::mat4 viewProj, float time_dif)
{
	m_ConfettiVao.Bind();

	float time = SDL_GetTicks() / 1000.0f * 2 * float(M_PI) / 10;

	for (int i = 0; i < confetti_number; ++i)
	{	
		int color = confetti_color[i];
		int rot_veloc = confetti_rotation_velocity[i];
		float temp_x = confetti_x_eps[i] + x;
		float temp_y = confetti_y_eps[i] + y;
		float temp_z = confetti_z_eps[i] + z;

		glm::mat4 confettiWorld;
		confettiWorld =
			glm::translate(glm::vec3(temp_x, temp_y, temp_z)) *
			glm::scale(glm::vec3(scale, scale, scale)) *
			glm::rotate(time * rot_veloc, glm::vec3(1, 1, 0)) *
			glm::rotate((float)M_PI / 2, glm::vec3(0, 0, 1));

		m_program->SetTexture("texImage", 0, m_confettiTexture[color]);
		m_program->SetUniform("MVP", viewProj * confettiWorld);
		m_program->SetUniform("world", confettiWorld);
		m_program->SetUniform("worldIT", glm::inverse(glm::transpose(confettiWorld)));
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);

		confetti_y_eps[i] -= confetti_fall_velocity[i] * time_dif;
	}
}

void Confetti::setPosition(glm::vec3 bg_position, float height)
{
	x = bg_position.x;
	y = bg_position.y + height*3;
	z = bg_position.z;
}

bool Confetti::getDropConfetti()
{
	return dropConfetti;
}

void Confetti::setDropConfetti(bool drop)
{
	dropConfetti = drop;
}
