// $Id: MoveFigTool.cc,v 1.2 2001-05-20 23:42:14 jle Exp $

#include <FL/Fl.H>
#include <FL/Enumerations.H>
#include <FL/Fl_Bitmap.H>
#include "MoveFigTool.hpp"
#include "mathutil.hpp"
#include "icons/movefig_icon.xbm"

static Fl_Bitmap movefig_bitmap
(movefig_icon_bits, movefig_icon_width, movefig_icon_height);

const char *MoveFigTool::get_name() const {
  static const char *name = "move figure";
  return name;
}

void MoveFigTool::draw_icon(int x, int y, int w, int h) const {
  movefig_bitmap.draw(x, y, w, h,
                      (movefig_bitmap.w - w) >> 1,
                      (movefig_bitmap.h - h) >> 1);
}

void MoveFigTool::deactivated(FigureView *view) {
  moving = false;
}

int MoveFigTool::handle(int event, FigureView *view) {
  switch (event) {

  case FL_PUSH:
    if (!moving) {
      moving = true;
      screen_coord[0] = Fl::event_x();
      screen_coord[1] = Fl::event_y();
      view->get_origin(fig_coord[0], fig_coord[1]);
    }
    break;

  case FL_DRAG:
    if (moving) {
      int sx = Fl::event_x();
      int sy = Fl::event_y();
      int origin_x = fig_coord[0] 
        + div_int_fp16u_fp16(screen_coord[0] - sx, view->get_scaling());
      int origin_y = fig_coord[1]
        + div_int_fp16u_fp16(screen_coord[1] - sy, view->get_scaling());
      view->set_origin(origin_x, origin_y);
    }
    break;

  case FL_RELEASE:
    moving = false;
    break;

  default:
    return 0;
  }
  return 1;
}
