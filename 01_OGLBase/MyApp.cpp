#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

CMyApp::CMyApp(void)
{
	m_camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	srand((unsigned)time(NULL));
	animation_last_time = SDL_GetTicks() / 1000.0f;
}

CMyApp::~CMyApp(void)
{
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
			glm::vec3(1, -1, -1),
			glm::vec3(1, 1, -1),
			glm::vec3(-1, 1, -1),
			// elülső lap
			glm::vec3(-1, -1, 1),
			glm::vec3(1, -1, 1),
			glm::vec3(1, 1, 1),
			glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az előző tömbökből) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
		// hátsó lap
		0, 1, 2,
			2, 3, 0,
			// elülső lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felső
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
		);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_skyboxTexture.AttachFromFile("assets/xpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile("assets/xneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile("assets/ypos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/yneg.png", false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile("assets/zpos.png", false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile("assets/zneg.png", true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	// a GL_TEXTURE_MAG_FILTER-t és a GL_TEXTURE_MIN_FILTER-t beállítja az AttachFromFile
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CMyApp::InitShaders()
{

	// a shadereket tároló program létrehozása az OpenGL-hez hasonló módon:
	m_program.AttachShaders({
		{ GL_VERTEX_SHADER, "myVert.vert"},
		{ GL_FRAGMENT_SHADER, "myFrag.frag"}
		});

	// attributomok osszerendelese a VAO es shader kozt
	m_program.BindAttribLocations({
		{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		{ 1, "vs_in_norm" },			// VAO 1-es csatorna menjen a vs_in_norm-ba
		{ 2, "vs_in_tex" },				// VAO 2-es csatorna menjen a vs_in_tex-be
		});

	m_program.LinkProgram();


	// SkyBox
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },
		}
		);

	// Bear Grylls
	m_BearGryllsProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "bg.vert"},
		{ GL_FRAGMENT_SHADER, "bg.frag"}
		});

	m_BearGryllsProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		{ 1, "vs_in_norm" },
		{ 2, "vs_in_tex" },
		});

	m_BearGryllsProgram.LinkProgram();
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitShaders();
	InitSkyBox();

	// egyéb textúrák betöltése
	m_woodTexture.FromFile("assets/wood.jpg");

	floor.Init(floor_size);
	rocks.Init();
	trees.Init();
	bearGryllsHimself.Init(floor_size);
	lions.Init();
	zebras.Init();
	confetti.Init(bearGryllsHimself.getHeight());

	// kamera
	m_camera.SetProj(glm::radians(60.0f), 640.0f / 480.0f, 0.01f, 1000.0f);

	//m_loc_eye = glGetUniformLocation(m_programID, "eye_pos");
	//m_loc_eye = m_program.GetLocation("eye_pos");

	return true;
}

