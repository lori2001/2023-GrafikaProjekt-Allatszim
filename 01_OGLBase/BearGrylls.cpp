#include "BearGrylls.h"

BearGrylls::BearGrylls()
{
	isBearGryllsHere = false;
	isOnFloor = false;
	isMoving = false;
	isFalling = false;
	reachedFinalSize = false;

	// Kezdeti pozíció beállítása
	x = 0.0;
	y = 10.0;
	z = 0.0;

	angle = 0.0f;

	move_velocity = 5;
	fall_velocity = 20;

	lion_distance_2 = 5;
}

BearGrylls::~BearGrylls()
{
}

bool BearGrylls::Init(float floor_size)
{
	m_BearGryllsMesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/bearGrylls/BearGrylls.obj"));
	m_BearGryllsMesh->initBuffers();

	m_bearGryllsTexture.FromFile("assets/bearGrylls/BearGrylls.png");

	scale = floor_size / 15;
	final_size = scale * 2;

	floor_limit = floor_size - 1;

	floor_diagonal_2 = (floor_size * 2) * (floor_size * 2) * 2;	// negyzet atmerojenek negyzete

	return true;
}

void BearGrylls::Render(ProgramObject* m_program, glm::mat4 viewProj, float ldx, glm::vec3 ambient)
{
	bearGryllsWorld = glm::translate(glm::vec3(x, y, z)) * glm::scale(glm::vec3(scale, scale, scale)) *  glm::rotate(angle, glm::vec3(0, 1, 0));
	m_program->SetUniform("La", ambient);
	m_program->SetUniform("Ld", glm::vec3(ldx, 0, 0));
	m_program->SetTexture("texImage", 0, m_bearGryllsTexture);
	m_program->SetUniform("MVP", viewProj * bearGryllsWorld);
	m_program->SetUniform("world", bearGryllsWorld);
	m_program->SetUniform("worldIT", glm::inverse(glm::transpose(bearGryllsWorld)));
	m_BearGryllsMesh->draw();

	if (SDL_GetTicks() / 1000.0f - stop_time > 0.1) {
		isMoving = false;
	}

}

bool BearGrylls::rotateTowardsNearestLion(std::vector<glm::vec3> lionPositions)
{
	bool isTooClose = false;
	int i = 0;
	float minDist2 = floor_diagonal_2;
	glm::vec3 minPos;

	while (!isTooClose && i < lionPositions.size()) {
		float dist2 = (x - lionPositions[i].x) * (x - lionPositions[i].x) + (z - lionPositions[i].z) * (z - lionPositions[i].z);

		if (dist2 < minDist2) {
			minDist2 = dist2;
			minPos = lionPositions[i];
		}

		if (minDist2 <= lion_distance_2) {
			isTooClose = true;
		}

		i++;
	}

	if (!isTooClose && !isMoving) {
		glm::vec3 bg_pos = { x, y, z };
		glm::vec3 direction = glm::normalize(minPos - bg_pos);

		float angle_temp = std::atan2(direction.x, direction.z);
		angle = angle_temp >= 0 ? angle_temp : angle_temp + 2 * M_PI;
	}

	return isTooClose;
}

void BearGrylls::moveUp(float t)
{
	angle = 0;
	if (z <= floor_limit)
		z += move_velocity * t;
}

void BearGrylls::moveDown(float t)
{
	angle = M_PI;
	if (z >= -floor_limit)
		z -= move_velocity * t;
}

void BearGrylls::moveLeft(float t)
{
	angle = M_PI / 2;
	if (x <= floor_limit)
		x += move_velocity * t;
}

void BearGrylls::moveRight(float t)
{
	angle = -M_PI / 2;
	if (x >= -floor_limit)
		x -= move_velocity * t;
}

void BearGrylls::drop(float t)
{
	y -= fall_velocity * t;
	fall_velocity += 0.3;

	if (y <= 0) {
		isFalling = false;
		isOnFloor = true;
		y = 0;
	}
}

void BearGrylls::grow()
{
	if (scale < final_size) 
	{
		scale += 0.1;
	}
	else 
	{
		reachedFinalSize = true;
	}
}

void BearGrylls::setBearGryllsIsHere(bool value)
{
	isBearGryllsHere = value;
}

bool BearGrylls::getBearGryllsIsHere()
{
	return isBearGryllsHere;
}

bool BearGrylls::getIsFalling()
{
	return isFalling;
}

void BearGrylls::setFalling(bool value)
{
	isFalling = value;
}

bool BearGrylls::getIsOnFloor()
{
	return isOnFloor;
}

void BearGrylls::setStopTime(float time)
{
	stop_time = time;
}

void BearGrylls::setIsMoving(bool moving)
{
	isMoving = moving;
}

glm::vec3 BearGrylls::getPosition()
{
	return glm::vec3(x, y, z);
}

float BearGrylls::getHeight()
{
	return scale;
}

bool BearGrylls::getReachedFinalSize()
{
	return reachedFinalSize;
}

