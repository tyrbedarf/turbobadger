#include "tb_window_desktop.h"
#include "tb_widgets_reader.h"

namespace tb {
	TBWindowDesktop *g_window_desktop = nullptr;

	TBWindowDesktop::TBWindowDesktop() {
		// Allow only on desktop.
		// Every other solution would be messy.
		assert(g_window_desktop == nullptr);
		const char* source =
			"TBLayout: distribution:gravity, axis : y\n"
			"	TBWidget: gravity: top bottom, distribution-position : right bottom, id: desktop_display\n"
			"	TBLayout: distribution: gravity, axis: x, id : desktop_minimized\n"
			"		TBWidget: gravity: left right, distribution-position: right bottom, id: minimized_filler\n";

		if (!g_widgets_reader->LoadData(GetContentRoot(), source)) {
			return;
		}

		m_view = GetWidgetByIDAndType<TBWidget>(TBID("desktop_display"));
		m_minimized = GetWidgetByIDAndType<TBLayout>(TBID("desktop_display"));
		m_filler = GetWidgetByIDAndType<TBLayout>(TBID("minimized_filler"));

		if (!m_view || !m_minimized || !m_filler) {
			return;
		}

		g_window_desktop = this;

	}

	void TBWindowDesktop::AddMinimized(TBWindow* w) {
		w->RemoveFromParent();
		m_minimized->AddChildRelative(w, WIDGET_Z_REL_BEFORE, m_filler);

		m_minimized->Invalidate();
		m_view->Invalidate();
	}

	void TBWindowDesktop::RemoveMinimized(TBWindow* w) {
		w->RemoveFromParent();
		m_view->AddChild(w);

		m_minimized->Invalidate();
		m_view->Invalidate();
	}
}