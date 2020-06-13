#pragma once

#include "tb_widgets.h"
#include "tb_msg.h"
#include "tb_editfield.h"
#include "tb_widgets_common.h"
#include "animation/tb_animation.h"
#include "animation/tb_widget_animation.h"

namespace tb {
	class TBQueuedMessage : public TBLinkOf<TBQueuedMessage> {
	public:
		const char* m_message;
		TBQueuedMessage() : m_message(nullptr) {

		}

		~TBQueuedMessage() {

		}
	};

	class TBStatusbar : public TBWidget, public TBAnimationListener {
		TBOBJECT_SUBCLASS(TBStatusbar, TBWidget);

	public:
		static TBID StatusMessage;

		TBStatusbar();

		// Implement TBAnimationListener
		virtual void OnAnimationStart(TBAnimationObject *obj) { }
		virtual void OnAnimationUpdate(TBAnimationObject *obj, float progress) { }

		virtual void OnAnimationStop(TBAnimationObject *obj, bool aborted);

		virtual void OnInflate(const INFLATE_INFO &info);
		void DisplayMessage(const char* text);

	private:
		double m_anim_duration;

		TBLinkListOf<TBQueuedMessage> m_message_queue;
		TBAnimationObject* m_animation;
		TBTextField* m_output;
		TBQueuedMessage* m_current_message;	// Keep a pointer in order to delete the last message, when animation stops.

		void DisplayMessage(TBQueuedMessage* text);
	};
}