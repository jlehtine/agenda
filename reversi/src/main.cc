#include <FL/Fl.h>
#include "MainWindow.hpp"

int main() {
  MainWindow *win = new MainWindow();
  win->show();
  return Fl::run();
}
