#include <FL/Fl.H>
#include "MainWindow.hpp"

int main(void) {
  MainWindow *main_window = new MainWindow();
  main_window->show();
  return Fl::run();
}
