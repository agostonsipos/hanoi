#include "MyApp.h"
#include "GLUtils.hpp"

#include <GL/glu.h>
#include <math.h>

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
	m_textureID = 0;
	m_korong = 0;
	m_oszlop = 0;
	memset(korong_pos, 0, 7 * sizeof(int));
	memset(korong_up, 0, 7 * sizeof(bool));
	memset(dir, 0, 7 * sizeof(int));
	memset(updir, 0, 7 * sizeof(int));
	memset(korong_num, 0, 3 * sizeof(int));
	korong_num[0] = 7;
}


CMyApp::~CMyApp(void)
{
}


GLuint CMyApp::GenTexture()
{
    unsigned char tex[256][256][3];
 
    for (int i=0; i<256; ++i)
        for (int j=0; j<256; ++j)
        {
			tex[i][j][0] = rand()%256;
			tex[i][j][1] = rand()%256;
			tex[i][j][2] = rand()%256;
        }
 
	GLuint tmpID;

    glGenTextures(1, &tmpID);
    glBindTexture(GL_TEXTURE_2D, tmpID);
    gluBuild2DMipmaps(  GL_TEXTURE_2D, GL_RGB8, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tmpID;
}

bool CMyApp::Init()
{
	for (int i = 0; i < 7; ++i){
		korong_height[i] = 6 - i;
	}
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_vb.AddAttribute(0, 3);
	m_vb.AddAttribute(1, 3);
	m_vb.AddAttribute(2, 2);

	m_vb.AddData(0, -50,  0, -50);
	m_vb.AddData(0,  50,  0, -50);
	m_vb.AddData(0, -50,  0,  50);
	m_vb.AddData(0,  50,  0,  50);

	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);

	m_vb.AddData(2, 0, 0);
	m_vb.AddData(2, 1, 0);
	m_vb.AddData(2, 0, 1);
	m_vb.AddData(2, 1, 1);

	m_vb.AddIndex(1, 0, 2);
	m_vb.AddIndex(1, 2, 3);

	m_vb.InitBuffers();

	m_program.AttachShader(GL_VERTEX_SHADER, "dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_normal");
	m_program.BindAttribLoc(2, "vs_in_tex0");

	if ( !m_program.LinkProgram() )
	{
		return false;
	}

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

	m_textureID = TextureFromFile("desk_texture.bmp");
	m_korong_textureID = TextureFromFile("wood_texture.bmp");
	m_oszlop_textureID = TextureFromFile("stone_texture.bmp");

	m_korong = ObjParser::parse("disk.obj");
	m_korong->initBuffers();
	m_oszlop = ObjParser::parse("tower.obj");
	m_oszlop->initBuffers();

	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_textureID);

	m_program.Clean();
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}


