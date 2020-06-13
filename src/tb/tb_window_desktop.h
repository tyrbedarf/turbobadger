#pragma once

#include "tb_window.h"

namespace tb
{
	class TBWindowDesktop : public TBWidget {
		TBOBJECT_SUBCLASS(TBWindowDesktop, TBWidget);

	public:
		TBWindowDesktop();
		void AddMinimized(TBWindow* w);
		void RemoveMinimized(TBWindow* w);

	private:
		TBWidget* m_view;
		TBLayout* m_minimized;
		TBWidget* m_filler;
	};

	extern TBWindowDesktop *g_window_desktop;
}