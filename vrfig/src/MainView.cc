// $Id: MainView.cc,v 1.12 2001-05-20 11:49:43 jle Exp $

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream.h>
#include <flpda/Widget_Factory.h>
#include <Flek/Fl_Dockable_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include "MainView.hpp"
#include "ToolFactory.hpp"
#include "mathutil.hpp"
#include "espws/FileChooser.h"
#include "icons/directory_icon.xbm"
#include "icons/vfg_icon.xbm"

static Fl_Bitmap vfg_bitmap
(vfg_icon_bits, vfg_icon_width, vfg_icon_height);

static Fl_Bitmap directory_bitmap
(directory_icon_bits, directory_icon_width, directory_icon_height);

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

// File icon which displays X bitmaps
class BitmapFileIcon : public FileIcon {

protected:
  Fl_Bitmap *bitmap;
  
public:
  BitmapFileIcon(Fl_Bitmap *bitmap,
              const char *p, int t, int nd = 0, short *d = 0):
    FileIcon(p, t, nd, d), bitmap(bitmap) {}

  virtual void draw(int x, int y, int w, int h, Fl_Color ic, int active = 1) {
    fl_color(ic);
    bitmap->draw(x, y, w, h, (bitmap->w - w) >> 1, (bitmap->h - h) >> 1);
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

MainView::MainView(): current_file("") {
  current_file[0] = '\0';

  win = Widget_Factory::new_window("VRFig");

  // Create tool list
  tools = ToolFactory::create_tools();

  // Create toolbar
  Fl_Dockable_Window *toolbar = Widget_Factory::new_toolbar();
  Fl_Menu_Button *file_menu = Widget_Factory::new_menu_button("File");
  file_menu->menu(file_popup);
  file_popup[0].callback(cb_new, this);
  file_popup[1].callback(cb_load, this);
  file_popup[2].callback(cb_revert, this);
  file_popup[3].callback(cb_save, this);
  file_popup[4].callback(cb_save_as, this);
  file_popup[5].callback(cb_exit, this);

  // Create tool selection button (taken from FLPDA)
  tools_button = new ToolsButton
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
  tools_choice->end();
  tools_choice->callback(cb_tool_select, this);

  win->end();
  win->show();

  // Select the default tool
  active_tool = *tools->begin();
  tools_button->set_active_tool(active_tool);
  editor->set_tool(active_tool);

  // Check if $HOME/figures exists and if not, whether it should be created
  string path("");
  char *home = getenv("HOME");
  if (home) {
    path.assign(home);
    path.append("/figures");
    struct stat stat_info;
    if (stat(path.data(), &stat_info)) {

      // Create $HOME/figures if user accepts
      if (fl_ask("$HOME/figures does not exist. Would you like to create it "
                 "for VRFig figure files?")) {
        if (mkdir(path.data(), 0777)) {
          fl_alert("Could not create $HOME/figures.");
          path.assign("");
        }
      } else
        path.assign("");
    }
  }

  // Fallback to current working directory or root as a last resort
  if (path.length() == 0) {
    char directory[256];
    if (!getcwd(directory, 256))
      path.assign("/");
    path.assign(directory);
  }

  // Initialize icons for file chooser
  new BitmapFileIcon(&directory_bitmap, "*", FileIcon::DIRECTORY);
  new BitmapFileIcon(&vfg_bitmap, "*.vfg", FileIcon::PLAIN);
  
  // Create file choosers
  file_chooser_save = new FileChooser(
    path.data(), "*.vfg", FileChooser::CREATE, "VRFig Save As");
  file_chooser_load = new FileChooser(
    path.data(), "*.vfg", FileChooser::SINGLE, "VRFig Load");
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
  if (view->active_tool != old_tool) {
    view->editor->set_tool(view->active_tool);
    view->tools_button->set_active_tool(view->active_tool);
  }
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

void MainView::cb_new(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  Figure *fig = view->editor->get_figure();
  view->editor->set_figure(new Figure());
  if (fig)
    delete fig;
}

void MainView::cb_load(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  view->file_chooser_load->show();
  while (view->file_chooser_load->visible())
    Fl::wait();
  if (!(view->file_chooser_load->value()))
    return;
  view->current_file.assign(view->file_chooser_load->value());
  cb_revert(widget, data);
}

void MainView::cb_revert(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (view->current_file.length() == 0) {
    fl_message("No file to revert from.");
    return;
  }

  // XXX: Enter the actual loading code here
}

void MainView::cb_save(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  
  // Check if actually a Save As operation
  if (view->current_file.length() == '\0')
    cb_save_as(widget, data);

  // XXX: Enter the actual saving code here
}

void MainView::cb_save_as(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  view->file_chooser_save->show();
  while (view->file_chooser_save->visible())
    Fl::wait();
  if (!(view->file_chooser_save->value()))
    return;
  view->current_file.assign(view->file_chooser_save->value());
  cb_save(widget, data);
}
