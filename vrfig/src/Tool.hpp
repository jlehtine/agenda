// $Id: Tool.hpp,v 1.6 2001-05-23 12:47:50 jle Exp $

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

#ifndef __Tool_hpp_INCLUDED__
#define __Tool_hpp_INCLUDED__

#include "FigureView.hpp"

/**
 * Tools are used for editing figures. They handle the tool specific
 * user interface issues and edit the figure according to user actions.
 * Tools are typically single instance classes. This is the common base
 * class for all tools.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.6 $
 */
class Tool {

protected:

public:

  virtual ~Tool() {}

  /**
   * Returns the (human readable) name of this tool.
   *
   * @return the name of this tool
   */
  virtual const char *get_name() const = 0;
  
  /**
   * Draws the tool icon to the specified rectangle of the current widget.
   * The icon area has already been cleared with the background color.
   *
   * @param x the x coordinate of the rectangle
   * @param y the y coordinate of the rectangle
   * @param w the width of the rectangle
   * @param h the height of the rectangle
   */
  virtual void draw_icon(int x, int y, int w, int h) const {}

  /**
   * Tells that the tool has been activated. The draw method is called
   * separately to update the view.
   *
   * @param view the current state of the view
   */
  virtual void activated(FigureView *view) {}

  /**
   * Tells that the tool has been deactivated. The view will be redrawn
   * so there is no need to restore any changes made to the view.
   *
   * @param view the current state of the view
   */
  virtual void deactivated(FigureView *view) {}

  /**
   * Draws the current state of edit action to the current widget. This
   * method is called when redrawing the whole view. The figure has already
   * been redrawn when this method is called.
   *
   * @param view the current state of the view
   */
  virtual void draw(FigureView *view) {};

  /**
   * Handles the specified event.
   *
   * @param event the event to handle
   * @param view the current state of the view
   * @return whether tool processed the event or not
   */
  virtual int handle(int event, FigureView *view) = 0;
};

#endif
