// $Id: MainView.cc,v 1.1.1.1 2001-05-05 18:42:21 jle Exp $

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
#include "icons/outline_icon.xbm"
#include "icons/filled_icon.xbm"
#include "icons/text_icon.xbm"
#include "icons/delete_icon.xbm"
#include "icons/move_icon.xbm"
#include "icons/edit_icon.xbm"
#include "icons/movefig_icon.xbm"

static void cb_exit(Fl_Widget *widget, void *data);
static void cb_tool(Fl_Widget *widget, void *data);
static void cb_undo(Fl_Widget *widget, void *data);
static void cb_zoomout(Fl_Widget *widget, void *data);
static void cb_zoomin(Fl_Widget *widget, void *data);

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

static Fl_Menu_Item file_popup[] = {
  { "New", 0, 0, 0, FL_MENU_DIVIDER },
  { "Load" },
  { "Revert", 0, 0, 0, FL_MENU_DIVIDER },
  { "Save" },
  { "Save As", 0, 0, 0, FL_MENU_DIVIDER },
  { "Exit", 0, cb_exit },
  { 0 }
};

static Fl_Menu_Item info_popup[] = {
  { "Tools" },
  { "Figures" },
  { "Tutorial", 0, 0, 0, FL_MENU_DIVIDER },
  { "About SmartDraw" },
  { 0 }
};

MainView::MainView() {
  win = Widget_Factory::new_window("SmartDraw");

  // Create toolbar
  Fl_Dockable_Window *toolbar = Widget_Factory::new_toolbar();
  Fl_Menu_Button *file_menu = Widget_Factory::new_menu_button("File");
  file_menu->menu(file_popup);

  // Create tool choice
  Fl_Choice *tools_choice = Widget_Factory::new_choice(0, cb_tool);
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

  Widget_Factory::new_button("Undo", cb_undo);
  Fl_Button *zoomout_button = Widget_Factory::new_button("-", cb_zoomout);
  zoomout_button->resize(zoomout_button->x(), zoomout_button->y(),
                         Widget_Factory::buttonheight(),
                         zoomout_button->h());
  Fl_Button *zoomin_button = Widget_Factory::new_button("+", cb_zoomin);
  zoomin_button->resize(zoomin_button->x(), zoomin_button->y(),
                         Widget_Factory::buttonheight(),
                         zoomin_button->h());
  Fl_Menu_Button *info_menu = Widget_Factory::new_menu_button("?");
  info_menu->menu(info_popup);
  info_menu->resize(info_menu->x(), info_menu->y(),
                    Widget_Factory::buttonheight(), info_menu->h());
  toolbar->end();
  win->add_dockable(toolbar, 1);

  win->end();
  win->show();
}

static void cb_exit(Fl_Widget *widget, void *data) {
  exit(0);
}

static void cb_tool(Fl_Widget *widget, void *data) {
}

static void cb_undo(Fl_Widget *widget, void *data) {
}

static void cb_zoomout(Fl_Widget *widget, void *data) {
}

static void cb_zoomin(Fl_Widget *widget, void *data) {
}
