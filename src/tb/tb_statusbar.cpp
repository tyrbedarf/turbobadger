#include "tb_statusbar.h"

#include "tb_widgets_reader.h"

namespace tb {
	TBID TBStatusbar::StatusMessage = TBID("STATUSMESSAGE");

	TBStatusbar::TBStatusbar() :
		TBWidget(),
		m_output(nullptr),
		m_animation(nullptr),
		m_anim_duration(3500) {
		const char* source =
			"TBLayout: distribution: gravity, axis: x\n"
			"	TBTextField: gravity: left bottom right, distribution-position: left bottom\n"
			"		readonly 1\n"
			"		type text\n"
			"		multiline 0\n"
			"		text-align left\n"
			"		id Statusbar\n"
			"	TBWidget: gravity: left right, distribution-position: right bottom";

		if (!g_widgets_reader->LoadData(GetContentRoot(), source)) {
			return;
		}

		m_output = GetWidgetByIDAndType<TBTextField>(TBID("Statusbar"));
		if (!m_output) {
			return;
		}
	}

	void TBStatusbar::DisplayMessage(const char* text) {
		auto message = new TBQueuedMessage();
		message->m_message = text;

		DisplayMessage(message);
	}

	void TBStatusbar::DisplayMessage(TBQueuedMessage* text) {
		if (!m_output) {
			delete text;	// Simply clean up memory.
			return;
		}

		if (m_animation) {
			m_message_queue.AddLast(text);
			return;
		}

		m_current_message = text;
		m_output->SetText(text->m_message);
		m_animation = new TBWidgetAnimationOpacity(m_output, 1, TB_ALMOST_ZERO_OPACITY, false);
		m_animation->AddListener(this);
		TBAnimationManager::StartAnimation(m_animation, ANIMATION_CURVE_SLOW_DOWN, m_anim_duration);
	}

	void TBStatusbar::OnAnimationStop(TBAnimationObject *obj, bool aborted) {
		auto next = m_message_queue.GetFirst();
		delete m_current_message;	// Delete the last message
		m_animation = nullptr;
		obj->RemoveListener(this);
		if (!next) {
			return;
		}

		m_message_queue.Remove(next);
		DisplayMessage(next);
	}
}