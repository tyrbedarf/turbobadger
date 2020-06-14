#ifndef DEMO_H
#define DEMO_H

#include "tb_widgets.h"
#include "tb_widgets_common.h"
#include "tb_widgets_reader.h"
#include "tb_widgets_listener.h"
#include "tb_message_window.h"
#include "tb_msg.h"
#include "tb_scroller.h"
#include "../Application.h"

using namespace tb;

class DemoApplication_01 : public App
{
public:
	DemoApplication_01() : App(1280, 700) {}

	virtual const char *GetTitle() const { return "Demo 01"; }
	virtual void OnBackendAttached(AppBackend *backend, int width, int height);
	virtual bool Init();
	virtual void RenderFrame();
};

class ApplicationWindow : public TBWindow
{
public:
	ApplicationWindow(TBWidget *root);
	bool LoadResourceFile(const char *filename);
	void LoadResourceData(const char *data);
	void LoadResource(TBNode &node);

	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class EditorWindow : public ApplicationWindow, public TBMessageHandler
{
public:
	EditorWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);

	// Implement TBMessageHandler
	virtual void OnMessageReceived(TBMessage *msg);
};

class ImageWindow : public ApplicationWindow
{
public:
	ImageWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class PageWindow : public ApplicationWindow, public TBScrollerSnapListener
{
public:
	PageWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
	virtual void OnScrollSnap(TBWidget *target_widget, int &target_x, int &target_y);
};

class AnimationsWindow : public ApplicationWindow
{
public:
	AnimationsWindow(TBWidget *root);
	void Animate();
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class LayoutWindow : public ApplicationWindow
{
public:
	LayoutWindow(TBWidget *root, const char *filename);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class TabContainerWindow : public ApplicationWindow
{
public:
	TabContainerWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class ConnectionWindow : public ApplicationWindow
{
public:
	ConnectionWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);
};

class ScrollContainerWindow : public ApplicationWindow, public TBMessageHandler
{
public:
	ScrollContainerWindow(TBWidget *root);
	virtual bool OnEvent(const TBWidgetEvent &ev);

	// Implement TBMessageHandler
	virtual void OnMessageReceived(TBMessage *msg);
};

#endif // DEMO_H
