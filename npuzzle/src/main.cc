#include <stdlib.h>
#include <time.h>
#include <FL/Fl.H>
#include "MainWindow.hpp"

int main(void) {
  srand(time(NULL));
  MainWindow *main_window = new MainWindow();
  main_window->show();
  return Fl::run();
}
