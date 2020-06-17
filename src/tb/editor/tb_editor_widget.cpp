#include "tb_editor_widget.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"

namespace tb {
	TBEditorWidget::TBEditorWidget() :
		TBSection(),
		TBEditorWidgetBase() {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_widget.tb.txt");

		SetValue(1);
		SetText("Widget Parameters");
	}

	bool TBEditorWidget::OnEvent(const TBWidgetEvent &ev) {
		if (ev.type == EVENT_TYPE_CHANGED) {
			if (!m_widget) { return false; }
			if (!ev.target) { return false; }

			if (ev.ref_id == TBID("widget_id") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetID(TBID(tmp->GetText()));
			}

			if (ev.ref_id == TBID("group_id") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetGroupID(TBID(tmp->GetText()));
			}

			if (ev.ref_id == TBID("widget_value") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetValue(tmp->GetValue());
			}

			if (ev.ref_id == TBID("is_group_root") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIsGroupRoot(tmp->GetState(WIDGET_STATE_SELECTED));
			}

			if (ev.ref_id == TBID("is_focusable") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIsFocusable(tmp->GetState(WIDGET_STATE_SELECTED));
			}

			if (ev.ref_id == TBID("want_long_click") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetWantLongClick(tmp->GetState(WIDGET_STATE_SELECTED));
			}

			if (ev.ref_id == TBID("ignore_input") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIgnoreInput(tmp->GetState(WIDGET_STATE_SELECTED));
			}

			if (ev.ref_id == TBID("opacity") && ev.target->IsOfType<TBSlider>()) {
				auto tmp = (TBSlider*)ev.target;
				m_widget->SetOpacity((float) tmp->GetValueDouble());
			}

			if (ev.ref_id == TBID("widget_text") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				m_widget->SetText(tmp->GetText());
			}

			if (ev.ref_id == TBID("widget_conneciton") && ev.target->IsOfType<TBEditField>()) {
				// TODO
				return false;
			}

			if (ev.ref_id == TBID("AXIS_X")) { m_widget->SetAxis(AXIS_X); }
			if (ev.ref_id == TBID("AXIS_Y")) { m_widget->SetAxis(AXIS_Y); }

			return true;
		}

		return TBWidget::OnEvent(ev);
	}
}