void CMyApp::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program.On();

	glm::mat4 matWorld = glm::mat4(1.0f);
	glm::mat4 matWorldIT = glm::transpose( glm::inverse( matWorld ) );
	glm::mat4 mvp = m_camera.GetViewProj() *matWorld;

	m_program.SetUniform( "world", matWorld );
	m_program.SetUniform( "worldIT", matWorldIT );
	m_program.SetUniform( "MVP", mvp );
	m_program.SetUniform( "eye_pos", m_camera.GetEye() );

	m_program.SetTexture("texImage", 0, m_textureID);

	m_vb.On();

	m_vb.DrawIndexed(GL_TRIANGLES, 0, 6, 0);

	m_vb.Off();

	glm::mat4 oszlop_pos[3] = {glm::translate<float>(glm::vec3(-20,0,0)), glm::mat4(1.0f), glm::translate<float>(glm::vec3(20,0,0))};

	for (int i = 0; i < 3; ++i){
		matWorld = oszlop_pos[i];
		if (i == active) matWorld *= glm::scale<float>(glm::vec3(1.0f, 1.1f, 1.0f));
		matWorldIT = glm::transpose(glm::inverse(matWorld));
		mvp = m_camera.GetViewProj() *matWorld;

		m_program.SetUniform("world", matWorld);
		m_program.SetUniform("worldIT", matWorldIT);
		m_program.SetUniform("MVP", mvp);
		m_program.SetUniform("eye_pos", m_camera.GetEye());

		m_program.SetTexture("texImage", 0, m_oszlop_textureID);

		m_oszlop->draw();
	}

	m_program.Off();


	m_program.On();
	bool b = false;
	for (int i = 0; i < 7; ++i){
		if (korong_up[i]) b = true;
	}

	for (int i = 0; i < 7; ++i){
		glm::mat4 meret = glm::scale<float>(glm::vec3(1.0f + i * 0.2f, 1.0f, 1.0f + i * 0.2f));

		matWorld = glm::mat4(1.0f);

		if (korong_up[i]) matWorld = glm::translate<float>(glm::vec3(0, 20, 0));
		else matWorld = glm::translate<float>(glm::vec3(0, korong_height[i], 0));

		matWorld *= glm::translate<float>(glm::vec3(0, (SDL_GetTicks() - t) / 1000.0f * (20-korong_height[i]) * updir[i], 0));

		if (updir[i] && SDL_GetTicks() - t >= 1000){ 
			updir[i] = 0; korong_up[i] = !korong_up[i]; 
		}

		matWorld *= glm::translate<float>(glm::vec3((SDL_GetTicks() - t) / 1000.0f * 20 * dir[i], 0, 0)) * oszlop_pos[korong_pos[i] ];

		if (dir[i] && SDL_GetTicks() - t >= 1000){ 
			dir[i] = 0; korong_pos[i] = active; 
			korong_height[i] = (top_korong(active)==-1?0:korong_height[top_korong(active)]+1);
		}

		matWorld *= meret;
		matWorldIT = glm::transpose(glm::inverse(matWorld));
		mvp = m_camera.GetViewProj() *matWorld;

		if (korong_up[i]){
			m_program.SetUniform("Kd", glm::vec4(0.4f, 0.4f, 1.0f, 1.0f));
			m_program.SetUniform("Ks", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			m_program.SetUniform("korong_up", 1);
			m_program.SetUniform("korong_pos", glm::vec3(matWorld[3]));
		}
		else{
			m_program.SetUniform("Kd", glm::vec4(0.3f, 0.7f, 0.4f, 1.0f));
			m_program.SetUniform("Ks", glm::vec4(0.3f, 0.7f, 0.4f, 1.0f));
		}
		if (!b) m_program.SetUniform("korong_up", 0);

		m_program.SetUniform("world", matWorld);
		m_program.SetUniform("worldIT", matWorldIT);
		m_program.SetUniform("MVP", mvp);
		m_program.SetUniform("eye_pos", m_camera.GetEye());

		m_program.SetTexture("texImage", 0, m_korong_textureID);

		m_korong->draw();

	}
	m_program.Off();
}

int CMyApp::top_korong(int oszlop){
	int min = -1, ind;
	for (int i = 0; i < 7; ++i){
		if (korong_pos[i] == oszlop && !korong_up[i] && korong_height[i] > min){
			ind = i;
			min = korong_height[i];
		}
	}
	if (min != -1) return ind;
	else return -1;
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);
	for (int i = 0; i < 7; ++i)
		if (dir[i] != 0 || updir[i] != 0) return;
	switch (key.keysym.sym){
	case SDLK_LEFT:
		if (active != 0){
			--active;
			for (int i = 0; i < 7; ++i)
			if (korong_up[i]){
				t = static_cast<float>(SDL_GetTicks());
				dir[i] = -1;
			}
		}
		break;
	case SDLK_RIGHT:
		if (active != 2){
			++active;
			for (int i = 0; i < 7; ++i)
			if (korong_up[i]){
				t = static_cast<float>(SDL_GetTicks());
				dir[i] = 1;
			}
		}
		break;
	case SDLK_SPACE:
		bool b = false;
		int i;
		for (i = 0; !b && i < 7; ++i)
			b = korong_up[i];
		if (b){
			updir[i-1] = -1;
			t = static_cast<float>(SDL_GetTicks());
			return;
		}
		i = top_korong(active);
		if (i == -1) return;
		if (korong_pos[i] == active){
			updir[i] = 1;
			t = static_cast<float>(SDL_GetTicks());
		}
	}
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

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}
