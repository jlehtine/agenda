#include <FL/Fl.H>
#include "MainWindow.hpp"

int main(void) {
  MainWindow *main_window = new MainWindow();
  return Fl::run();
}