void CMyApp::Clean()
{
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();

	// Light
	float time = SDL_GetTicks() / 1000.0f * 2 * float(M_PI) / 10;

	if (time - lastTime > timeDiff && alive) {
		beta += betaPlus;
		if (beta >= 2 * M_PI) beta = 0;

		lightPos.y += 1 * glm::cos(beta);
		lightPos.x += -2.3 * glm::sin(beta);

		if (beta >= M_PI) {
			lightDir = glm::vec3(0, 0, 0);
		}
		else {
			lightDir = glm::vec3(lightPos.x, -lightPos.y, -lightPos.z);
		}

		// ambiens feny
		if (lightPos.y >= 22)
		{
			sunLight = glm::vec3(1, 0.99, 0.5);
			ambientCol = glm::vec3(0.5, 0.5, 0.5);
			softAmbient = glm::vec3(1, 1, 1);
		}
		else if (lightPos.y >= 0 && lightPos.y < 22)
		{
			sunLight = glm::vec3(0.95, 0.416, 0.416);
			ambientCol = glm::vec3(1, 0.2, 0.2); // 255, 51, 51 -> piros
			softAmbient = glm::vec3(1, 0.8, 0.8);
			lightIntensity += (lightIntensity < 1.3) ? 0.05 : 0;
		}
		else
		{
			sunLight = glm::vec3(0.95, 0.416, 0.416);
			ambientCol = glm::vec3(0.2, 0.4, 1); // 51, 102, 255 -> kek
			softAmbient = glm::vec3(0.8, 0.8, 1);
			lightIntensity -= (lightIntensity > 0.5) ? 0.05 : 0;
		}

		lastTime = time;
	}

	// Bear Grylls
	if (bearGryllsHimself.getBearGryllsIsHere() && alive)
	{
		newTime = SDL_GetTicks() / 1000.0f;

		if (newTime - oldTime <= period) {
			ldx += is_lighted * light_diff;
		}
		else {
			is_lighted *= -1;
			oldTime = newTime;
		}

		if (bearGryllsHimself.getIsFalling()) {
			bearGryllsHimself.drop(getElapsedTime());
		}

		if (bearGryllsHimself.getIsOnFloor() && lions_spawned && lions.isMeshBufferInitialized()) {
			alive = !bearGryllsHimself.rotateTowardsNearestLion(lions.getLionPositions());
		}
	}
	else if (!alive) {
		bearGryllsHimself.grow();
	}

	// Lions
	if (lions_spawned && alive && lions.isMeshBufferInitialized())
	{
		lions.Update(zebras.GetCurrentPositions());
	}

	// Zebras
	if (zebras_spawned && alive)
	{
		if (lions_spawned && lions.isMeshBufferInitialized()) {
			zebras.Update(lions.getLions());
		}
		else {
			zebras.Update(lions.emptyLions());
		}
	}

	// Confetti
	if (bearGryllsHimself.getReachedFinalSize()) 
	{
		confetti.setPosition(bearGryllsHimself.getPosition(), bearGryllsHimself.getHeight());
	}
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const glm::mat4 viewProj = m_camera.GetViewProj();

	m_program.Use();

	m_program.SetUniform("ambientCol", ambientCol);
	m_program.SetUniform("sunLight", sunLight);
	m_program.SetUniform("lightPos", lightPos);
	m_program.SetUniform("lightDir", lightDir);

	rocks.Render(&m_program, viewProj);
	trees.Render(&m_program, viewProj);
	floor.Render(&m_program, viewProj);

	// Zebras
	if (zebras_spawned)
	{
		zebras.Render(&m_program, viewProj);

		if ((SDL_GetTicks() - startTime_zebra) / 1000.0 > 10)
		{
			lions_spawned = true;
		}
	}

	// Lions
	if (lions_spawned)
	{
		lions.Render(&m_program, viewProj);
	}

	// Confetti
	if (bearGryllsHimself.getReachedFinalSize()) 
	{
		confetti.Render(&m_program, viewProj, getElapsedTime());
	}

	m_program.Unuse();


	// Bear Grylls
	if (bearGryllsHimself.getBearGryllsIsHere()) {
		m_BearGryllsProgram.Use();
		bearGryllsHimself.Render(&m_BearGryllsProgram, viewProj, ldx, ambientCol * 0.6F);
		m_BearGryllsProgram.Unuse();
	}

	// skybox
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", viewProj * glm::translate(m_camera.GetEye()));
	m_programSkybox.SetUniform("La", softAmbient * lightIntensity);
	m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	glDepthFunc(prevDepthFnc);


	ImGui::ShowTestWindow();
	animation_last_time = SDL_GetTicks() / 1000.0f;
}

void CMyApp::SpawnZebras()
{
	zebras_spawned = true;
	startTime_zebra = SDL_GetTicks();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

// a két paraméterben az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}

// BEAR GRYLLS
void CMyApp::ShowBearGrylls()
{
	bearGryllsHimself.setBearGryllsIsHere(true);
}

void CMyApp::MoveBearGrylls(int new_direction)
{
	float t = 0.1;

	if (alive) {
		bearGryllsHimself.setIsMoving(true);

		if (new_direction == 0) {
			bearGryllsHimself.moveUp(t);
		}
		else if (new_direction == 2) {
			bearGryllsHimself.moveDown(t);
		}
		else if (new_direction == 1) {
			bearGryllsHimself.moveLeft(t);
		}
		else if (new_direction == 3) {
			bearGryllsHimself.moveRight(t);
		}

		bearGryllsHimself.setStopTime(SDL_GetTicks() / 1000.0f);
	}
}

void CMyApp::DropBearGrylls()
{
	bearGryllsHimself.setFalling(true);
}

float CMyApp::getElapsedTime()
{
	return SDL_GetTicks() / 1000.0f - animation_last_time;
}