// $Id: FigureView.hpp,v 1.6 2001-05-26 14:25:19 jle Exp $

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

#ifndef __FigureView_hpp_INCLUDED__
#define __FigureView_hpp_INCLUDED__

#include <FL/Fl_Widget.H>
#include "Figure.hpp"
#include "ActionBuffer.hpp"
#include "mathutil.hpp"

/** Enumeration for control point drawing style */
enum cp_drawing_style { CP_NONE, CP_NORMAL, CP_SELECTED };

/**
 * A widget which draws the associated figure.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.6 $
 */
class FigureView : public Fl_Widget {

protected:
  
  /** The figure associated with the editor view. */
  Figure *figure;

  /** The x coordinate associated with the origin (16.16 fixed) */
  fp16 origin_x;
  
  /** The y coordinate associated with the origin (16.16 fixed) */
  fp16 origin_y;

  /** The scaling (16.16 fixed) */
  u_fp16 scaling;

  /** The action buffer associated with the figure */
  ActionBuffer *action_buffer;

public:

  /**
   * Constructs a new view and initializes it for default look. No
   * figure is initially associated with the view.
   */
  FigureView(int x, int y, int w, int h, ActionBuffer *abuffer);

  virtual ~FigureView() {};

  /**
   * Sets the figure associated with this widget. Also resets the view origin
   * and scaling.
   *
   * @param figure the figure to be associated with this widget
   */
  virtual void set_figure(Figure *_figure);

  /**
   * Returns the current figure.
   *
   * @return the currently associated figure
   */
  inline Figure *get_figure() {
    return figure; 
  }

  /**
   * Returns the action buffer associated with the view.
   *
   * @return the associated action buffer
   */
  inline ActionBuffer *get_action_buffer() {
    return action_buffer;
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
