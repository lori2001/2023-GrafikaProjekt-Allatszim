#pragma once

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

// mesh
#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include <math.h>

class BearGrylls {
public:
	BearGrylls();
	~BearGrylls();

	bool Init(float);
	void Render(ProgramObject*, glm::mat4, float, glm::vec3);

	// Update
	bool rotateTowardsNearestLion(std::vector<glm::vec3>);
	void moveUp(float);
	void moveDown(float);
	void moveLeft(float);
	void moveRight(float);
	void drop(float);
	void grow();

	// Getters, setters
	void setBearGryllsIsHere(bool);
	bool getBearGryllsIsHere(void);
	bool getIsFalling();
	void setFalling(bool);
	bool getIsOnFloor();
	void setStopTime(float);
	void setIsMoving(bool);
	glm::vec3 getPosition();
	float getHeight();
	bool getReachedFinalSize();

private:
	std::unique_ptr<Mesh>	m_BearGryllsMesh;
	Texture2D				m_bearGryllsTexture;
	glm::mat4				bearGryllsWorld;

	// Bear Grylls
	bool					isBearGryllsHere;
	bool					isFalling;
	bool					isOnFloor;
	bool					isMoving;
	bool					reachedFinalSize;

	float					x, y, z, scale, angle;
	float					move_velocity, fall_velocity;
	float					final_size;

	// Other
	float					floor_limit;
	float					lion_distance_2;				// négyzetes távolság
	float					floor_diagonal_2;				// négyzetes távolság
	float					stop_time;
};
