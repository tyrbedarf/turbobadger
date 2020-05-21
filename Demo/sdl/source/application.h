#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "tb_scroller.h"
#include "renderers/tb_renderer_gl.h"

namespace SDL_Demo {
	class Application;

	/** The root of widgets in a platform backend. */
	using namespace tb;
	class SdlAppRootWidget : public TBWidget
	{
	public:
		// For safe typecasting
		TBOBJECT_SUBCLASS(SdlAppRootWidget, TBWidget);

		SdlAppRootWidget(Application *app) : m_app(app) {}
		virtual void OnInvalid() {

		}

		Application *GetApp() {
			return m_app;
		}
	private:
		Application *m_app;
	};

	class Application {
	private:
		int m_widht;
		int m_height;

		SDL_Window* m_window;
		SDL_GLContext m_gl_context;

		SDL_Event m_evt_cache;

		tb::TBRendererGL* m_renderer;

		bool m_running;

		bool init_gl();

		void on_application_quit() {
			m_running = false;
		}

	public:
		Application(int width, int height) :
			m_widht(width),
			m_height(height),
			m_window(nullptr),
			m_running(false),
			m_renderer(false) {

		}

		bool Init();
		void Run();
		void Shutdown();
	};
}