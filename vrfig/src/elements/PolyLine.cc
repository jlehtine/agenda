// $Id: PolyLine.cc,v 1.1 2001-05-17 19:35:28 jle Exp $

#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PolyLine.hpp"
#include "mathutil.hpp"
#include "flext.hpp"

#define CLOSED_MAX_GAP 16
#define DEVIATION_MAX 25

#if USE_EXPERIMENTAL_UI
/**
 * Checks if the specified set of points is no more than a maximum
 * distance away from the specified line.
 *
 * @param points_begin the beginning of points to check
 * @param points_end the end of points to check
 * @param l1 the line end point 1
 * @param l2 the line end point 2
 */
static bool check_line(int *points_begin, int *points_end, int *l1, int *l2);
#endif

PolyLine::~PolyLine() {
  points.clear();
}

const string *PolyLine::get_name() const {
  static const string name("polyline");
  return &name;
}

const string *PolyLine::get_namespace() const {
  return &vrf_default_namespace;
}

void PolyLine::draw(int origin_x, int origin_y, unsigned int scaling,
                    bool xorred) const {
  fl_color(FL_BLACK);
  vector<int>::const_iterator i = points.begin();
  i += 2;
  int old_func = 0;
  if (xorred)
    old_func = fle_xorred_mode();
  while (i < points.end()) {
    int x = coord_to_screen(*(i-2), origin_x, scaling);
    int y = coord_to_screen(*(i-1), origin_y, scaling);
    int x1 = coord_to_screen(*i, origin_x, scaling);
    int y1 = coord_to_screen(*(i+1), origin_y, scaling);
    if (xorred && i > points.begin() + 2)
      fl_point(x, y);
    fl_line(x, y, x1, y1);
    i += 2;
  }
  if (xorred)
    fle_reset_mode(old_func);
}

void PolyLine::get_bounding_box(int &x, int &y, int &w, int &h) const {
  vector<int>::const_iterator i = points.begin();
  x = *(i++);
  y = *(i++);
  w = 1;
  h = 1;
  while (i < points.end()) {
    if (*i < x) {
      w += x - *i;
      x = *i;
    } else if (*i >= x + w)
      w += *i - (x + w) + 1;
    i++;
    if (*i < y) {
      h += y - *i;
      y = *i;
    } else if (*i >= y + h)
      h += *i - (y + h) + 1;
    i++;
  }
}

ostream &PolyLine::serialize(ostream &os, const string *ns, int indent) const {
  static const string elem_points("points");
  static const string elem_point("point");

  output_indent(os, indent);
  output_ns_name(os << "<", ns, &elem_points) << 
    " num=\"" << (points.size() >> 1) << "\">\n";
  vector<int>::const_iterator i = points.begin();
  while (i < points.end()) {
    output_indent(os, indent+2);
    output_ns_name(os << "<", ns, &elem_point) <<
      " x=\"" << (*i) << "\" y=\"" << *(i+1) << "\"/>\n";
    i += 2;
  }
  output_indent(os, indent);
  output_ns_name(os << "</", ns, &elem_points) << ">\n";
  return os;
}

#if USE_EXPERIMENTAL_UI
PolyLine *PolyLine::fit_to_points(
  vector<int> *_points, int origin_x, int origin_y, unsigned int scaling) {

  // Check that we have enough points for a polyline
  if (_points->size() < 4)
    return 0;
  
  PolyLine *line = new PolyLine();

  // Check if the user would like to draw a polygon
  int xdiff = (*(_points->begin())) - (*(_points->end() - 2));
  int ydiff = (*(_points->begin() + 1)) - (*(_points->end() - 1));
  if (xdiff*xdiff + ydiff*ydiff <= CLOSED_MAX_GAP)
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
    while (i < end) {

      // Check whether the poinds in the middle are close enough to the line
      if (check_line(seg_start + 2, i - 2, seg_start, i))
        i += 2;
      else
        break;
    }

    // Check the other direction as well, for first segment of a polygon
    if (line->closed && seg_start == _points->begin()) {
      while (end - 2 > seg_start) {
        if (!check_line(seg_start, i, end - 2, i))
          break;
        if (!check_line(end, _points->end(), end - 2, i))
          break;
        end -= 2;
      }

      // Update the first coordinates
      if (end < _points->end()) {
        int *j = line->points.begin();
        *j = screen_to_coord(*end, origin_x, scaling);
        *(j+1) = screen_to_coord(*(end+1), origin_y, scaling);
      }
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

static bool check_line(int *points_begin, int *points_end, int *l1, int *l2) {
  while (points_begin < points_end) {
    printf("distance_from_line(%d, %d, %d, %d, %d, %d) = %d\n",
           (*points_begin), (*(points_begin+1)),
           (*l1), (*(l1+1)),
           ((*l2) - (*l1)), ((*(l2+1)) - (*(l1+1))),
           distance_from_line
           ((*points_begin) << 16, (*(points_begin+1)) << 16,
            (*l1) << 16, (*(l1+1)) << 16,
            ((*l2) - (*l1)) << 16, ((*(l2+1)) - (*(l1+1))) << 16) >> 16);
    if (distance_from_line
        ((*points_begin) << 16, (*(points_begin+1)) << 16,
         (*l1) << 16, (*(l1+1)) << 16,
         ((*l2) - (*l1)) << 16, ((*(l2+1)) - (*(l1+1))) << 16)
        > (DEVIATION_MAX << 16))
      return false;
    points_begin += 2;
  }
  return true;
}
#endif
