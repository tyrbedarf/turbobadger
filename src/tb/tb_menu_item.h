#pragma once

#include "tb_window.h"
#include "tb_scroll_container.h"
#include "tb_select_item.h"
#include "tb_menu_window.h"

namespace tb {
	/**
		TBMenuItem shows a button that opens a popup with a TBSelectList with items
		provided by a TBSelectItemSource. It does not swallo events, so parent widgets can
		react if a menu item was clicked.
	*/
	class TBMenuItem : public TBButton, public TBSelectItemViewer
	{
	public:
		// For safe typecasting
		TBOBJECT_SUBCLASS(TBMenuItem, TBButton);

		TBMenuItem();
		~TBMenuItem();

		/**
			Get the default item source for this widget. This source can be used to add
			items of type TBGenericStringItem to this widget.
			It is the item source that is fed from resource files.

			If you need to add other types of items, or if you want to share item sources
			between several TBSelectDropDown/TBSelectList widgets, use SetSource using a
			external item source.
		*/
		TBGenericStringItemSource *GetDefaultSource() {
			return &m_default_source;
		}

		/** Open the window if the model has items. */
		void OpenWindow();

		/** Close the window if it is open. */
		void CloseWindow();

		/** Return the menu window if it's open, or nullptr. */
		TBMenuWindow *GetMenuIfOpen() const;

		virtual void OnInflate(const INFLATE_INFO &info);
		virtual bool OnEvent(const TBWidgetEvent &ev);

		// == TBSelectItemViewer ==================================================
		virtual void OnSourceChanged();
		virtual void OnItemChanged(int index);
		virtual void OnItemAdded(int index) {}
		virtual void OnItemRemoved(int index) {}
		virtual void OnAllItemsRemoved() {}

	protected:
		TBGenericStringItemSource m_default_source;
		TBWidgetSafePointer m_window_pointer; ///< Points to the dropdown window if opened
	};
}