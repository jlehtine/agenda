// $Id: flext.cc,v 1.2 2001-05-23 12:47:51 jle Exp $

/*--------------------------------------------------------------------------
 * VRFig, a vector graphics editor for PDA environment
 * Copyright 2001 Johannes Lehtinen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *------------------------------------------------------------------------*/

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
