#include <stdlib.h>
#include <time.h>
#include <FL/Fl.H>
#include "MainWindow.hpp"

static MainWindow *main_window;

static void update_time(void *ptr);
static void update_moves();
static void settings_updated();

int main(void) {
  srand(time(NULL));
  main_window = new MainWindow();
  main_window->set_move_callback(update_moves);
  main_window->set_settings_callback(settings_updated);
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

static void settings_updated() {
  main_window->settings_updated();
}
