#pragma once

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "tb_scroller.h"
#include "../Application.h"

using namespace tb;

class DemoApplication_02 : public App
{
public:
	DemoApplication_02() : App(1280, 700) {}

	virtual const char *GetTitle() const { return "Demo 02"; }
	virtual void OnBackendAttached(AppBackend *backend, int width, int height);
	virtual bool Init();
	virtual void RenderFrame();
};
