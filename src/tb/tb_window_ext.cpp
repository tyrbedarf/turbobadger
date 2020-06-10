#include "tb_window_ext.h"

#include "tb_widgets_listener.h"
#include "tb_language.h"
#include "tb_tempbuffer.h"
#include "tb_sort.h"

namespace tb {
	TBMenuItem::TBMenuItem()
		: m_value(-1)
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
		m_value = -1;
		if (m_source && m_source->GetNumItems())
			SetValue(0);
	}

	void TBMenuItem::OnItemChanged(int index)
	{

	}

	void TBMenuItem::SetValue(int value)
	{
		if (value == m_value || !m_source)
			return;

		m_value = value;

		TBWidgetEvent ev(EVENT_TYPE_CHANGED);
		InvokeEvent(ev);
	}

	TBID TBMenuItem::GetSelectedItemID()
	{
		if (m_source && m_value >= 0 && m_value < m_source->GetNumItems())
			return m_source->GetItemID(m_value);

		return TBID();
	}

	void TBMenuItem::OpenWindow()
	{
		if (!m_source || !m_source->GetNumItems() || m_window_pointer.Get())
			return;

		if (TBMenuWindow *window = new TBMenuWindow(this, TBIDC("TBMenuItem.window")))
		{
			m_window_pointer.Set(window);
			window->SetSkinBg(TBIDC("TBMenuItem.window"));
			window->Show(m_source, TBPopupAlignment(), GetValue());
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
				TBWidgetSafePointer tmp(this);
				int value = menu_window->GetList()->GetValue();
				menu_window->Die();
				if (tmp.Get())
					SetValue(value);
			}
			else
				OpenWindow();

			return true;
		}
		else if (ev.target->GetID() == TBIDC("TBMenuItem.window") && ev.type == EVENT_TYPE_CLICK)
		{
			// Set the value of the clicked item
			if (TBMenuWindow *menu_window = GetMenuIfOpen())
				SetValue(menu_window->GetList()->GetValue());

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