#pragma once

#include "../Application.h"
#include "tb_system.h"
#include "tb_skin.h"
#include "tb_msg.h"
#include "tb_font_renderer.h"
#include "tb_widgets.h"
#include "tb_editfield.h"
#include "animation/tb_widget_animation.h"
#include "renderers/tb_renderer_gl.h"

#include <SDL.h>

class AppBackendSDL : public AppBackend
{
public:
	bool Init(App *app);
	AppBackendSDL() : m_app(nullptr)
		, m_window(nullptr)
		, m_pause(false)
		, m_cursor_arrow(nullptr)
		, m_cursor_i_beam(nullptr)
		// , m_cursor_i_beam(nullptr)
	{
		m_has_pending_update = false;
		m_quit_requested = false;
	}

	~AppBackendSDL();

	virtual void OnAppEvent(const EVENT &ev);

	virtual void Update();

	tb::TBWidget *GetRoot() const {
		return m_app->GetRoot();
	}
	int GetWidth() const {
		return m_app->GetWidth();
	}
	int GetHeight() const {
		return m_app->GetHeight();
	}

private:
	App *m_app;

	SDL_Window* m_window;
	SDL_GLContext m_gl_context;

	SDL_Event m_evt_cache;

	SDL_Cursor* m_cursor_arrow;
	SDL_Cursor* m_cursor_i_beam;

	bool m_pause;

	tb::MODIFIER_KEYS GetModifierKeys();
	tb::MODIFIER_KEYS GetModifierKeys(Uint16 glfwmod);
	bool InvokeShortcut(int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down);
	bool InvokeKey(unsigned int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down);

	void HandleWindowEvent(const SDL_Event &evt);
	void HandlePointerMoveEvent(const SDL_Event & evt);
	void HandleKeyPressEvent(const SDL_Event & evt, bool press);
	void HandleMouseButtonEvent(const SDL_Event & evt, bool press);
};