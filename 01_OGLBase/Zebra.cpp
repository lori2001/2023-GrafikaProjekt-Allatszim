#include "Zebra.h"

glm::vec2 Zebra::getPosition()
{
	return { m_zebra_position.x, m_zebra_position.z };
}

void Zebra::checkLions(std::vector<Lion> lions) {
	if (timeOfDeath > 0) {
		return;
	}

	glm::vec2 zebra_pos2{ m_zebra_position.x, m_zebra_position.z };
	for (auto& lion : lions)
	{
		float currDist = glm::distance(zebra_pos2, lion.getPosition2());
		if (currDist < lion.getAura()) {
			timeOfDeath = int(SDL_GetTicks());
		}
	}
}

void Zebra::Update() {

	if (timeOfDeath > 0) {
		if (!eaten) {
			// gives the lions a chance to discover the zebra before it does its thing
			eaten = true;
		}
		else {
			m_time_after_death = (SDL_GetTicks() - timeOfDeath) / 1000.0F;

			if (m_time_after_death > 4.0F) {
				dead = true;
				m_zebra_world =
					glm::translate(glm::mat4(1.0f), m_zebra_position) *
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
					glm::scale(glm::vec3(m_zebra_scale, m_zebra_scale, m_zebra_scale));
			}
			else if (m_time_after_death >= 3.0F) {
				float t = (m_time_after_death - 3.0f);
				glm::vec3 initialPosition = glm::vec3(m_zebra_position.x, 5.0F, m_zebra_position.z);
				glm::vec3 sinkPosition = glm::vec3(m_zebra_position.x, -0.5F, m_zebra_position.z);
				glm::vec3 interpolatedPosition = glm::mix(initialPosition, sinkPosition, t);
				m_zebra_position = interpolatedPosition;
				m_zebra_world =
					glm::translate(glm::mat4(1.0f), m_zebra_position) *
					glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
					glm::scale(glm::vec3(m_zebra_scale, m_zebra_scale, m_zebra_scale));
			}
			else if (m_time_after_death >= m_color_transition_duration) {
				float t = (m_time_after_death - 2.0f);
				float currentAngle = glm::mix(0.0f, glm::radians(180.0f), t);
				glm::vec3 initialPosition = glm::vec3(m_zebra_position.x, 0.0F, m_zebra_position.z);
				glm::vec3 sinkPosition = glm::vec3(m_zebra_position.x, 5.0F, m_zebra_position.z);
				glm::vec3 interpolatedPosition = glm::mix(initialPosition, sinkPosition, t);
				m_zebra_position = interpolatedPosition;
				m_zebra_world =
					glm::translate(glm::mat4(1.0f), m_zebra_position) *
					glm::rotate(glm::mat4(1.0f), currentAngle, glm::vec3(0.0f, 0.0f, 1.0f)) *
					glm::scale(glm::vec3(m_zebra_scale, m_zebra_scale, m_zebra_scale));
			}
		}
		return;
	}

	float time = SDL_GetTicks() / 1000.0f * 2 * float(M_PI) / 10;
	glm::vec2 zebra_pos2{ m_zebra_position.x, m_zebra_position.z };

	if (time - m_last_time > m_time_diff && glm::distance(zebra_pos2, path[it]) > 1.0F && it < 100) {

		glm::vec2 direction = glm::normalize(path[it] - zebra_pos2);
		m_zebra_position.x += direction.x * m_speed;
		m_zebra_position.z += direction.y * m_speed;

		float angle = glm::atan(direction.x / direction.y);

		if (direction.y < 0) {
			angle = angle + M_PI;
		}

		m_zebra_world =
			glm::translate(glm::mat4(1.0f), m_zebra_position) *
			glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::scale(glm::vec3(m_zebra_scale, m_zebra_scale, m_zebra_scale));

		m_last_time = time;
	}
	else if (glm::distance(zebra_pos2, path[it]) <= 1.0F)
	{
		if (it == 99)
		{
			it = 0;
		}
		else
		{
			it++;
		}
	}
}

bool Zebra::isEaten() {
	return eaten;
}

const glm::mat4& Zebra::getZebraWorld() {
	return m_zebra_world;
}