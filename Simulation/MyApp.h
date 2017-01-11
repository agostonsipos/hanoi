#pragma once

// GLEW
#include <GL/glew.h>

// SDL
#include <SDL.h>
#include <SDL_opengl.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include "gCamera.h"
#include "gShaderProgram.h"
#include "gVertexBuffer.h"
#include "Mesh_OGL3.h"

class CMyApp
{
public:
	CMyApp(void);
	~CMyApp(void);

	bool Init();
	void Clean();

	void Update();
	void Render();

	void KeyboardDown(SDL_KeyboardEvent&);
	void KeyboardUp(SDL_KeyboardEvent&);
	void MouseMove(SDL_MouseMotionEvent&);
	void MouseDown(SDL_MouseButtonEvent&);
	void MouseUp(SDL_MouseButtonEvent&);
	void MouseWheel(SDL_MouseWheelEvent&);
	void Resize(int, int);
protected:
	GLuint GenTexture();

	GLuint m_textureID;
	GLuint m_korong_textureID;
	GLuint m_oszlop_textureID;

	gCamera			m_camera;
	gShaderProgram	m_program;
	gVertexBuffer	m_vb;

	Mesh			*m_korong;
	Mesh			*m_oszlop;

	int active = 0;

	int korong_pos[7];
	bool korong_up[7];
	int korong_height[7];

	int dir[7];
	int updir[7];
	float t;

	int korong_num[3];

	int top_korong(int oszlop);
};

