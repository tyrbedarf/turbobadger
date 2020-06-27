#include "tb_editor_layout_parameter.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"

namespace tb {
	TBEditorLayoutParameter::TBEditorLayoutParameter() :
		TBSection(),
		TBEditorWidgetBase() {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_layout_parameter.tb.txt");

		SetValue(1);
		SetText("Layout Parameter");
	}

	bool TBEditorLayoutParameter::OnEvent(const TBWidgetEvent &ev) {
		if (ev.type == EVENT_TYPE_CHANGED) {
			if (!m_widget) {
				return false;
			}
			if (!ev.target) {
				return false;
			}

			// Width
			if (ev.ref_id == TBID("widget_lp_width") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.SetWidth(tmp->GetValue());
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_min_width") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.min_w = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_max_width") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.max_w = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_pref_width") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.pref_w = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}

			// Height
			if (ev.ref_id == TBID("widget_lp_height") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.SetHeight(tmp->GetValue());
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_min_height") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.min_h = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_max_height") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.max_h = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}

			if (ev.ref_id == TBID("widget_lp_pref_height") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				auto rect = *m_widget->GetLayoutParams();
				rect.pref_h = tmp->GetValue();
				m_widget->SetLayoutParams(rect);

				return true;
			}
			return false;
		}

		return TBWidget::OnEvent(ev);
	}
}