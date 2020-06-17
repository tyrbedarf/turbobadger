#pragma once

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "animation/tb_widget_animation.h"
#include "tb_window_desktop.h"
#include "tb_scroller.h"
#include "../Application.h"
#include "tb_statusbar.h"

#include "tb_window.h"

#include <memory>

using namespace tb;

class ApplicationWindow : public TBWindow
{
public:
	ApplicationWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class EditorWindow : public TBMessageHandler, public ApplicationWindow
{
private:
	App* m_application;
	TBStatusbar* m_statusbar;

	/** Show confirmation dialog before shuting down the application. */
	void ShowConfirmationDialog();

	/** Display text in status bar and fade it out. */
	void DisplayStatusMessage(const char* msg) {
		if (!m_statusbar) { return; }
		m_statusbar->DisplayMessage(msg);
	}

public:
	EditorWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);

	void SetApplication(App* app) {
		m_application = app;
	}

	// Implement TBMessageHandler
	virtual void OnMessageReceived(TBMessage *msg);
};

class TurboBadgerEditor : public App
{
private:
	TBStr m_message;

public:
	TurboBadgerEditor() : App(1280, 700) {}

	virtual const char *GetTitle() const { return "Turbobadger - Editor"; }
	virtual void OnBackendAttached(AppBackend *backend, int width, int height);
	virtual bool Init();
	virtual void RenderFrame();
};
