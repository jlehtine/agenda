// $Id: MainView.hpp,v 1.5 2001-05-19 08:43:43 jle Exp $

#ifndef __MainWindow_hpp_INCLUDED__
#define __MainWindow_hpp_INCLUDED__

#include <Flek/Fl_App_Window.H>
#include <FL/Fl_Widget.H>
#include "Editor.hpp"
#include "ToolsChoice.hpp"

class ToolsButton;

/**
 * The MainView class controls the main view of the drawing application
 * including the drawing area and toolbar. Notice that this class is not
 * a subclass of Fl_Window.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.5 $
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

  // Callback functions
  static void cb_exit(Fl_Widget *widget, void *data);
  static void cb_tool(Fl_Widget *widget, void *data);
  static void cb_tool_select(Fl_Widget *widget, void *data);
  static void cb_undo(Fl_Widget *widget, void *data);
  static void cb_zoomout(Fl_Widget *widget, void *data);
  static void cb_zoomin(Fl_Widget *widget, void *data);

public:

  MainView();

};

#endif
