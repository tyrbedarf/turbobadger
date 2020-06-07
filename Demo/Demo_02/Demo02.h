#pragma once

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "tb_scroller.h"
#include "../Application.h"

#include "tb_window.h"

#include <memory>

using namespace tb;

class DemoWindow : public TBWindow
{
public:
	DemoWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class MainWindow : public TBMessageHandler, public DemoWindow
{
public:
	MainWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);

	// Implement TBMessageHandler
	virtual void OnMessageReceived(TBMessage *msg);
};

class DemoApplication_02 : public App
{
private:
	TBStr m_message;

public:
	DemoApplication_02() : App(1280, 700) {}

	virtual const char *GetTitle() const { return "Demo 02"; }
	virtual void OnBackendAttached(AppBackend *backend, int width, int height);
	virtual bool Init();
	virtual void RenderFrame();
};
