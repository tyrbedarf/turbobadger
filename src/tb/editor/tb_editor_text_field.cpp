#include "tb_editor_text_field.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"

namespace tb {
	TBEditorTextField::TBEditorTextField() :
		TBSection(),
		TBEditorWidgetBase() {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_text_field.tb.txt");

		SetValue(1);
		SetText("Text Field");
	}

	bool TBEditorTextField::OnEvent(const TBWidgetEvent &ev) {
		if (ev.type == EVENT_TYPE_CHANGED) {
			if (!m_widget || !m_widget->IsOfType<TBTextField>()) {
				return false;
			}

			if (!ev.target) {
				return false;
			}

			auto field = (TBTextField*)ev.target;
			if (ev.ref_id == TBID("LEFT")) {
				field->SetTextAlign(TB_TEXT_ALIGN_LEFT);
				return true;
			}
			if (ev.ref_id == TBID("CENTER")) {
				field->SetTextAlign(TB_TEXT_ALIGN_CENTER);
				return true;
			}
			if (ev.ref_id == TBID("RIGHT")) {
				field->SetTextAlign(TB_TEXT_ALIGN_RIGHT);
				return true;
			}

			return false;
		}

		return TBWidget::OnEvent(ev);
	}
}