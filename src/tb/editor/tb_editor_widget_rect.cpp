#include "tb_editor_widget_rect.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"

namespace tb {
	TBEditorWidgetRect::TBEditorWidgetRect() :
		TBSection(),
		TBEditorWidgetBase() {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_widget_rect.tb.txt");

		SetValue(1);
		SetText("Widget Rect");
	}

	bool TBEditorWidgetRect::OnEvent(const TBWidgetEvent &ev) {
		if (ev.type == EVENT_TYPE_CHANGED) {
			if (!m_widget) {
				return false;
			}
			if (!ev.target) {
				return false;
			}

			if (ev.ref_id == TBID("widget_rect_x") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = m_widget->GetRect();
				rect.x = tmp->GetValue();
				m_widget->SetRect(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_rect_y") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = m_widget->GetRect();
				rect.y = tmp->GetValue();
				m_widget->SetRect(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_rect_width") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = m_widget->GetRect();
				rect.w = tmp->GetValue();
				m_widget->SetRect(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_rect_height") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = m_widget->GetRect();
				rect.h = tmp->GetValue();
				m_widget->SetRect(rect);

				return true;
			}

			return false;
		}

		return TBWidget::OnEvent(ev);
	}
}