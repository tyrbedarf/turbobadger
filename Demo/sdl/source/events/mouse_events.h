#pragma once

#include "event.h"

namespace Voxer {
	struct MouseMoveEvent {
		int x_pos;
		int y_pos;

		int x_rel_pos;
		int y_rel_pos;
	};

	struct MouseButtonEvent {
		enum Button {
			Left,
			Right,
			Middle,
			X1,
			X2
		};

		int x_pos;
		int y_pos;
		bool down; // False if up.
		int clicks; // Number of clicks for double click etc.
		Button button;

		int window_id;
	};

	struct MouseWheelEvent {
		int window_id;
		int x;	// Left and right direction
		int y;	// Up and down direction
	};
}