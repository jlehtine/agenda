// $Id: FigureView.hpp,v 1.3 2001-05-19 08:42:52 jle Exp $

#ifndef __FigureView_hpp_INCLUDED__
#define __FigureView_hpp_INCLUDED__

#include <FL/Fl_Widget.H>
#include "Figure.hpp"
#include "mathutil.hpp"

/** Enumeration for control point drawing style */
enum cp_drawing_style { CP_NONE, CP_NORMAL, CP_SELECTED };

/**
 * A widget which draws the associated figure.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.3 $
 */
class FigureView : public Fl_Widget {

protected:
  
  /** The figure associated with the editor view. */
  Figure *figure;

  /** How the element control points should be drawn */
  cp_drawing_style used_cp_style;

  /** The x coordinate associated with the origin (16.16 fixed) */
  fp16 origin_x;
  
  /** The y coordinate associated with the origin (16.16 fixed) */
  fp16 origin_y;

  /** The scaling (16.16 fixed) */
  u_fp16 scaling;

public:

  /**
   * Constructs a new view and initializes it for default look. No
   * figure is initially associated with the view.
   */
  FigureView(int x, int y, int w, int h);

  /**
   * Sets the figure associated with this widget. Also resets the view origin
   * and scaling.
   *
   * @param figure the figure to be associated with this widget
   */
  void set_figure(Figure *_figure);

  /**
   * Returns the current figure.
   *
   * @return the currently associated figure
   */
  inline Figure *get_figure() {
    return figure; 
  }

  /**
   * Returns the current coordinates for the origin
   *
   * @param x returns the x coordinate (16.16 fixed)
   * @param y returns the y coordinate (16.16 fixed)
   */
  inline void get_origin(fp16 &x, fp16 &y) { 
    x = origin_x; 
    y = origin_y; 
  }

  /**
   * Returns the current x coordinate for the origin.
   *
   * @return the x coordinate (16.16 fixed)
   */
  inline fp16 get_origin_x() {
    return origin_x;
  }

  /**
   * Returns the current y coordinate for the origin.
   *
   * @return the x coordinate (16.16 fixed)
   */
  inline fp16 get_origin_y() {
    return origin_y;
  }

  /**
   * Sets the coordinates for the origin and refreshes the view.
   *
   * @param x the x coordinate (16.16 fixed)
   * @param y the y coordinate (16.16 fixed)
   */
  void set_origin(fp16 x, fp16 y);

  /**
   * Returns the currently used scaling.
   *
   * @return the scaling (16.16 fixed)
   */
  inline u_fp16 get_scaling() { return scaling; }

  /**
   * Sets the new scaling value to be used and refreshes the view.
   *
   * @param _scaling the new scaling
   */
  void set_scaling(u_fp16 _scaling);

  /**
   * Sets how the control points are drawn.
   *
   * @param style the style for drawing control points
   */
  inline void set_cp_drawing_style(cp_drawing_style style) {
    used_cp_style = style;
    redraw();
  }
  
  /**
   * Draws the widget.
   */
  virtual void draw();

  /**
   * Adds a new element to the figure and updates the view accordingly.
   *
   * @param element the element to be added
   */
  void add_element(Element *element);

};

#endif
