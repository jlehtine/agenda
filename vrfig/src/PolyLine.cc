// $Id: PolyLine.cc,v 1.1 2001-05-07 21:02:10 jle Exp $

#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PolyLine.hpp"
#include "transform.hpp"

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
  PolyLine *line = new PolyLine();
  line->points.reserve(_points->size());
  vector<int>::iterator i = _points->begin();
  while (i < _points->end()) {
    line->points.insert(line->points.end(),
                        screen_to_coord(*i, origin_x, scaling));
    i++;
    line->points.insert(line->points.end(),
                        screen_to_coord(*i, origin_y, scaling));
    i++;
  }
  return line;
}
