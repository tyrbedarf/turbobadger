#pragma once

#include "EASTL/string.h"
#include "EASTL/shared_ptr.h"

namespace Voxer {
	using namespace eastl;
	struct FileSavedEvent {
		shared_ptr<string> file_path;
	};
}