// $Id: MainWindowBase.hpp,v 1.1 2000-11-12 06:47:08 jle Exp $

#ifndef __MainWindowBase_hpp_INCLUDED__
#define __MainWindowBase_hpp_INCLUDED__

#include <FL/Fl_Window.H>

class MainWindowBase {
  
protected:

  /** Handle to the real FLTK window */
  Fl_Window *win;

public:

  /**
   * Shows the window after it has been created.
   */
  inline void show() {
    win->show();
  }
};

#endif
