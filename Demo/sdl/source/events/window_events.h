#pragma once

#include "event.h"

namespace Voxer {
	struct WindowResizedEvent {
		int window_id;
		int width;
		int height;
	};
}