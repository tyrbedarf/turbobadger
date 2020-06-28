#include "AppBackendSDL.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include <stdio.h>
#include <algorithm>

AppBackendSDL::~AppBackendSDL() {
	m_app->OnBackendDetached();
	m_app = nullptr;

	tb::tb_core_shutdown();

	if (m_cursor_arrow != nullptr) {
		SDL_FreeCursor(m_cursor_arrow);
		m_cursor_arrow = nullptr;
	}

	if (m_cursor_i_beam != nullptr) {
		SDL_FreeCursor(m_cursor_i_beam);
		m_cursor_i_beam = nullptr;
	}

	if (m_window != nullptr)
	{
		SDL_DestroyWindow(m_window);
	}

	// Quit SDL subsystems
	SDL_Quit();

	delete tb::g_renderer;
}

bool AppBackendSDL::Init(App *app) {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	m_window = SDL_CreateWindow("Turbobadger SDL Demo",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		app->GetWidth(),
		app->GetHeight(),
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	if (m_window == nullptr)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	m_gl_context = SDL_GL_CreateContext(m_window);
	if (m_gl_context == nullptr)
	{
		printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	m_cursor_arrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
	SDL_SetCursor(m_cursor_arrow);
	if (m_cursor_arrow == nullptr)
	{
		printf("Could not create arrow cursor! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	m_cursor_i_beam = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
	if (m_cursor_i_beam == nullptr)
	{
		printf("Could not create beam cursor! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_GL_MakeCurrent(m_window, m_gl_context);

	tb::g_renderer = new tb::TBRendererGL();
	tb::tb_core_init(tb::g_renderer);

	m_app = app;
	m_app->OnBackendAttached(this, m_app->GetWidth(), m_app->GetHeight());

	return true;
}

tb::MODIFIER_KEYS AppBackendSDL::GetModifierKeys(Uint16 mod) {
	tb::MODIFIER_KEYS code = tb::TB_MODIFIER_NONE;

	if ((mod & KMOD_LALT) != 0)		code |= tb::TB_ALT;
	if ((mod & KMOD_RALT) != 0)		code |= tb::TB_ALT;
	if ((mod & KMOD_LCTRL) != 0)	code |= tb::TB_CTRL;
	if ((mod & KMOD_RCTRL) != 0)	code |= tb::TB_CTRL;
	if ((mod & KMOD_LSHIFT) != 0)	code |= tb::TB_SHIFT;
	if ((mod & KMOD_RSHIFT) != 0)	code |= tb::TB_SHIFT;
	if ((mod & KMOD_LGUI) != 0)		code |= tb::TB_SUPER;
	if ((mod & KMOD_RGUI) != 0)		code |= tb::TB_SUPER;

	return code;
}

tb::MODIFIER_KEYS AppBackendSDL::GetModifierKeys() {
	SDL_Keymod mod = SDL_GetModState();
	return GetModifierKeys(mod);
}

bool AppBackendSDL::InvokeShortcut(int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down)
{
#ifdef TB_TARGET_MACOSX
	bool shortcut_key = (modifierkeys & tb::TB_SUPER) ? true : false;
#else
	bool shortcut_key = (modifierkeys & tb::TB_CTRL) ? true : false;
#endif
	if (!tb::TBWidget::focused_widget || !down || !shortcut_key)
		return false;

	bool reverse_key = (modifierkeys & tb::TB_SHIFT) ? true : false;

	tb::TBID id;
	if (key == 'x')
		id = TBIDC("cut");
	else if (key == 'c' || special_key == tb::TB_KEY_INSERT)
		id = TBIDC("copy");
	else if (key == 'v' || (special_key == tb::TB_KEY_INSERT && reverse_key))
		id = TBIDC("paste");
	else if (key == 'a')
		id = TBIDC("selectall");
	else if (key == 'z' || key == 'y')
	{
		bool undo = key == 'z';
		if (reverse_key)
			undo = !undo;
		id = undo ? TBIDC("undo") : TBIDC("redo");
	}
	else if (key == 'n')
		id = TBIDC("new");
	else if (key == 'o')
		id = TBIDC("open");
	else if (key == 's')
		id = TBIDC("save");
	else if (key == 'w')
		id = TBIDC("close");
	else if (special_key == tb::TB_KEY_PAGE_UP)
		id = TBIDC("prev_doc");
	else if (special_key == tb::TB_KEY_PAGE_DOWN)
		id = TBIDC("next_doc");
	else
		return false;

	tb::TBWidgetEvent ev(tb::EVENT_TYPE_SHORTCUT);
	ev.modifierkeys = modifierkeys;
	ev.ref_id = id;

	return tb::TBWidget::focused_widget->InvokeEvent(ev);
}

bool AppBackendSDL::InvokeKey(unsigned int key, tb::SPECIAL_KEY special_key, tb::MODIFIER_KEYS modifierkeys, bool down)
{
	if (InvokeShortcut(key, special_key, modifierkeys, down))
		return true;

	m_app->GetRoot()->InvokeKey(key, special_key, modifierkeys, down);

	return true;
}

void AppBackendSDL::HandleWindowEvent(const SDL_Event &evt) {
	switch (evt.window.event) {
	case SDL_WINDOWEVENT_SHOWN:
		// Trigger repaint
		m_has_pending_update = true;
		m_pause = false;
		break;
	case SDL_WINDOWEVENT_HIDDEN:
		// Trigger stop updating
		m_pause = true;
		m_has_pending_update = false;
		break;
	case SDL_WINDOWEVENT_EXPOSED:
		// Trigger repaint
		m_has_pending_update = true;
		m_pause = false;
		break;
	case SDL_WINDOWEVENT_MOVED:
		break;
	case SDL_WINDOWEVENT_RESIZED:
		m_app->OnResized(evt.window.data1, evt.window.data2);
		break;
	case SDL_WINDOWEVENT_SIZE_CHANGED:
		m_app->OnResized(evt.window.data1, evt.window.data2);
		break;
	case SDL_WINDOWEVENT_MINIMIZED:
		m_pause = true;
		m_has_pending_update = false;
		break;
	case SDL_WINDOWEVENT_MAXIMIZED:
		m_pause = false;
		m_has_pending_update = true;
		break;
	case SDL_WINDOWEVENT_RESTORED:
		m_pause = false;
		m_has_pending_update = true;
		break;
	case SDL_WINDOWEVENT_ENTER:
		break;
	case SDL_WINDOWEVENT_LEAVE:
		break;
	case SDL_WINDOWEVENT_FOCUS_GAINED:
		break;
	case SDL_WINDOWEVENT_FOCUS_LOST:
		break;
	case SDL_WINDOWEVENT_CLOSE:
		m_quit_requested = true;
		break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
	case SDL_WINDOWEVENT_TAKE_FOCUS:
		break;
	case SDL_WINDOWEVENT_HIT_TEST:
		break;
#endif
	default:
		SDL_Log("Window %d got unknown event %d", evt.window.windowID, evt.window.event);
		break;
	}
}

void AppBackendSDL::HandlePointerMoveEvent(const SDL_Event & evt) {
	int mouse_x = (int) evt.motion.x;
	int mouse_y = (int) evt.motion.y;

	m_app->GetRoot()->InvokePointerMove(mouse_x, mouse_y, GetModifierKeys(), false);

	if (m_app->GetRoot() && tb::TBWidget::hovered_widget) {
		tb::TBWidget *active_widget = tb::TBWidget::captured_widget ? tb::TBWidget::captured_widget : tb::TBWidget::hovered_widget;
		if (tb::TBSafeCast<tb::TBEditField>(active_widget)) {
			SDL_SetCursor(m_cursor_i_beam);
		}
		else {
			SDL_SetCursor(m_cursor_arrow);
		}
	}
}

void AppBackendSDL::HandleKeyPressEvent(const SDL_Event & evt, bool press) {
	tb::MODIFIER_KEYS modifier = GetModifierKeys(evt.key.keysym.mod);

	switch (evt.key.keysym.scancode)
	{
	case SDL_SCANCODE_F1:			InvokeKey(0, tb::TB_KEY_F1, modifier, press); break;
	case SDL_SCANCODE_F2:			InvokeKey(0, tb::TB_KEY_F2, modifier, press); break;
	case SDL_SCANCODE_F3:			InvokeKey(0, tb::TB_KEY_F3, modifier, press); break;
	case SDL_SCANCODE_F4:			InvokeKey(0, tb::TB_KEY_F4, modifier, press); break;
	case SDL_SCANCODE_F5:			InvokeKey(0, tb::TB_KEY_F5, modifier, press); break;
	case SDL_SCANCODE_F6:			InvokeKey(0, tb::TB_KEY_F6, modifier, press); break;
	case SDL_SCANCODE_F7:			InvokeKey(0, tb::TB_KEY_F7, modifier, press); break;
	case SDL_SCANCODE_F8:			InvokeKey(0, tb::TB_KEY_F8, modifier, press); break;
	case SDL_SCANCODE_F9:			InvokeKey(0, tb::TB_KEY_F9, modifier, press); break;
	case SDL_SCANCODE_F10:			InvokeKey(0, tb::TB_KEY_F10, modifier, press); break;
	case SDL_SCANCODE_F11:			InvokeKey(0, tb::TB_KEY_F11, modifier, press); break;
	case SDL_SCANCODE_F12:			InvokeKey(0, tb::TB_KEY_F12, modifier, press); break;
	case SDL_SCANCODE_LEFT:			InvokeKey(0, tb::TB_KEY_LEFT, modifier, press); break;
	case SDL_SCANCODE_UP:			InvokeKey(0, tb::TB_KEY_UP, modifier, press); break;
	case SDL_SCANCODE_RIGHT:		InvokeKey(0, tb::TB_KEY_RIGHT, modifier, press); break;
	case SDL_SCANCODE_DOWN:			InvokeKey(0, tb::TB_KEY_DOWN, modifier, press); break;
	case SDL_SCANCODE_PAGEUP:		InvokeKey(0, tb::TB_KEY_PAGE_UP, modifier, press); break;
	case SDL_SCANCODE_PAGEDOWN:		InvokeKey(0, tb::TB_KEY_PAGE_DOWN, modifier, press); break;
	case SDL_SCANCODE_HOME:			InvokeKey(0, tb::TB_KEY_HOME, modifier, press); break;
	case SDL_SCANCODE_END:			InvokeKey(0, tb::TB_KEY_END, modifier, press); break;
	case SDL_SCANCODE_INSERT:		InvokeKey(0, tb::TB_KEY_INSERT, modifier, press); break;
	case SDL_SCANCODE_TAB:			InvokeKey(0, tb::TB_KEY_TAB, modifier, press); break;
	case SDL_SCANCODE_DELETE:		InvokeKey(0, tb::TB_KEY_DELETE, modifier, press); break;
	case SDL_SCANCODE_BACKSPACE:	InvokeKey(0, tb::TB_KEY_BACKSPACE, modifier, press); break;
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_KP_ENTER:		InvokeKey(0, tb::TB_KEY_ENTER, modifier, press); break;
	case SDL_SCANCODE_ESCAPE:		InvokeKey(0, tb::TB_KEY_ESC, modifier, press); break;
	case SDL_SCANCODE_MENU:
		if (tb::TBWidget::focused_widget && !press)
		{
			tb::TBWidgetEvent ev(tb::EVENT_TYPE_CONTEXT_MENU);
			ev.modifierkeys = modifier;
			tb::TBWidget::focused_widget->InvokeEvent(ev);
		}
		break;

	default:
		if ((modifier & tb::TB_CTRL) != 0)
		{
			InvokeKey(evt.key.keysym.sym, tb::TB_KEY_UNDEFINED, modifier, press);
		}
		break;
	}
}

void AppBackendSDL::HandleMouseButtonEvent(const SDL_Event & evt, bool press) {
	tb::MODIFIER_KEYS modifier = GetModifierKeys();
	int x = evt.button.x;
	int y = evt.button.y;
	if (evt.button.button == SDL_BUTTON_LEFT)
	{
		if (press)
		{
			m_app->GetRoot()->InvokePointerDown(x, y, evt.button.clicks, modifier, false);
		}
		else
		{
			m_app->GetRoot()->InvokePointerUp(x, y, modifier, false);
		}
	}
	else if (evt.button.button == SDL_BUTTON_RIGHT && !press)
	{
		m_app->GetRoot()->InvokePointerMove(x, y, modifier, false);
		if (tb::TBWidget::hovered_widget)
		{
			tb::TBWidget::hovered_widget->ConvertFromRoot(x, y);
			tb::TBWidgetEvent ev(tb::EVENT_TYPE_CONTEXT_MENU, x, y, false, modifier);
			tb::TBWidget::hovered_widget->InvokeEvent(ev);
		}
	}
}

void AppBackendSDL::HandleFileDropEvent(const SDL_Event & evt) {
	auto f = evt.drop.file;
	auto file = new tb::TBStr(f);

	tb::TBWidgetEventFileDrop ev;
	ev.files.Add(file);
	m_app->GetRoot()->InvokeEvent(ev);

	SDL_free(f);
}

void AppBackendSDL::Update() {
	if (m_pause) {
		return;
	}

	while (SDL_PollEvent(&m_evt_cache)) {
		if (m_evt_cache.type == SDL_TEXTINPUT) {
			auto mod = GetModifierKeys();
			for (int i = 0; i < 32 && m_evt_cache.text.text[i] != 0; i++) {
				auto c = m_evt_cache.text.text[i];
				InvokeKey(c, tb::TB_KEY_UNDEFINED, mod, true);
				InvokeKey(c, tb::TB_KEY_UNDEFINED, mod, false);
			}

			continue;
		}

		if (m_evt_cache.type == SDL_KEYDOWN) {
			HandleKeyPressEvent(m_evt_cache, true);
			continue;
		}

		if (m_evt_cache.type == SDL_KEYUP) {
			HandleKeyPressEvent(m_evt_cache, false);
			continue;
		}

		if (m_evt_cache.type == SDL_MOUSEMOTION) {
			HandlePointerMoveEvent(m_evt_cache);
			continue;
		}

		if (m_evt_cache.type == SDL_MOUSEBUTTONUP) {
			HandleMouseButtonEvent(m_evt_cache, false);
			continue;
		}

		if (m_evt_cache.type == SDL_MOUSEBUTTONDOWN) {
			HandleMouseButtonEvent(m_evt_cache, true);
			continue;
		}

		if (m_evt_cache.type == SDL_MOUSEWHEEL) {
			int mouse_x, mouse_y;
			SDL_GetMouseState(&mouse_x, &mouse_y);
			m_app->GetRoot()->InvokeWheel(mouse_x, mouse_y, m_evt_cache.wheel.x, -m_evt_cache.wheel.y, GetModifierKeys());

			continue;
		}

		if (m_evt_cache.type == SDL_DROPFILE) {
			HandleFileDropEvent(m_evt_cache);
			continue;
		}

		if (m_evt_cache.type == SDL_WINDOWEVENT) {
			HandleWindowEvent(m_evt_cache);
			continue;
		}
	}


	if (!HasPendingUpdate()) {
		return;
	}

	m_app->Process();

	m_has_pending_update = false;

	// Bail out if we get here with invalid dimensions.
	if (GetWidth() == 0 || GetHeight() == 0)
	{
		return;
	}

	m_app->RenderFrame();

	SDL_GL_SwapWindow(m_window);
}

void AppBackendSDL::OnAppEvent(const EVENT &ev)
{
	switch (ev)
	{
	case EVENT_PAINT_REQUEST:
		if (!m_has_pending_update)
		{
			m_has_pending_update = true;
		}
		break;

	case EVENT_QUIT_REQUEST:
		m_quit_requested = true;
		break;

	case EVENT_TITLE_CHANGED:
		SDL_SetWindowTitle(m_window, m_app->GetTitle());
		break;

	default:
		assert(!"Unhandled app event!");
	}
}