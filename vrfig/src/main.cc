// $Id: main.cc,v 1.2 2001-05-08 15:31:20 jle Exp $

#include <FL/Fl.H>
#include "MainView.hpp"

int main(int argc, char *argv) {
  new MainView();
  return Fl::run();
}
