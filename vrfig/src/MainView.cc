// $Id: MainView.cc,v 1.9 2001-05-18 13:16:54 jle Exp $

#include <stdlib.h>
#include <flpda/Widget_Factory.h>
#include <Flek/Fl_Dockable_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include "MainView.hpp"
#include "ToolFactory.hpp"
#include "mathutil.hpp"

// Tool selection button. This is an ordinary button except that it
// displays the icon of the selected tool.
class ToolsButton : public Fl_Button {

protected:
  
  // The active tool
  Tool *active_tool;

public:

  inline ToolsButton(int x, int y, int w, int h, const char *n = 0):
    Fl_Button(x, y, w, h, n), active_tool(0) {}

  // Set the active tool
  inline void set_active_tool(Tool *t) {
    active_tool = t;
    redraw();
  }

  // Draw the button (with tool icon)
  void draw() {
    Fl_Button::draw();
    if (active_tool && w() > 4 && h() > 4) {
      fl_color(labelcolor());
      active_tool->draw_icon(x() + 2, y() + 2, w() - 4, h() - 4);
    }
  }
};

static Fl_Menu_Item file_popup[] = {
  { "New", 0, 0, 0, FL_MENU_DIVIDER },
  { "Load" },
  { "Revert", 0, 0, 0, FL_MENU_DIVIDER },
  { "Save" },
  { "Save As", 0, 0, 0, FL_MENU_DIVIDER },
  { "Exit" },
  { 0 }
};

static Fl_Menu_Item info_popup[] = {
  { "Tools" },
  { "Figures" },
  { "Tutorial", 0, 0, 0, FL_MENU_DIVIDER },
  { "About VRFig" },
  { 0 }
};

MainView::MainView() {
  win = Widget_Factory::new_window("VRFig");

  // Create tool list
  tools = ToolFactory::create_tools();

  // Create toolbar
  Fl_Dockable_Window *toolbar = Widget_Factory::new_toolbar();
  Fl_Menu_Button *file_menu = Widget_Factory::new_menu_button("File");
  file_menu->menu(file_popup);
  file_popup[5].callback(cb_exit, this);

  // Create tool selection button (taken from FLPDA)
  ToolsButton *tools_button = new ToolsButton
    (0, 0, Widget_Factory::buttonheight(), Widget_Factory::buttonheight());
#ifdef FLTK_VR3
  tools_button->box(VR_BUTTON_UP_BOX);
  tools_button->down_box(VR_BUTTON_DOWN_BOX);
  tools_button->selection_color(FL_BLACK);
#endif
  tools_button->callback(cb_tool, this);

  Widget_Factory::new_button("Undo", cb_undo, this);
  Fl_Button *zoomout_button = 
    Widget_Factory::new_button("-", cb_zoomout, this);
  zoomout_button->resize(zoomout_button->x(), zoomout_button->y(),
                         Widget_Factory::buttonheight(),
                         zoomout_button->h());
  Fl_Button *zoomin_button = 
    Widget_Factory::new_button("+", cb_zoomin, this);
  zoomin_button->resize(zoomin_button->x(), zoomin_button->y(),
                         Widget_Factory::buttonheight(),
                         zoomin_button->h());
  Fl_Menu_Button *info_menu = Widget_Factory::new_menu_button("?");
  info_menu->menu(info_popup);
  info_menu->resize(info_menu->x(), info_menu->y(),
                    Widget_Factory::buttonheight(), info_menu->h());
  toolbar->end();
  win->add_dockable(toolbar, 1);

  // Create editor view
  editor = new Editor(0, 0, win->w(), win->h() - toolbar->h());
  win->contents()->add(editor);
  win->contents()->resizable(editor);
  editor->set_figure(new Figure());

  // Create the tools choice
  tools_choice = new ToolsChoice(0, 0, tools);

  win->end();
  win->show();

  // Select the default tool
  active_tool = *tools->begin();
  tools_button->set_active_tool(active_tool);
  editor->set_tool(active_tool);
}

void MainView::cb_exit(Fl_Widget *widget, void *data) {
  exit(0);
}

void MainView::cb_tool(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  ToolsChoice *tc = view->tools_choice;
  Fl_App_Window *w = view->win;
  tc->resize(w->x() + Widget_Factory::buttonwidth(),
             w->y() + w->h() - Widget_Factory::buttonheight() - tc->h(),
             tc->w(),
             tc->h());
  tc->show();
}

void MainView::cb_tool_select(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  Tool *old_tool = view->active_tool;
  view->active_tool = (dynamic_cast<ToolsChoice *>(widget))->get_tool();
  if (view->active_tool != old_tool)
    view->editor->set_tool(view->active_tool);
}

void MainView::cb_undo(Fl_Widget *widget, void *data) {
}

void MainView::cb_zoomout(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  int scaling = view->editor->get_scaling();
  int ox, oy;
  view->editor->get_origin(ox, oy);
  ox -= div_int_fp16u_fp16(view->editor->w() >> 1, scaling);
  oy -= div_int_fp16u_fp16(view->editor->h() >> 1, scaling);
  scaling >>= 1;
  if (scaling > 256) {
    view->editor->set_scaling(scaling);
    view->editor->set_origin(ox, oy);
  }
}

void MainView::cb_zoomin(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  int scaling = view->editor->get_scaling();
  scaling <<= 1;
  if (scaling) {
    int ox, oy;
    view->editor->get_origin(ox, oy);
    ox += div_int_fp16u_fp16(view->editor->w() >> 1, scaling);
    oy += div_int_fp16u_fp16(view->editor->h() >> 1, scaling);
    view->editor->set_scaling(scaling);
    view->editor->set_origin(ox, oy);
  }
}
