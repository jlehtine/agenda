// $Id: main.cc,v 1.9 2001-04-28 16:00:18 jle Exp $

/*
* NPuzzle
* Copyright (C) 2000 Johannes Lehtinen
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* The license can be found in the package directory in file COPYING.
*/

#include <stdlib.h>
#include <time.h>
#include <FL/Fl.H>
#include "MainWindow.hpp"

static MainWindow *main_window;

static void update_time(void *ptr);
static void update_moves();

int main(void) {
  srand(time(NULL));
  main_window = new MainWindow();
  main_window->set_move_callback(update_moves);
  main_window->show();
#ifdef HAVE_AGENDA_FLTK
  Fl::add_timeout((unsigned int)0, update_time, 0);
#else
  Fl::add_timeout(0, update_time);
#endif
  return Fl::run();
}

static void update_time(void *ptr) {
#ifdef HAVE_AGENDA_FLTK
  Fl::repeat_timeout((unsigned int)1000, update_time);
#else
  Fl::repeat_timeout(1, update_time);
#endif
  main_window->update_time();
}

static void update_moves() {
  main_window->update_moves();
}
