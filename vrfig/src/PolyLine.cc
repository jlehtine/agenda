// $Id: PolyLine.cc,v 1.2 2001-05-08 21:20:01 jle Exp $

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PolyLine.hpp"
#include "mathutil.hpp"

void PolyLine::draw(int origin_x, int origin_y, unsigned int scaling) {
  fl_color(FL_BLACK);
  vector<int>::iterator i = points.begin();
  i += 2;
  while (i < points.end()) {
    fl_line(coord_to_screen(*(i-2), origin_x, scaling),
            coord_to_screen(*(i-1), origin_y, scaling),
            coord_to_screen(*i, origin_x, scaling),
            coord_to_screen(*(i+1), origin_y, scaling));
    i += 2;
  }
}

PolyLine *PolyLine::fit_to_points(
  vector<int> *_points, int origin_x, int origin_y, unsigned int scaling) {

  // Check that we have enough points for a polyline
  if (_points->size() < 4)
    return 0;
  
  PolyLine *line = new PolyLine();

  // Check if the user would like to draw a polygon
  int xdiff = (*(_points->begin())) - (*(_points->end() - 2));
  int ydiff = (*(_points->begin() + 1)) - (*(_points->end() - 1));
  if (xdiff*xdiff + ydiff*ydiff <= 9)
    line->closed = true;
  else
    line->closed = false;
  
  // Find the longest possible segments
  line->points.insert
    (line->points.end(),
     screen_to_coord(*(_points->begin()), origin_x, scaling));
  line->points.insert
    (line->points.end(),
     screen_to_coord(*(_points->begin() + 1), origin_y, scaling));
  vector<int>::iterator end = _points->end();
  vector<int>::iterator i = _points->begin();
  while (i < end - 2) {
    int *seg_start = i;
    i += 2;
    bool ok = true;
    while (ok && i < end) {

      // Check whether the poinds in the middle are close enough to the line
      vector<int>::iterator j = seg_start + 2;
      while (j < i) {
        if (distance_from_line
            ((*j) << 16, (*(j+1)) << 16, 
             (*seg_start) << 16, (*(seg_start+1)) << 16,
             (*i - *seg_start) << 16, 
             (*(i+1) - *(seg_start+1)) << 16) > 0x100000) {
          ok = false;
          break;
        }
        j += 2;
      }
      i += 2;
    }

    // Add segment to points list
    i -= 2;
    line->points.insert(line->points.end(),
                        screen_to_coord(*i, origin_x, scaling));
    line->points.insert(line->points.end(),
                        screen_to_coord(*(i+1), origin_y, scaling));
  }

  return line;
}
