#pragma once

#include "tb_toggle_container.h"
#include "tb_widgets_listener.h"
#include "tb_editor_widget_base.h"

namespace tb {
	class TBEditorWidget : public TBSection, public TBEditorWidgetBase<TBWidget> {
	TBOBJECT_SUBCLASS(TBEditorWidget, TBSection);

	protected:
		virtual void HideEditor() {
			SetVisibility(WIDGET_VISIBILITY_INVISIBLE);
		}
		virtual void ShowEditor() {
			SetVisibility(WIDGET_VISIBILITY_VISIBLE);
		}

	public:
		TBEditorWidget();

		virtual bool OnEvent(const TBWidgetEvent &ev);
	};
}