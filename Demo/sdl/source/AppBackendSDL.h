#pragma once

#include "../Application.h"
#include "tb_system.h"
#include "tb_skin.h"
#include "tb_msg.h"
#include "tb_editfield.h"
#include "renderers/tb_renderer_gl.h"
#include "tb_font_renderer.h"

#include <SDL.h>

class AppBackendSDL : public AppBackend
{
public:
	bool Init(App *app);
	AppBackendSDL() : m_app(nullptr)
		, m_renderer(nullptr)
		, mainWindow(nullptr)
		// , m_cursor_i_beam(nullptr)
		, m_has_pending_update(false)
		, m_quit_requested(false) {}
	~AppBackendSDL();

	virtual void OnAppEvent(const EVENT &ev);

	tb::TBWidget *GetRoot() const {
		return m_app->GetRoot();
	}
	int GetWidth() const {
		return m_app->GetWidth();
	}
	int GetHeight() const {
		return m_app->GetHeight();
	}

	App *m_app;
	tb::TBRendererGL *m_renderer;

	SDL_Window* mainWindow;
	// GLFWcursor *m_cursor_i_beam;

	bool m_has_pending_update;
	bool m_quit_requested;
};