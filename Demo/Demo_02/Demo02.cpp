#include "Demo02.h"
#include <stdio.h>
#include <stdarg.h>
#include "tests/tb_test.h"
#include "tb_system.h"
#include "tb_language.h"
#include "tb_inline_select.h"
#include "tb_select.h"
#include "tb_menu_window.h"
#include "tb_editfield.h"
#include "tb_tab_container.h"
#include "tb_bitmap_fragment.h"
#include "animation/tb_widget_animation.h"
#include "tb_node_tree.h"
#include "tb_tempbuffer.h"
#include "tb_font_renderer.h"
#include "image/tb_image_manager.h"
#include "utf8/utf8.h"

// ======================================================
int fps = 0;
uint32 frame_counter_total = 0;
uint32 frame_counter = 0;
double frame_counter_reset_time = 0;

ApplicationWindow::ApplicationWindow(TBWidget *root)
{
	root->AddChild(this);
}

bool ApplicationWindow::OnEvent(const TBWidgetEvent &ev)
{
	if (ev.type == EVENT_TYPE_KEY_DOWN && ev.special_key == TB_KEY_ESC)
	{
		// We could call Die() to fade away and die, but click the close button instead.
		// That way the window has a chance of intercepting the close and f.ex ask if it really should be closed.
		TBWidgetEvent click_ev(EVENT_TYPE_CLICK);
		m_close_button.InvokeEvent(click_ev);

		return true;
	}

	return TBWindow::OnEvent(ev);
}

EditorWindow::EditorWindow(TBWidget *root) :
	ApplicationWindow(root),
	m_application(nullptr),
	m_statusbar(nullptr) {
	SetText("Test Window");
	SetRect(TBRect{ 20, 20, 800, 600 }); // Always add dimensions, TurboBadger will set them to 0. Which means you won´t see anything.

	// test_02_main_window.tb.txt
	g_widgets_reader->LoadFile(this, "Demo/resources/ui_resources/test_02_main_window.tb.txt");
	m_statusbar = GetWidgetByIDAndType<TBStatusbar>(TBIDC("Statusbar"));
}

void EditorWindow::ShowConfirmationDialog() {
	TBMessageWindow *msg_win = new TBMessageWindow(this, TBIDC("confirm_close_dialog"));
	TBMessageWindowSettings settings(TB_MSG_YES_NO);
	settings.dimmer = true;
	settings.styling = true;
	msg_win->Show("Are you sure?", "Do you really want to close the application?", &settings);
}

bool EditorWindow::OnEvent(const TBWidgetEvent &ev) {
	if (ev.type == EVENT_TYPE_CLICK)
	{
		if (ev.ref_id == TBIDC("menu_item_quit") && m_application)
		{
			ShowConfirmationDialog();
			return true;
		}

		if (ev.ref_id == TBIDC("menu_item_save") && m_application)
		{
			DisplayStatusMessage("Menu item save was clicked.");
			return true;
		}

		if (ev.ref_id == TBIDC("menu_item_load") && m_application)
		{
			DisplayStatusMessage("Menu item load was clicked.");
			return true;
		}

		if (ev.ref_id == TBIDC("menu_item_sub_program") && m_application)
		{
			DisplayStatusMessage("Menu item program was clicked.");
			return true;
		}

		if (ev.ref_id == TBIDC("menu_item_sub_author") && m_application)
		{
			DisplayStatusMessage("Menu item author was clicked.");
			return true;
		}

		if (ev.ref_id == TBIDC("menu_item_company") && m_application)
		{
			DisplayStatusMessage("Menu item company was clicked.");
			return true;
		}

		if (ev.target->GetID() == TBIDC("TBWindow.close"))
		{
			// Intercept the TBWindow.close message and stop it from bubbling
			// to TBWindow (prevent the window from closing)
			ShowConfirmationDialog();
			return true;
		}

		if (ev.target->GetID() == TBIDC("confirm_close_dialog") && m_application)
		{
			if (ev.ref_id == TBIDC("TBMessageWindow.yes"))
			{
				m_application->RequestQuit();
				Close();
			}

			return true;
		}
	}

	return ApplicationWindow::OnEvent(ev);
}

void EditorWindow::OnMessageReceived(TBMessage *msg) {

}

