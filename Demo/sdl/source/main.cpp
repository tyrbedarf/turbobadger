#include "../Application.h"
#include "AppBackendSDL.h"

#ifdef WIN32
#include <Windows.h>
#include "tb_tempbuffer.h"
#endif

bool port_main() {
	App *app = app_create();

	AppBackendSDL *backend = new AppBackendSDL();
	if (!backend || !backend->Init(app))
		return false;

	bool success = app->Init();
	if (success) {
		// Main loop
		do
		{
			backend->Update();

		} while (!backend->IsQuitRequested());

		app->ShutDown();
	}

	delete backend;

	delete app;

	return success;
}

#ifdef TB_TARGET_WINDOWS

#include <mmsystem.h>
int main(int argc, char** argv)
{
	// Set the current path to the directory of the app so we find assets also when visual studio start it.
	char modname[MAX_PATH];
	GetModuleFileName(NULL, modname, MAX_PATH);
	tb::TBTempBuffer buf;
	buf.AppendPath(modname);
	SetCurrentDirectory(buf.GetData());

	// Crank up windows timer resolution (it's awfully low res normally). Note: This affects battery time!
	timeBeginPeriod(1);
	bool success = port_main();
	timeEndPeriod(1);
	return success ? 0 : 1;
}

#else // TB_TARGET_WINDOWS

int main(int argc, char** argv)
{
	return port_main() ? 0 : 1;
}

#endif // !TB_TARGET_WINDOWS