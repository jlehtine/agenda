// $Id: flext.cc,v 1.1 2001-05-16 19:50:16 jle Exp $

#include <X11/Xlib.h>
#include <FL/fl_draw.H>
#include <FL/x.H>
#include "flext.hpp"

void fle_xorred_point(int x, int y) {
  int old_func = fle_xorred_mode();
  fl_point(x, y);
  fle_reset_mode(old_func);
}

void fle_xorred_line(int x, int y, int x1, int y1) {
  int old_func = fle_xorred_mode();
  fl_line(x, y, x1, y1);
  fle_reset_mode(old_func);
}

int fle_xorred_mode() {
  XGCValues gcValues;
  XGetGCValues(fl_display, fl_gc, GCFunction, &gcValues);
  int old_func = gcValues.function;
  gcValues.function = GXxor;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
  return old_func;
}

void fle_reset_mode(int old_func) {
  XGCValues gcValues;
  gcValues.function = old_func;
  XChangeGC(fl_display, fl_gc, GCFunction, &gcValues);
}
