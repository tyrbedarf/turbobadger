#pragma once

#include "tb_toggle_container.h"
#include "tb_widgets_listener.h"
#include "tb_editor_widget_base.h"

namespace tb {
	class TBEditorWidgetRect : public TBSection, public TBEditorWidgetBase<TBLayout> {
		TBOBJECT_SUBCLASS(TBEditorWidgetRect, TBSection);

	protected:
		virtual void HideEditor() {
			SetVisibility(WIDGET_VISIBILITY_INVISIBLE);
		}
		virtual void ShowEditor() {
			SetVisibility(WIDGET_VISIBILITY_VISIBLE);
		}

	public:
		TBEditorWidgetRect();

		virtual bool OnEvent(const TBWidgetEvent &ev);
	};
}