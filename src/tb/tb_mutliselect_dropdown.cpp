#include "tb_mutliselect_dropdown.h"

#include "tb_widgets_listener.h"
#include "tb_language.h"
#include "tb_tempbuffer.h"
#include "tb_sort.h"
#include "tb_widgets_reader.h"

namespace tb {
	TBMultiselectDropdown::TBMultiselectDropdown()
	{
		SetSource(&m_default_source);
		SetSkinBg(TBIDC("TBMenuItem"), WIDGET_INVOKE_INFO_NO_CALLBACKS);
		m_auto_close = true;
	}

	TBMultiselectDropdown::~TBMultiselectDropdown()
	{
		SetSource(nullptr);
		CloseWindow();
	}

	void TBMultiselectDropdown::OnSourceChanged()
	{

	}

	void TBMultiselectDropdown::OnItemChanged(int index)
	{

	}

	void TBMultiselectDropdown::OpenWindow()
	{
		if (!m_source || !m_source->GetNumItems() || m_window_pointer.Get())
			return;

		// TODO: Loading the string representation everytime the windows is opened, seems wasteful.
		const char* layout =
			"TBScrollContainer: gravity: all, adapt-content: 1, scroll-mode: auto\n"
			"	TBLayout: id: layout, axis: y, position: left top\n";

		if (TBPopupWindow *window = new TBPopupWindow(this))
		{
			m_window_pointer.Set(window);
			g_widgets_reader->LoadData(window, layout);
			auto layout = window->GetWidgetByIDAndType<TBLayout>(TBID("layout"));
			if (layout) {
				for (int i = 0; i < m_default_source.GetNumItems(); i++) {
					auto l = new TBLayout();
					l->SetLayoutDistribution(LAYOUT_DISTRIBUTION_GRAVITY);

					auto cb = new TBCheckBox();
					cb->SetID(m_default_source.GetItemID(i));
					cb->SetValue(m_default_source.GetItem(i)->tag.GetInt()); // Store changes in the tag field.
					l->AddChild(cb);

					auto text = new TBTextField();
					text->SetText(m_default_source.GetItemString(i));
					l->AddChild(text);

					layout->GetContentRoot()->AddChild(l);
				}
			}

			window->ResizeToFitContent();
			window->Show(TBPopupAlignment());
		}
	}

	void TBMultiselectDropdown::CloseWindow()
	{
		if (TBPopupWindow *window = GetMenuIfOpen())
			window->Close();
	}

	TBPopupWindow *TBMultiselectDropdown::GetMenuIfOpen() const
	{
		return TBSafeCast<TBPopupWindow>(m_window_pointer.Get());
	}

	bool TBMultiselectDropdown::OnEvent(const TBWidgetEvent &ev)
	{
		if (ev.type == EVENT_TYPE_CLICK && ev.target && ev.target->IsOfType<TBCheckBox>()) {
			// Did we get receive the id of one of our items?
			auto cb = (TBCheckBox*) ev.target;
			for (int i = 0; i < m_default_source.GetNumItems(); i++) {
				if (cb->GetID() == m_default_source.GetItemID(i)) {
					auto old = m_default_source.GetItem(i)->tag.GetInt();
					m_default_source.GetItem(i)->tag.SetInt(cb->GetValue());
					if (old != m_default_source.GetItem(i)->tag.GetInt()) {
						TBWidgetEvent e(EVENT_TYPE_CHANGED);
						e.target = this;

						InvokeEvent(e);
					}

					if (m_auto_close)
					{
						if (TBPopupWindow *window = GetMenuIfOpen())
						{
							window->Close();
						}
					}

					return true;
				}
			}

			return false;
		}

		if (ev.target == this && ev.type == EVENT_TYPE_CLICK)
		{
			// Open the menu, or set the value and close it if already open (this will
			// happen when clicking by keyboard since that will call click on this button)
			if (TBPopupWindow *menu_window = GetMenuIfOpen())
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

		return false;
	}
}