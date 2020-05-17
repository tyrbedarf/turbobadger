#include "application.h"

#ifdef WIN32
#include <Windows.h>
#include "tb_tempbuffer.h"
#endif


int main(int argc, char *argv[]) {
#ifdef WIN32
	// Set the current path to the directory of the app so we find assets also when visual studio start it.
	char modname[MAX_PATH];
	GetModuleFileName(NULL, modname, MAX_PATH);
	tb::TBTempBuffer buf;
	buf.AppendPath(modname);
	SetCurrentDirectory(buf.GetData());
#endif

	SDL_Demo::Application app(1024, 800);
	if (app.Init()) {
		app.Run();
		app.Shutdown();

		return 0;
	}

	return -1;
}