#include "application.h"

#include "animation/tb_widget_animation.h"

#include <stdio.h>

namespace SDL_Demo {
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
			SDL_WINDOW_SHOWN);

		if (m_window == nullptr)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			return false;
		}

		tb::TBWidgetsAnimationManager::Init();

		m_renderer = new tb::TBRendererGL();
		tb_core_init(m_renderer);
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
			}
		}
	}

	void Application::Shutdown() {
		tb::TBWidgetsAnimationManager::Shutdown();

		if (m_window != nullptr) {
			SDL_DestroyWindow(m_window);
		}

		// Quit SDL subsystems
		SDL_Quit();
	}
}