bool TurboBadgerEditor::Init()
{
	if (!App::Init())
		return false;

	// Block new animations during Init.
	TBAnimationBlocker anim_blocker;

	// Run unit tests
	int num_failed_tests = TBRunTests();

	if (num_failed_tests)
	{
		TBStr text;
		text.SetFormatted("There is %d failed tests!\nCheck the output for details.", num_failed_tests);
		TBMessageWindow *msg_win = new TBMessageWindow(GetRoot(), TBIDC(""));
		msg_win->Show("Testing results", text);
	}

	// No need to keep track of the pointers.
	// After an element has been removed from the widget tree
	// it will be deleted.
	// As a matter of fact keeping track of the pointer might lead to crashes.
	auto window = new EditorWindow(&m_root);
	window->SetApplication(this);

	return true;
}

void TurboBadgerEditor::RenderFrame()
{
	// Render
	g_renderer->BeginPaint(m_root.GetRect().w, m_root.GetRect().h);
	m_root.InvokePaint(TBWidget::PaintProps());

	frame_counter++;
	frame_counter_total++;

	// Update the FPS counter
	double time = TBSystem::GetTimeMS();
	if (time > frame_counter_reset_time + 1000)
	{
		fps = (int) ((frame_counter / (time - frame_counter_reset_time)) * 1000);
		frame_counter_reset_time = time;
		frame_counter = 0;
	}

	TBWidgetValue *continuous_repaint_val = g_value_group.GetValue(TBIDC("continous-repaint"));
	bool continuous_repaint = continuous_repaint_val ? !!continuous_repaint_val->GetInt() : 0;

	m_root.GetFont()->DrawString(5, m_root.GetRect().h - 20, TBColor(255, 255, 255), m_message);

	g_renderer->EndPaint();

	// If we want continous updates or got animations running, reinvalidate immediately
	if (continuous_repaint || TBAnimationManager::HasAnimationsRunning())
		m_root.Invalidate();
}

void TurboBadgerEditor::OnBackendAttached(AppBackend *backend, int width, int height)
{
	App::OnBackendAttached(backend, width, height);

	// Load language file
	g_tb_lng->Load("resources/language/lng_en.tb.txt");

	// Load the default skin, and override skin that contains the graphics specific to the demo.
	g_tb_skin->Load("resources/default_skin/skin.tb.txt", "Demo/resources/skin/skin.tb.txt");

	// Register font renderers.
#ifdef TB_FONT_RENDERER_TBBF
	void register_tbbf_font_renderer();
	register_tbbf_font_renderer();
#endif

#ifdef TB_FONT_RENDERER_FREETYPE
	void register_freetype_font_renderer();
	register_freetype_font_renderer();
#endif

	// Add resources/fonts we can use to the font manager.
#if defined(TB_FONT_RENDERER_FREETYPE)
	//g_font_manager->AddFontInfo("resources/fonts/vera.ttf", "Vera");
	g_font_manager->AddFontInfo("resources/fonts/LeroyLetteringLightBeta01.ttf", "Leroy Lettering");
#endif

#ifdef TB_FONT_RENDERER_TBBF
	g_font_manager->AddFontInfo("resources/default_font/segoe_white_with_shadow.tb.txt", "Segoe");
	g_font_manager->AddFontInfo("Demo/resources/fonts/neon.tb.txt", "Neon");
	g_font_manager->AddFontInfo("Demo/resources/fonts/orangutang.tb.txt", "Orangutang");
	g_font_manager->AddFontInfo("Demo/resources/fonts/orange.tb.txt", "Orange");
#endif

	// Set the default font description for widgets to one of the resources/fonts we just added
	TBFontDescription fd;
#ifdef TB_FONT_RENDERER_TBBF
	fd.SetID(TBIDC("Segoe"));
#else
	//fd.SetID(TBIDC("Vera"));
	fd.SetID(TBIDC("Leroy Lettering"));
#endif
	fd.SetSize(g_tb_skin->GetDimensionConverter()->DpToPx(12));
	g_font_manager->SetDefaultFontDescription(fd);

	// Create the font now.
	TBFontFace *font = g_font_manager->CreateFontFace(g_font_manager->GetDefaultFontDescription());

	// Render some glyphs in one go now since we know we are going to use them. It would work fine
	// without this since glyphs are rendered when needed, but with some extra updating of the glyph bitmap.
	if (font)
		font->RenderGlyphs(" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~•·åäöÅÄÖ");

	// Turbobadger (at least in this demos) always needs a background image.
	// Otherwise it leads to rendering artifacts.
	m_root.SetSkinBg(TBIDC("background_black"));
}

App *app_create() {
	return new TurboBadgerEditor();
}
