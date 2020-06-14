#include "tb_editor_layout_parameters.h"

#include "tb_widgets_reader.h"
#include "tb_editfield.h"

namespace tb {
	TBEditorLayoutParameters::TBEditorLayoutParameters() :
		TBSection(),
		m_layout(nullptr) {
		g_widgets_reader->LoadFile(this, "resources/editor_layout/tb_editor_layout_parameters.tb.txt");

		SetValue(1);
		SetText("Layout Parameters");
	}

	bool TBEditorLayoutParameters::OnEvent(const TBWidgetEvent &ev) {
		if(ev.type == EVENT_TYPE_CHANGED) {
			if (!m_layout) { return false; }

			if (ev.ref_id == TBID("AXIS_X")) { m_layout->SetAxis(AXIS_X); }
			if (ev.ref_id == TBID("AXIS_Y")) { m_layout->SetAxis(AXIS_Y); }

			if (ev.ref_id == TBID("LAYOUT_SIZE_GRAVITY")) { m_layout->SetLayoutSize(LAYOUT_SIZE_GRAVITY); }
			if (ev.ref_id == TBID("LAYOUT_SIZE_PREFERRED")) { m_layout->SetLayoutSize(LAYOUT_SIZE_PREFERRED); }
			if (ev.ref_id == TBID("LAYOUT_SIZE_AVAILABLE")) { m_layout->SetLayoutSize(LAYOUT_SIZE_AVAILABLE); }

			if (ev.ref_id == TBID("LAYOUT_POSITION_CENTER")) { m_layout->SetLayoutPosition(LAYOUT_POSITION_CENTER); }
			if (ev.ref_id == TBID("LAYOUT_POSITION_LEFT_TOP")) { m_layout->SetLayoutPosition(LAYOUT_POSITION_LEFT_TOP); }
			if (ev.ref_id == TBID("LAYOUT_POSITION_RIGHT_BOTTOM")) { m_layout->SetLayoutPosition(LAYOUT_POSITION_RIGHT_BOTTOM); }
			if (ev.ref_id == TBID("LAYOUT_POSITION_GRAVITY")) { m_layout->SetLayoutPosition(LAYOUT_POSITION_GRAVITY); }

			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_PREFERRED")) { m_layout->SetLayoutDistribution(LAYOUT_DISTRIBUTION_PREFERRED); }
			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_AVAILABLE")) { m_layout->SetLayoutDistribution(LAYOUT_DISTRIBUTION_AVAILABLE); }
			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_GRAVITY")) { m_layout->SetLayoutDistribution(LAYOUT_DISTRIBUTION_GRAVITY); }

			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_POSITION_CENTER")) { m_layout->SetLayoutDistributionPosition(LAYOUT_DISTRIBUTION_POSITION_CENTER); }
			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP")) { m_layout->SetLayoutDistributionPosition(LAYOUT_DISTRIBUTION_POSITION_LEFT_TOP); }
			if (ev.ref_id == TBID("LAYOUT_DISTRIBUTION_POSITION_RIGHT_BOTTOM")) { m_layout->SetLayoutDistributionPosition(LAYOUT_DISTRIBUTION_POSITION_RIGHT_BOTTOM); }

			if (ev.ref_id == TBID("LAYOUT_ORDER_BOTTOM_TO_TOP")) { m_layout->SetLayoutOrder(LAYOUT_ORDER_BOTTOM_TO_TOP); }
			if (ev.ref_id == TBID("LAYOUT_ORDER_TOP_TO_BOTTOM")) { m_layout->SetLayoutOrder(LAYOUT_ORDER_TOP_TO_BOTTOM); }

			if (ev.ref_id == TBID("LAYOUT_OVERFLOW_CLIP")) { m_layout->SetLayoutOverflow(LAYOUT_OVERFLOW_CLIP); }
			if (ev.ref_id == TBID("LAYOUT_OVERFLOW_SCROLL")) { m_layout->SetLayoutOverflow(LAYOUT_OVERFLOW_SCROLL); }

			if (ev.ref_id == TBID("layout_spacing") && ev.target->IsOfType<TBEditField>()) {
				auto tmp = (TBEditField*) ev.target;
				m_layout->SetSpacing(tmp->GetValue());
			}

			return true;
		}

		return TBWidget::OnEvent(ev);
	}
}