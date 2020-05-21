#include "application.h"

#include "animation/tb_widget_animation.h"
#include "tb_system.h"

#include <stdio.h>
#include <algorithm>

namespace SDL_Demo {

	// https://lazyfoo.net/tutorials/SDL/50_SDL_and_opengl_2/index.php
	bool Application::init_gl() {
		GLenum error = GL_NO_ERROR;

		// Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error initializing OpenGL! %s\n", gluErrorString(error));
			return false;
		}

		// Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error initializing OpenGL! %s\n", gluErrorString(error));
			return false;
		}

		// Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		// Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error initializing OpenGL! %s\n", gluErrorString(error));
			return false;
		}

		return true;
	}

	bool Application::Init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			return false;
		}

		m_window = SDL_CreateWindow("Turbobadger SDL Demo",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			m_widht,
			m_height,
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (m_window == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}

		m_gl_context = SDL_GL_CreateContext(m_window);
		if (m_gl_context == NULL)
		{
			printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
		}

		//Initialize OpenGL
		if (!init_gl())
		{
			printf("Unable to initialize OpenGL!\n");
			return false;
		}

		SDL_GL_MakeCurrent(m_window, m_gl_context);

		tb::TBWidgetsAnimationManager::Init();

		g_renderer = new tb::TBRendererGL();
		tb_core_init(g_renderer);

		return true;
	}

	void Application::Run() {
		m_running = true;
		while (m_running) {
			while (SDL_PollEvent(&m_evt_cache) != 0)
			{
				// User requests quit
				if (m_evt_cache.type == SDL_QUIT)
				{
					m_running = false;
				}

				Process();
				Render();
			}
		}
	}

	void Application::Process()
	{
		TBAnimationManager::Update();

		m_root.InvokeProcessStates();
		m_root.InvokeProcess();
	}

	void Application::Render() {
		g_renderer->BeginPaint(m_root.GetRect().w, m_root.GetRect().h);
		m_root.InvokePaint(TBWidget::PaintProps());
		g_renderer->EndPaint();

		// If animations are running, reinvalidate immediately
		if (TBAnimationManager::HasAnimationsRunning())
		{
			GetRoot()->Invalidate();
		}
	}

	void Application::Shutdown() {
		tb::TBWidgetsAnimationManager::Shutdown();

		if (g_renderer != nullptr)
		{
			delete g_renderer;
		}

		if (m_window != nullptr)
		{
			SDL_DestroyWindow(m_window);
		}

		// Quit SDL subsystems
		SDL_Quit();
	}
}