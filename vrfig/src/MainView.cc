// $Id: MainView.cc,v 1.23 2001-06-10 18:36:43 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream.h>
#include <fstream.h>
#include <flpda/Widget_Factory.h>
#include <Flek/Fl_Dockable_Window.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Bitmap.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Button.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include "MainView.hpp"
#include "ToolFactory.hpp"
#include "ElementFactory.hpp"
#include "ActionBuffer.hpp"
#include "mathutil.hpp"
#include "espws/FileChooser.h"
#include "icons/directory_icon.xbm"
#include "icons/vfg_icon.xbm"

static Fl_Bitmap vfg_bitmap
(vfg_icon_bits, vfg_icon_width, vfg_icon_height);

static Fl_Bitmap directory_bitmap
(directory_icon_bits, directory_icon_width, directory_icon_height);

static Fl_App_Window *create_about_win();
static void cb_about_done(Fl_Widget *widget, void *data);

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

  win = Widget_Factory::new_window("VRFig");

  // Create tool list
  tools = ToolFactory::create_tools();

  // Create about window
  about_win = create_about_win();

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

  undo_button = Widget_Factory::new_button("Undo", cb_undo, this);
  undo_button->deactivate();
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
  Fl_Menu_Button *info_menu = Widget_Factory::new_menu_button("Help");
  info_menu->menu(info_popup);
  info_popup[0].deactivate();
  info_popup[1].deactivate();
  info_popup[2].deactivate();
  info_popup[3].callback(cb_about, this);
  toolbar->end();
  win->add_dockable(toolbar, 1);

  // Create editor view
  editor = new Editor(0, 0, win->w(), win->h() - toolbar->h(),
                      new ActionBuffer());
  editor->get_action_buffer()->set_callback(cb_action_buffer, this);
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

  // Check if $HOME/vrfig exists and if not, whether it should be created
  string path("");
  char *home = getenv("HOME");
  if (home) {
    path.assign(home);
    path.append("/vrfig");
    struct stat stat_info;
    if (stat(path.c_str(), &stat_info)) {

      // Create $HOME/figures if user accepts
      if (fl_ask("Subdirectory 'vrfig' does not exist in your home "
                 "directory. Would you like to create it for VRFig "
                 "drawings?")) {
        if (mkdir(path.c_str(), 0777)) {
          fl_alert("Could not create subdirectory 'vrfig'.");
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
    path.c_str(), "*.vfg", FileChooser::CREATE, "VRFig Save As");
  file_chooser_load = new FileChooser(
    path.c_str(), "*.vfg", FileChooser::SINGLE, "VRFig Load");
}

bool MainView::check_discard() {
  if (editor->get_action_buffer()->is_dirty())
    return fl_ask("Changes made to the current drawing will be discarded. "
                  "Do you want to proceed?");
  else
    return true;
}

void MainView::cb_exit(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (!view->check_discard())
    return;
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
  MainView *view = reinterpret_cast<MainView *>(data);
  view->editor->set_tool(0);
  view->editor->get_action_buffer()->undo_action();
  view->editor->set_tool(view->active_tool);
}

void MainView::cb_zoomout(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  int scaling = view->editor->get_scaling();
  int ox, oy;
  view->editor->get_origin(ox, oy);
  ox -= length_from_screen(view->editor->w() >> 1, scaling);
  oy += length_from_screen(view->editor->h() >> 1, scaling);
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
    fp16 ox, oy;
    view->editor->get_origin(ox, oy);
    ox += length_from_screen(view->editor->w() >> 1, scaling);
    oy -= length_from_screen(view->editor->h() >> 1, scaling);
    view->editor->set_scaling(scaling);
    view->editor->set_origin(ox, oy);
  }
}

void MainView::cb_new(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (!view->check_discard())
    return;

  Figure *fig = view->editor->get_figure();
  view->editor->set_figure(new Figure());
  if (fig)
    delete fig;
  view->current_file.assign("");
}

void MainView::cb_load(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (!view->check_discard())
    return;
  view->file_chooser_load->show();
  view->file_chooser_load->rescan();
  while (view->file_chooser_load->visible())
    Fl::wait();
  const char *path = view->file_chooser_load->value();
  if (!path || path[0] == '\0')
    return;
  view->current_file.assign(path);
  cb_revert(widget, data);
}

void MainView::cb_revert(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (view->current_file.length() == 0) {
    fl_message("No file to revert from.");
    return;
  }

  // Load figure from the specified file
  ifstream ifs(view->current_file.c_str());
  if (ifs.bad()) {
    fl_alert("Could not open input file.");
    return;
  }
  Figure *fig = ElementFactory::deserialize(ifs);
  ifs.close();
  if (!fig)
    return;
  Figure *old_fig = view->editor->get_figure();
  view->editor->set_figure(fig);
  if (old_fig)
    delete old_fig;
}

void MainView::cb_save(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  
  // Check if actually a Save As operation
  if (view->current_file.length() == 0) {
    cb_save_as(widget, data);
    return;
  }

  // Save the figure to file
  ofstream ofs(view->current_file.c_str(), ios::out, 0666);
  if (ofs.bad()) {
    fl_alert("Error when creating the save file.");
    return;
  }
  view->editor->get_figure()->serialize(ofs);
  ofs.close();
  if (ofs.bad())
    fl_alert("Error when saving the drawing.");
  else
    view->editor->get_action_buffer()->clear_dirty();
}

void MainView::cb_save_as(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  view->file_chooser_save->show();
  view->file_chooser_save->rescan();
  while (view->file_chooser_save->visible())
    Fl::wait();
  const char *path = view->file_chooser_save->value();
  if (!path || path[0] == '\0')
    return;
  view->current_file.assign(path);
  
  // Add the trailing .vfg if nothing else specified
  int i = view->current_file.length() - 1;
  int c;
  while (i >= 0) {
    c = view->current_file[i];
    if (c == '.' || c == '/')
      break;
    i--;
  }
  if (i < 0 || c == '/')
    view->current_file.append(".vfg");

  // Check if overwriting an existing file
  struct stat stat_info;
  if (!stat(view->current_file.c_str(), &stat_info)) {
    if (!fl_ask("Are you sure you want to overwrite an existing file '%s'?",
                view->current_file.c_str()))
      return;
  }
  
  cb_save(widget, data);
}

void MainView::cb_about(Fl_Widget *widget, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  view->about_win->show();
}

void MainView::cb_action_buffer(ActionBuffer *buffer, void *data) {
  MainView *view = reinterpret_cast<MainView *>(data);
  if (buffer->can_undo()) 
    view->undo_button->activate();
  else
    view->undo_button->deactivate();
}

static Fl_App_Window *create_about_win() {
  Fl_App_Window *about_win = Widget_Factory::new_window("About VRFig");

  // Create the toolbar with "Done" button
  Fl_Dockable_Window *toolbar = Widget_Factory::new_toolbar();
  Widget_Factory::new_button("Done", cb_about_done, about_win);
  toolbar->end();
  about_win->add_dockable(toolbar, 1);

  // Create the label
  Fl_Box *vrfig = new Fl_Box(0, 0, about_win->w(), 40, "VRFig");
  vrfig->labelfont(FL_HELVETICA_BOLD);
  about_win->contents()->add(vrfig);
  
  // Create the version text
  Fl_Box *ver = new Fl_Box(0, 40, about_win->w(), 20, "v " VERSION);
  ver->labelsize(Widget_Factory::labelsize());
  about_win->contents()->add(ver);
  
  // Create the copyright text
  Fl_Box *copyright = new Fl_Box(
    0, 60, about_win->w(), 40,
    "Copyright 2001\nJohannes Lehtinen");
  copyright->labelsize(Widget_Factory::labelsize());
  about_win->contents()->add(copyright);

  // Create the license text
  Fl_Box *gpl = new Fl_Box(
    5, 100, about_win->w() - 5, about_win->h() - 100 - toolbar->h(),
    "This program is free software; you can redistribute it and/or modify "
    "it under the terms of the GNU General Public License as published by "
    "the Free Software Foundation; either version 2 of the License, or "
    "(at your option) any later version.");
  gpl->labelsize(8);
  gpl->align(FL_ALIGN_WRAP);
  about_win->contents()->add(gpl);
  about_win->contents()->resizable(gpl);

  about_win->end();
  about_win->set_modal();
  return about_win;
}

static void cb_about_done(Fl_Widget *widget, void *data) {
  Fl_App_Window *about_win = reinterpret_cast<Fl_App_Window *>(data);
  about_win->hide();
}
