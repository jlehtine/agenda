// $Id: main.cc,v 1.1.1.1 2001-05-05 18:42:21 jle Exp $

#include <FL/Fl.H>
#include "MainView.hpp"

int main(int argc, char *argv) {
  MainView *mv = new MainView();
  return Fl::run();
}
