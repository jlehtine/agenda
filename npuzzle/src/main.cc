// $Id: main.cc,v 1.8 2001-01-28 15:49:20 jle Exp $

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
  Fl::add_timeout(0, update_time);
  return Fl::run();
}

static void update_time(void *ptr) {
  Fl::repeat_timeout(1, update_time);
  main_window->update_time();
}

static void update_moves() {
  main_window->update_moves();
}
