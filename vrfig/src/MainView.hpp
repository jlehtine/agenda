// $Id: MainView.hpp,v 1.9 2001-05-23 12:47:50 jle Exp $

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

#ifndef __MainWindow_hpp_INCLUDED__
#define __MainWindow_hpp_INCLUDED__

#include <Flek/Fl_App_Window.H>
#include <FL/Fl_Widget.H>
#include "Editor.hpp"
#include "ToolsChoice.hpp"
#include "espws/FileChooser.h"

class ToolsButton;

/**
 * The MainView class controls the main view of the drawing application
 * including the drawing area and toolbar. Notice that this class is not
 * a subclass of Fl_Window.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.9 $
 */
class MainView {

protected:

  /** The main application window */
  Fl_App_Window *win;

  /** The editor widget */
  Editor *editor;

  /** The tools available */
  vector<Tool *> *tools;

  /** The currently active tool */
  Tool *active_tool;

  /** The tool selection button */
  ToolsButton *tools_button;

  /** The menu window for tool selection */
  ToolsChoice *tools_choice;

  /** The file chooser for saving */
  FileChooser *file_chooser_save;

  /** The file chooser for loading */
  FileChooser *file_chooser_load;

  /** The filename for the current figure or empty string if none */
  string current_file;

  /** The about window */
  Fl_App_Window *about_win;

  // Callback functions
  static void cb_exit(Fl_Widget *widget, void *data);
  static void cb_tool(Fl_Widget *widget, void *data);
  static void cb_tool_select(Fl_Widget *widget, void *data);
  static void cb_undo(Fl_Widget *widget, void *data);
  static void cb_zoomout(Fl_Widget *widget, void *data);
  static void cb_zoomin(Fl_Widget *widget, void *data);
  static void cb_new(Fl_Widget *widget, void *data);
  static void cb_load(Fl_Widget *widget, void *data);
  static void cb_revert(Fl_Widget *widget, void *data);
  static void cb_save(Fl_Widget *widget, void *data);
  static void cb_save_as(Fl_Widget *widget, void *data);
  static void cb_about(Fl_Widget *widget, void *data);

public:

  MainView();

};

#endif
