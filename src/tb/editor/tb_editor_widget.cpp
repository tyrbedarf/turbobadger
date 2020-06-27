#include "tb_editor_widget.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"
#include "tb_mutliselect_dropdown.h"

namespace tb {
	TBEditorWidget::TBEditorWidget() :
		TBSection(),
		TBEditorWidgetBase() {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_widget.tb.txt");

		SetValue(1);
		SetText("Widget");
	}

	bool TBEditorWidget::OnEvent(const TBWidgetEvent &ev) {
		if (ev.type == EVENT_TYPE_CHANGED) {
			if (!m_widget) { return false; }
			if (!ev.target) { return false; }

			if (ev.ref_id == TBID("widget_id") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetID(TBID(tmp->GetText()));

				return true;
			}

			if (ev.ref_id == TBID("group_id") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetGroupID(TBID(tmp->GetText()));

				return true;
			}

			if (ev.ref_id == TBID("widget_value") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_widget->SetValue(tmp->GetValue());

				return true;
			}

			if (ev.ref_id == TBID("is_group_root") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIsGroupRoot(tmp->GetState(WIDGET_STATE_SELECTED));

				return true;
			}

			if (ev.ref_id == TBID("is_focusable") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIsFocusable(tmp->GetState(WIDGET_STATE_SELECTED));

				return true;
			}

			if (ev.ref_id == TBID("want_long_click") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetWantLongClick(tmp->GetState(WIDGET_STATE_SELECTED));

				return true;
			}

			if (ev.ref_id == TBID("ignore_input") && ev.target->IsOfType<TBCheckBox>()) {
				auto tmp = (TBCheckBox*)ev.target;
				m_widget->SetIgnoreInput(tmp->GetState(WIDGET_STATE_SELECTED));

				return true;
			}

			if (ev.ref_id == TBID("opacity") && ev.target->IsOfType<TBSlider>()) {
				auto tmp = (TBSlider*)ev.target;
				m_widget->SetOpacity((float) tmp->GetValueDouble());

				return true;
			}

			if (ev.ref_id == TBID("widget_text") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*)ev.target;
				m_widget->SetText(tmp->GetText());

				return true;
			}

			if (ev.ref_id == TBID("widget_conneciton") && ev.target->IsOfType<TBEditField>()) {
				// TODO
				return false;
			}

			if (ev.ref_id == TBID("AXIS_X")) { m_widget->SetAxis(AXIS_X); return true; }
			if (ev.ref_id == TBID("AXIS_Y")) { m_widget->SetAxis(AXIS_Y); return true; }

			if (ev.ref_id == TBID("widget_gravity") && ev.target->IsOfType<TBMultiselectDropdown>())
			{
				WIDGET_GRAVITY g = WIDGET_GRAVITY_NONE;
				auto dd = (TBMultiselectDropdown*) ev.target;
				for (int i = 0; i < dd->GetNumItems(); i++) {
					auto item = dd->GetItem(i);
					g |= item->id == TBID("TOP") ? WIDGET_GRAVITY_TOP : WIDGET_GRAVITY_NONE;
					g |= item->id == TBID("LEFT") ? WIDGET_GRAVITY_LEFT : WIDGET_GRAVITY_NONE;
					g |= item->id == TBID("BOTTOM") ? WIDGET_GRAVITY_BOTTOM : WIDGET_GRAVITY_NONE;
					g |= item->id == TBID("RIGHT") ? WIDGET_GRAVITY_RIGHT : WIDGET_GRAVITY_NONE;
				}

				m_widget->SetGravity(g);

				return true;
			}

			if (ev.ref_id == TBID("VISIBLE")) { m_widget->SetVisibility(WIDGET_VISIBILITY_VISIBLE); }
			if (ev.ref_id == TBID("INVISIBLE")) { m_widget->SetVisibility(WIDGET_VISIBILITY_INVISIBLE); }
			if (ev.ref_id == TBID("GONE")) { m_widget->SetVisibility(WIDGET_VISIBILITY_GONE); }

			if (ev.ref_id == TBID("widget_skin") && ev.target->IsOfType<TBEditField>()) {
				auto dd = (TBEditField*) ev.target;
				m_widget->SetSkinBg(TBID(dd->GetText()));

				return true;
			}

			if (ev.ref_id == TBID("widget_auto_focus") && ev.target->IsOfType<TBCheckBox>()) {
				auto dd = (TBCheckBox*)ev.target;
				m_widget->SetAutoFocusState(dd->GetValue() ? true : false);

				return true;
			}

			if (ev.ref_id == TBID("widget_font_name") && ev.target->IsOfType<TBEditField>()) {
				return false;
			}

			if (ev.ref_id == TBID("widget_font_size") && ev.target->IsOfType<TBEditField>()) {
				return false;
			}

			return false;
		}

		return TBWidget::OnEvent(ev);
	}
}