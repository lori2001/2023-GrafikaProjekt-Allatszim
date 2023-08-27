#pragma once

// C++ includes
#include <memory>

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "includes/gCamera.h"

#include "includes/ProgramObject.h"
#include "includes/BufferObject.h"
#include "includes/VertexArrayObject.h"
#include "includes/TextureObject.h"

// mesh
#include "includes/ObjParser_OGL3.h"

#include "Rocks.h"
#include "Trees.h"
#include "Zebras.h"

#include "SavannaFloor.h"
#include "BearGrylls.h"
#include "Lions.h"
#include "Confetti.h"

class CMyApp
{
public:
	CMyApp();
	~CMyApp();

	bool Init();
	void Clean();

	void Update();
	void Render();

	void ShowBearGrylls();
	void MoveBearGrylls(int);
	void DropBearGrylls();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void SpawnZebras();
	void Resize(int, int);

protected:
	// shaderekhez szükséges változók
	ProgramObject		m_program;			// mesh shader
	ProgramObject		m_programSkybox;	// skybox shader
	ProgramObject		m_BearGryllsProgram;

	VertexArrayObject	m_CubeVao;			// VAO
	IndexBuffer			m_CubeIndices;		// index buffer
	ArrayBuffer			m_CubeVertexBuffer;	// VBO
	
	VertexArrayObject	m_SkyboxVao;
	IndexBuffer			m_SkyboxIndices;	
	ArrayBuffer			m_SkyboxPos;		

	gCamera				m_camera;

	Texture2D			m_woodTexture;
	TextureCubeMap		m_skyboxTexture;


	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec2 t;
	};

	struct Vertex2
	{
		glm::vec3 p;
		glm::vec3 n;
	};

	// obj
	Lions lions;
	Rocks rocks;
	SavannaFloor floor;
	Trees trees;
	BearGrylls bearGryllsHimself;
	Confetti confetti;

	// global
	float floor_size = 40.0f;
	float animation_last_time = 0.0f;
	bool alive = true;
	Zebras zebras;
	bool zebras_spawned = false;
	bool lions_spawned = false;

	// a napnak a pozicioja es az iranya
	const glm::vec3 INITIAL_LIGHT_DIR = glm::vec3(0.0F, -90.0F * (M_PI / 180.0F), 0);
	const glm::vec3 INITIAL_LIGHT_POS = glm::vec3(45.0F, -5, 0);
	glm::vec3 lightPos = INITIAL_LIGHT_POS;
	glm::vec3 lightDir = INITIAL_LIGHT_DIR;
	glm::vec3 ambientCol = glm::vec3(1, 1, 1);
	glm::vec3 sunLight = glm::vec3(1, 1, 1);
	glm::vec3 softAmbient;

	float beta = 0;
	const float betaPlus = 3.14 / 60;

	float lastTime = 0;
	float timeDiff = 0.08F;
	float lightIntensity = 0.0F;

	// pulzalo feny
	float period = 1.0F;
	float oldTime = 0.0F;
	float newTime = 0.0F;

	//timer a zebrak spawnolasahoz
	uint32_t startTime_zebra;
	uint32_t currTime_zebra;

	float light_diff = 0.05F;
	int is_lighted = -1;
	float ldx = 0.5F;

	// a jobb olvashatóság kedvéért
	void InitShaders();
	void InitCube();
	void InitSkyBox();
	float getElapsedTime();
};

