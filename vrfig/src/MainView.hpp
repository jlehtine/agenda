// $Id: MainView.hpp,v 1.2 2001-05-06 22:12:46 jle Exp $

#ifndef __MainWindow_hpp_INCLUDED__
#define __MainWindow_hpp_INCLUDED__

#include <Flek/Fl_App_Window.H>
#include <FL/Fl_Widget.H>

/**
 * The MainView class controls the main view of the drawing application
 * including the drawing area and toolbar. Notice that this class is not
 * a subclass of Fl_Window.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class MainView {

protected:

  Fl_App_Window *win;

public:

  MainView();

};

#endif
