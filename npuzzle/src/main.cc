#include <stdlib.h>
#include <time.h>
#include <FL/Fl.H>
#include "MainWindow.hpp"

static MainWindow *main_window;

static void update_time(void *ptr);

int main(void) {
  srand(time(NULL));
  main_window = new MainWindow();
  main_window->show();
  Fl::add_timeout(0, update_time);
  return Fl::run();
}

static void update_time(void *ptr) {
  Fl::repeat_timeout(1, update_time);
  main_window->update_time();
}
