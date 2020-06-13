// ================================================================================
// ==      This file is a part of Turbo Badger. (C) 2011-2014, Emil Segerås      ==
// ==                     See tb_core.h for more information.                    ==
// ================================================================================
#include "tb_window.h"
#include <assert.h>
#include "tb_window_desktop.h"

namespace tb {

// == TBWindow ==========================================================================

TBWindow::TBWindow()
	: m_settings(WINDOW_SETTINGS_DEFAULT),
	  m_minimized(false)
{
	SetSkinBg(TBIDC("TBWindow"), WIDGET_INVOKE_INFO_NO_CALLBACKS);
	AddChild(&m_mover);
	AddChild(&m_resizer);
	m_mover.SetSkinBg(TBIDC("TBWindow.mover"));
	m_mover.AddChild(&m_textfield);
	m_textfield.SetIgnoreInput(true);
	m_mover.AddChild(&m_minimize_button);
	m_mover.AddChild(&m_close_button);

	m_close_button.SetSkinBg(TBIDC("TBWindow.close"));
	m_close_button.SetIsFocusable(false);
	m_close_button.SetID(TBIDC("TBWindow.close"));

	m_minimize_button.SetSkinBg(TBIDC("TBWindow.minimize"));
	m_minimize_button.SetIsFocusable(false);
	m_minimize_button.SetID(TBIDC("TBWindow.minimize"));

	m_textfield.SetTextAlign(TB_TEXT_ALIGN_LEFT);

	SetIsGroupRoot(true);

	if (g_window_desktop == nullptr) {
		m_settings |= ~WINDOW_SETTINGS_MINIMIZE;
		SetSettings(m_settings);
	}
}

TBWindow::~TBWindow()
{
	m_resizer.RemoveFromParent();
	m_mover.RemoveFromParent();
	m_close_button.RemoveFromParent();
	m_textfield.RemoveFromParent();
	m_minimize_button.RemoveFromParent();
}

TBRect TBWindow::GetResizeToFitContentRect(RESIZE_FIT fit)
{
	PreferredSize ps = GetPreferredSize();
	int new_w = ps.pref_w;
	int new_h = ps.pref_h;
	if (fit == RESIZE_FIT_MINIMAL)
	{
		new_w = ps.min_w;
		new_h = ps.min_h;
	}
	else if (fit == RESIZE_FIT_CURRENT_OR_NEEDED)
	{
		new_w = Clamp(GetRect().w, ps.min_w, ps.max_w);
		new_h = Clamp(GetRect().h, ps.min_h, ps.max_h);
	}
	if (GetParent())
	{
		new_w = Min(new_w, GetParent()->GetRect().w);
		new_h = Min(new_h, GetParent()->GetRect().h);
	}
	return TBRect(GetRect().x, GetRect().y, new_w, new_h);
}

void TBWindow::ResizeToFitContent(RESIZE_FIT fit)
{
	SetRect(GetResizeToFitContentRect(fit));
}

void TBWindow::Close()
{
	Die();
}

bool TBWindow::IsActive() const
{
	return GetState(WIDGET_STATE_SELECTED);
}

TBWindow *TBWindow::GetTopMostOtherWindow(bool only_activable_windows)
{
	TBWindow *other_window = nullptr;
	TBWidget *sibling = GetParent()->GetLastChild();
	while (sibling && !other_window)
	{
		if (sibling != this)
			other_window = TBSafeCast<TBWindow>(sibling);

		if (only_activable_windows && other_window && !(other_window->m_settings & WINDOW_SETTINGS_CAN_ACTIVATE))
			other_window = nullptr;

		sibling = sibling->GetPrev();
	}
	return other_window;
}

void TBWindow::Activate()
{
	if (!GetParent() || !(m_settings & WINDOW_SETTINGS_CAN_ACTIVATE))
		return;
	if (IsActive())
	{
		// Already active, but we may still have lost focus,
		// so ensure it comes back to us.
		EnsureFocus();
		return;
	}

	// Deactivate currently active window
	TBWindow *active_window = GetTopMostOtherWindow(true);
	if (active_window)
		active_window->DeActivate();

	// Activate this window

	SetZ(WIDGET_Z_TOP);
	SetWindowActiveState(true);
	EnsureFocus();
}

bool TBWindow::EnsureFocus()
{
	// If we already have focus, we're done.
	if (focused_widget && IsAncestorOf(focused_widget))
		return true;

	// Focus last focused widget (if we have one)
	bool success = false;
	if (m_last_focus.Get())
		success = m_last_focus.Get()->SetFocus(WIDGET_FOCUS_REASON_UNKNOWN);
	// We didn't have one or failed, so try focus any child.
	if (!success)
		success = SetFocusRecursive(WIDGET_FOCUS_REASON_UNKNOWN);
	return success;
}

void TBWindow::DeActivate()
{
	if (!IsActive())
		return;
	SetWindowActiveState(false);
}

void TBWindow::SetWindowActiveState(bool active)
{
	SetState(WIDGET_STATE_SELECTED, active);
	m_mover.SetState(WIDGET_STATE_SELECTED, active);
}

void TBWindow::SetSettings(WINDOW_SETTINGS settings)
{
	if (settings == m_settings)
		return;
	m_settings = settings;

	if (settings & WINDOW_SETTINGS_TITLEBAR)
	{
		if (!m_mover.GetParent())
			AddChild(&m_mover);
	}
	else if (!(settings & WINDOW_SETTINGS_TITLEBAR))
	{
		m_mover.RemoveFromParent();
	}

	if (settings & WINDOW_SETTINGS_RESIZABLE)
	{
		if (!m_resizer.GetParent())
			AddChild(&m_resizer);
	}
	else if (!(settings & WINDOW_SETTINGS_RESIZABLE))
	{
		m_resizer.RemoveFromParent();
	}

	if (settings & WINDOW_SETTINGS_CLOSE_BUTTON)
	{
		if (!m_close_button.GetParent())
			m_mover.AddChild(&m_close_button);
	}
	else if (!(settings & WINDOW_SETTINGS_CLOSE_BUTTON))
	{
		m_close_button.RemoveFromParent();
	}

	if (settings & WINDOW_SETTINGS_MINIMIZE && g_window_desktop)
	{
		if (!m_minimize_button.GetParent())
			m_mover.AddChild(&m_minimize_button);
	}
	else if (!(settings & WINDOW_SETTINGS_MINIMIZE))
	{
		m_minimize_button.RemoveFromParent();
	}

	// FIX: invalidate layout / resize window!
	Invalidate();
}

int TBWindow::GetTitleHeight()
{
	if (m_settings & WINDOW_SETTINGS_TITLEBAR)
		return m_mover.GetPreferredSize().pref_h;
	return 0;
}

TBRect TBWindow::GetPaddingRect()
{
	TBRect padding_rect = TBWidget::GetPaddingRect();
	int title_height = GetTitleHeight();
	padding_rect.y += title_height;
	padding_rect.h -= title_height;
	return padding_rect;
}

PreferredSize TBWindow::OnCalculatePreferredSize(const SizeConstraints &constraints)
{
	PreferredSize ps = OnCalculatePreferredContentSize(constraints);

	// Add window skin padding
	if (TBSkinElement *e = GetSkinBgElement())
	{
		ps.min_w += e->padding_left + e->padding_right;
		ps.pref_w += e->padding_left + e->padding_right;
		ps.min_h += e->padding_top + e->padding_bottom;
		ps.pref_h += e->padding_top + e->padding_bottom;
	}
	// Add window title bar height
	int title_height = GetTitleHeight();
	ps.min_h += title_height;
	ps.pref_h += title_height;
	return ps;
}

bool TBWindow::OnEvent(const TBWidgetEvent &ev)
{
	if (ev.target == &m_close_button)
	{
		if (ev.type == EVENT_TYPE_CLICK)
			Close();
		return true;
	}

	if (ev.target == &m_minimize_button && g_window_desktop != nullptr)
	{
		if (ev.type == EVENT_TYPE_CLICK && m_minimized)
		{
			m_minimized = false;
			SetRect(m_saved_rect);

			m_close_button.SetVisibility(WIDGET_VISIBILITY_VISIBLE);
			m_resizer.SetVisibility(WIDGET_VISIBILITY_VISIBLE);

			g_window_desktop->RemoveMinimized(this);

			return true;
		}

		if (ev.type == EVENT_TYPE_CLICK && !m_minimized)
		{
			m_minimized = true;

			m_saved_rect = GetRect();
			auto height = GetTitleHeight();
			auto width = 250;

			m_close_button.SetVisibility(WIDGET_VISIBILITY_INVISIBLE);
			m_resizer.SetVisibility(WIDGET_VISIBILITY_INVISIBLE);

			SetRect(TBRect(m_saved_rect.x, m_saved_rect.y, width, height));

			g_window_desktop->AddMinimized(this);

			return true;
		}

		return true;
	}

	return false;
}

void TBWindow::OnAdded()
{
	if (GetParent()->IsOfType<TBWindowDesktop>()) {
		m_settings |= WINDOW_SETTINGS_MINIMIZE;
		m_minimize_button.SetVisibility(WIDGET_VISIBILITY_VISIBLE);
		SetSettings(m_settings);
	}
	else
	{
		m_settings |= ~WINDOW_SETTINGS_MINIMIZE;
		m_minimize_button.SetVisibility(WIDGET_VISIBILITY_INVISIBLE);
		SetSettings(m_settings);
	}

	// If we was added last, call Activate to update status etc.
	if (GetParent()->GetLastChild() == this)
		Activate();
}

void TBWindow::OnRemove()
{
	DeActivate();

	// Active the top most other window
	if (TBWindow *active_window = GetTopMostOtherWindow(true))
		active_window->Activate();
}

void TBWindow::OnChildAdded(TBWidget *child)
{
	m_resizer.SetZ(WIDGET_Z_TOP);
}

void TBWindow::OnResized(int old_w, int old_h)
{
	// Apply gravity on children
	TBWidget::OnResized(old_w, old_h);
	// Manually move our own decoration children
	// FIX: Put a layout in the TBMover so we can add things there nicely.
	const int title_height = GetTitleHeight();
	m_mover.SetRect(TBRect(0, 0, GetRect().w, title_height));

	PreferredSize ps = m_resizer.GetPreferredSize();
	m_resizer.SetRect(TBRect(GetRect().w - ps.pref_w, GetRect().h - ps.pref_h, ps.pref_w, ps.pref_h));

	const TBRect mover_rect = m_mover.GetRect();
	const TBRect mover_padding_rect = m_mover.GetPaddingRect();
	const int mover_padding_right = mover_rect.x + mover_rect.w - (mover_padding_rect.x + mover_padding_rect.w);
	const int button_w = m_close_button.GetPreferredSize().pref_w;
	const int button_h = Max(m_close_button.GetPreferredSize().pref_h, mover_padding_rect.h);

	if (!m_minimized)
	{
		m_close_button.SetRect(TBRect(mover_padding_rect.x + mover_padding_rect.w - button_w, mover_padding_rect.y, button_w, button_h));

		// Position relative to the close button.
		m_minimize_button.SetRect(TBRect(mover_padding_rect.x + mover_padding_rect.w - button_w - button_w - 5, mover_padding_rect.y, button_w, button_h));
	}
	else
	{
		m_minimize_button.SetRect(TBRect(mover_padding_rect.x + mover_padding_rect.w - button_w, mover_padding_rect.y, button_w, button_h));
	}


	TBRect title_rect = mover_padding_rect;
	if (m_settings & WINDOW_SETTINGS_CLOSE_BUTTON)
		title_rect.w -= mover_padding_right + button_w;
	m_textfield.SetRect(title_rect);
}

} // namespace tb
