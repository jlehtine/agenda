// $Id: MainView.cc,v 1.7 2001-05-16 19:50:16 jle Exp $

#include <stdlib.h>
#include <flpda/Widget_Factory.h>
#include <Flek/Fl_Dockable_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include "MainView.hpp"
#include "Editor.hpp"
#include "DrawingTool.hpp"
#include "MoveFigTool.hpp"
#include "mathutil.hpp"
#include "icons/outline_icon.xbm"
#include "icons/filled_icon.xbm"
#include "icons/text_icon.xbm"
#include "icons/delete_icon.xbm"
#include "icons/move_icon.xbm"
#include "icons/edit_icon.xbm"
#include "icons/movefig_icon.xbm"

static Fl_Bitmap outline_bitmap
(outline_icon_bits, outline_icon_width, outline_icon_height);
static Fl_Bitmap filled_bitmap
(filled_icon_bits, filled_icon_width, filled_icon_height);
static Fl_Bitmap text_bitmap
(text_icon_bits, text_icon_width, text_icon_height);
static Fl_Bitmap delete_bitmap
(delete_icon_bits, delete_icon_width, delete_icon_height);
static Fl_Bitmap move_bitmap
(move_icon_bits, move_icon_width, move_icon_height);
static Fl_Bitmap edit_bitmap
(edit_icon_bits, edit_icon_width, edit_icon_height);
static Fl_Bitmap movefig_bitmap
(movefig_icon_bits, movefig_icon_width, movefig_icon_height);

static Fl_Menu_Item tools_popup[] = {
  { "Outline" },
  { "Filled" },
  { "Text", 0, 0, 0, FL_MENU_DIVIDER },
  { "Delete" },
  { "Move" },
  { "Edit", 0, 0, 0, FL_MENU_DIVIDER },
  { "Move Figure" },
  { 0 }
};

static Tool *tools[] = {
#if EXPERIMENTAL_UI
  new DrawingTool(),
  new DrawingTool(), // For now
  new DrawingTool(), // For now
  new DrawingTool(), // For now
  new DrawingTool(), // For now
  new DrawingTool(), // For now
#else
  new MoveFigTool(),
  new MoveFigTool(),
  new MoveFigTool(),
  new MoveFigTool(),
  new MoveFigTool(),
  new MoveFigTool(),
#endif
  new MoveFigTool()
};

enum tool_types { OUTLINE = 0, FILLED = 1, TEXT = 2, DELETE = 3, MOVE = 4,
                  EDIT = 5, MOVEFIG = 6 };

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

  // Create toolbar
  Fl_Dockable_Window *toolbar = Widget_Factory::new_toolbar();
  Fl_Menu_Button *file_menu = Widget_Factory::new_menu_button("File");
  file_menu->menu(file_popup);
  file_popup[5].callback(cb_exit, this);

  // Create tool choice
  Fl_Choice *tools_choice = Widget_Factory::new_choice(0, cb_tool, this);
  tools_choice->resize(tools_choice->x(), tools_choice->y(),
                       5 * Widget_Factory::buttonheight() / 2,
                       tools_choice->h());
  outline_bitmap.label(tools_popup);
  filled_bitmap.label(tools_popup + 1);
  text_bitmap.label(tools_popup + 2);
  delete_bitmap.label(tools_popup + 3);
  move_bitmap.label(tools_popup + 4);
  edit_bitmap.label(tools_popup + 5);
  movefig_bitmap.label(tools_popup + 6);
  tools_choice->menu(tools_popup);

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
  editor->set_tool(tools[0]);

  win->end();
  win->show();
}

void MainView::cb_exit(Fl_Widget *widget, void *data) {
  exit(0);
}

void MainView::cb_tool(Fl_Widget *widget, void *data) {
  MainView *view = (MainView *)data;
  Fl_Choice *tools_choice = (Fl_Choice *)widget;
  view->editor->set_tool(tools[tools_choice->value()]);
}

void MainView::cb_undo(Fl_Widget *widget, void *data) {
}

void MainView::cb_zoomout(Fl_Widget *widget, void *data) {
  MainView *view = (MainView *)data;
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
  MainView *view = (MainView *)data;
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
