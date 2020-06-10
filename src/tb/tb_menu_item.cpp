#include "tb_menu_item.h"

#include "tb_widgets_listener.h"
#include "tb_language.h"
#include "tb_tempbuffer.h"
#include "tb_sort.h"

namespace tb {
	TBMenuItem::TBMenuItem()
	{
		SetSource(&m_default_source);
		SetSkinBg(TBIDC("TBMenuItem"), WIDGET_INVOKE_INFO_NO_CALLBACKS);
	}

	TBMenuItem::~TBMenuItem()
	{
		SetSource(nullptr);
		CloseWindow();
	}

	void TBMenuItem::OnSourceChanged()
	{

	}

	void TBMenuItem::OnItemChanged(int index)
	{

	}

	void TBMenuItem::OpenWindow()
	{
		if (!m_source || !m_source->GetNumItems() || m_window_pointer.Get())
			return;

		if (TBMenuWindow *window = new TBMenuWindow(this, TBIDC("TBMenuItem.window")))
		{
			m_window_pointer.Set(window);
			window->SetSkinBg(TBIDC("TBMenuItem.window"));
			window->Show(m_source, TBPopupAlignment());
		}
	}

	void TBMenuItem::CloseWindow()
	{
		if (TBMenuWindow *window = GetMenuIfOpen())
			window->Close();
	}

	TBMenuWindow *TBMenuItem::GetMenuIfOpen() const
	{
		return TBSafeCast<TBMenuWindow>(m_window_pointer.Get());
	}

	bool TBMenuItem::OnEvent(const TBWidgetEvent &ev)
	{
		if (ev.target == this && ev.type == EVENT_TYPE_CLICK)
		{
			// Open the menu, or set the value and close it if already open (this will
			// happen when clicking by keyboard since that will call click on this button)
			if (TBMenuWindow *menu_window = GetMenuIfOpen())
			{
				menu_window->Die();
			}
			else
			{
				OpenWindow();
			}

			return true;
		}
		else if (ev.target->GetID() == TBIDC("TBMenuItem.window") && ev.type == EVENT_TYPE_CLICK)
		{
			return false;
		}
		else if (ev.target == this && m_source && ev.IsKeyEvent())
		{
			if (TBMenuWindow *menu_window = GetMenuIfOpen())
			{
				// Redirect the key strokes to the list
				TBWidgetEvent redirected_ev(ev);
				menu_window->GetList()->InvokeEvent(redirected_ev);

				return false;
			}
		}
		return false;
	}
}