#include "Lion.h"

void Lion::Update(const std::vector<glm::vec2>& targets) {
	float time = SDL_GetTicks() / 1000.0f * 2 * float(M_PI) / 10;
	glm::vec2 lion_pos2{ m_lion_position.x, m_lion_position.z };

	CheckZebras(targets);
	FollowClosestZebra(targets);

	if (resting) {
		unsigned int timeR = SDL_GetTicks();
		if ((timeR - rest_time) / 1000 > 8.0F) {
			m_lion_scale = m_lion_scale * 1.5F;
			resting = false;
		}
		return;
	}

	if (time - m_last_time > m_time_diff && glm::distance(lion_pos2, target) > 1.0F) {

		glm::vec2 direction = glm::normalize(target - lion_pos2);
		m_lion_position.x += direction.x * m_speed;
		m_lion_position.z += direction.y * m_speed;

		float angle = glm::atan(direction.x / direction.y);

		if (direction.y < 0) {
			angle = angle + M_PI;
		}

		m_lion_world =
			glm::translate(glm::mat4(1.0f), m_lion_position) *
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::scale(glm::vec3(m_lion_scale, m_lion_scale, m_lion_scale));

		m_last_time = time;
	}
}


void Lion::FollowClosestZebra(std::vector<glm::vec2> zebraPositions)
{
	glm::vec2 lion_pos2{m_lion_position.x, m_lion_position.z};
	float minDist = 10000.0F;
	for (auto& position : zebraPositions)
	{
		float currDist = glm::distance(lion_pos2, position);
		if (currDist < minDist)
		{
			target = position;
			minDist = currDist;
		}
	}
}

void Lion::CheckZebras(std::vector<glm::vec2> zebraPositions)
{
	if (resting) {
		return;
	}
	glm::vec2 lion_pos2{ m_lion_position.x, m_lion_position.z };
	for (auto& position : zebraPositions)
	{
		float currDist = glm::distance(lion_pos2, position);
		if (currDist < m_lion_aura)
		{
			m_lion_aura = m_lion_aura * 1.5F;
			rest_time = SDL_GetTicks();
			resting = true;
		}
	}
}

glm::vec2 Lion::getPosition2()
{
	return { m_lion_position.x, m_lion_position.z };
}