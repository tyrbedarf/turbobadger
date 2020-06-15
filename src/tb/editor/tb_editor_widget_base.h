#pragma once

#include "tb_widgets.h"

namespace tb {
	/**
		Provides a common interfce for all Editor Widgets
	*/
	template<class T>
	class TBEditorWidgetBase {
	protected:
		T* m_widget; ///< The widget we are editing

		virtual void HideEditor() = 0;
		virtual void ShowEditor() = 0;

		TBEditorWidgetBase() : m_widget(nullptr) { }

	public:
		// Called when the editor window wants to edit another widget.
		// Will set m_widget to nullptr if the given widget is not of the type in question.
		void SetWidget(TBWidget* t) {
			if (!t || !t->IsOfType<T>()) {
				m_widget == nullptr;
				HideEditor();
				return;
			}

			m_widget = (T*) t;
			ShowEditor();
		}
	};
}