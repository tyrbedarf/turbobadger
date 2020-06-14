#pragma once

#include "tb_toggle_container.h"
#include "tb_widgets_listener.h"

namespace tb {
	class TBEditorLayoutParameters : public TBSection {
		TBOBJECT_SUBCLASS(TBEditorLayoutParameters, TBSection);

	public:
		TBEditorLayoutParameters();
		void SetWidgetToEdit(TBLayout* widget) { m_layout = widget; }

		virtual bool OnEvent(const TBWidgetEvent &ev);

	private:
		TBLayout* m_layout; ///< The widget we are editing
	};
}