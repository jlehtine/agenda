// $Id: LabelTool.hpp,v 1.1 2001-05-25 22:32:15 jle Exp $

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

#ifndef __LabelTool_hpp_INCLUDED__
#define __LabelTool_hpp_INCLUDED__

#include "Tool.hpp"
#include "Textual.hpp"

class LabelTool : public Tool {

protected:

  /** The current textual element being edited or 0 for none */
  Textual *element;

  /** The current edit position */
  size_t position;

  /**
   * Selects the specified element. Places the cursor at the end of the
   * text by default.
   *
   * @param elem the element to select
   */
  void select(Textual *elem, FigureView *view);

  /**
   * Unselects the current element. Removes the element altogether if it has
   * empty text. Does nothing if no element is selected.
   */
  void unselect(FigureView *view);

  /**
   * Sets cursor position and updates the view accordingly. The graphics
   * context must have been set.
   *
   * @param pos the new cursor position
   * @param view the current view
   */
  void update_position(int pos, FigureView *view);

  /**
   * Draws a xorred text cursor for the current state.
   *
   * @param view the current view
   */
  void draw_xorred_text_cursor(FigureView *view);

  /**
   * Draws the text.
   *
   * @param view the current view
   * @param xorred whether to draw the text in xorred mode
   */
  void draw_text(FigureView *view, bool xorred = false);

public:
  
  inline LabelTool(): element(0) {}

  virtual const char *get_name() const;

  virtual void draw_icon(int x, int y, int w, int h) const;

  virtual void activated(FigureView *view);

  virtual void deactivated(FigureView *view);

  virtual void draw(FigureView *view);

  virtual int handle(int event, FigureView *view);
};

#endif
