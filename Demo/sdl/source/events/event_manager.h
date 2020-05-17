#pragma once

#include "../isubsystem.h"
#include "events.h"

#include <entt/signal/dispatcher.hpp>

namespace Voxer {
	class EventManager : public ISubsystem {
		SUBSYSTEM(EventManager, ISubsystem);
	private:
		shared_ptr<entt::dispatcher> m_event_dispatcher;

		template<typename T> auto get_sink() {
			return m_event_dispatcher->sink<T>();
		}

		/* Application Events */
		VOID_EVENT(application_init);
		VOID_EVENT(application_run);
		VOID_EVENT(application_stop);
		VOID_EVENT(application_quit);

		SINGLE_VALUE_EVENT(update, float);
		SINGLE_VALUE_EVENT(pre_render, float);
		SINGLE_VALUE_EVENT(post_render, float);

		/* window events */
		VOID_EVENT(quit);
		EVENT(window_resized, WindowResizedEvent);
		VOID_EVENT(window_minimized);
		VOID_EVENT(window_maximized);
		VOID_EVENT(window_restored);
		SINGLE_VALUE_EVENT(window_close, int);

		/* mouse events */
		VOID_EVENT(mouse_enter);
		VOID_EVENT(mouse_leave);
		EVENT(mouse_button, MouseButtonEvent);
		EVENT(mouse_move, MouseMoveEvent);
		EVENT(mouse_wheel, MouseWheelEvent);

		/* Key events */
		EVENT(key, KeyEvent);
		EVENT(file_saved, FileSavedEvent);

		virtual void create(VoxerApplication* app) override {
			m_event_dispatcher = make_shared<entt::dispatcher>();
		}

		virtual void initialize(VoxerApplication* app) override { }
		virtual void shutdown() override { }
		virtual void quit() override { }
		virtual Priority priority() override { return Priority::EventManager; }

		void update() {
			m_event_dispatcher->update();
		}

	};
}