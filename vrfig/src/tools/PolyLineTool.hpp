// $Id: PolyLineTool.hpp,v 1.2 2001-05-20 23:42:14 jle Exp $

#ifndef __PolyLineTool_hpp_INCLUDED__
#define __PolyLineTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "elements/PolyLine.hpp"

/**
 * Tool for drawing polylines and polygons.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class PolyLineTool : public Tool {

protected:

  /** The last polyline drawn or 0 for none */
  PolyLine *last_polyline;

  /** Whether currently drawing a line segment */
  bool drawing_segment;

  /** The x coordinate of the last fixed point */
  int last_x;

  /** The y coordinate of the last fixed point */
  int last_y;

  /** The last end point x coordinate for the segment */
  int seg_x;

  /** The last end point y coordinate for the segment */
  int seg_y;

public:

  PolyLineTool(): last_polyline(0), drawing_segment(false) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
