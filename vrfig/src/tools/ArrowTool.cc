// $Id: ArrowTool.cc,v 1.1 2004-11-10 14:03:05 jle Exp $

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

#include "ArrowTool.hpp"
#include "icons/arrow_icon.xbm"

static Fl_Bitmap arrow_bitmap
(arrow_icon_bits, arrow_icon_width, arrow_icon_height);

virtual const char *ArrowTool::get_name() const {
  static const char *name = "arrow";
  return name;
}

virtual void ArrowTool::draw_icon(int x, int y, int w, int h) const {
}

virtual void ArrowTool::draw(FigureView *view) {
}

virtual int ArrowTool::handle(int event, FigureView *view) {
}
