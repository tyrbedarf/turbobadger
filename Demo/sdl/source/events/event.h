#pragma once

namespace Voxer
{
// Define an event for a single object (usually a subsystem). name is the name of the event,
// type is the type of the data that is beeing passed.
#define EVENT(name, event_name) \
public: \
	template<auto METHOD, typename T> \
	void connect_##name(T& receiver) { \
		get_sink<event_name>().connect<METHOD>(receiver); \
	} \
\
	void trigger_##name(event_name evt) { \
		m_event_dispatcher->trigger<event_name>(evt); \
	}\
\
	void queue_##name(event_name evt) {\
		m_event_dispatcher->enqueue<event_name>(evt);\
	}

#define VOID_EVENT(name) \
private: \
	struct name##_event { \
	}; \
	name##_event m_##name##_event; \
\
public: \
	template<auto METHOD, typename T> \
	void connect_##name(T& receiver) { \
		get_sink<##name##_event>().connect<METHOD>(receiver); \
	} \
\
	void trigger_##name() { \
		m_event_dispatcher->trigger<##name##_event>(m_##name##_event); \
	}\
\
	void queue_##name() {\
		m_event_dispatcher->enqueue<##name##_event>(m_##name##_event);\
	}

#define SINGLE_VALUE_EVENT(name, type_name) \
private: \
	struct name##_event { \
		type_name value; \
	}; \
\
public: \
	template<auto METHOD, typename T> \
	void connect_##name(T& receiver) { \
		get_sink<##name##_event>().connect<METHOD>(receiver); \
	} \
\
	void trigger_##name(type_name value) { \
		auto evt = name##_event{ value }; \
		m_event_dispatcher->trigger<##name##_event>(evt); \
	}\
\
	void queue_##name(type_name value) {\
		auto evt = name##_event{ value }; \
		m_event_dispatcher->enqueue<##name##_event>(evt);\
	}
}