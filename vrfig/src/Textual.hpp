// $Id: Textual.hpp,v 1.1 2001-05-25 22:30:02 jle Exp $

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

#ifndef __Textual_hpp_INCLUDED__
#define __Textual_hpp_INCLUDED__

/**
 * Textual interface must be implemented by elements which include textual
 * information that can be edited.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class Textual {

public:

  /**
   * Returns the current text of the element.
   *
   * @return the text of the element
   */
  virtual const char *get_text() const = 0;

  /**
   * Sets the text of the element. Does not automatically redraw the
   * element.
   *
   * @param text the new text for the element
   */
  virtual void set_text(const char *text) = 0;

  /**
   * Returns the length of the string.
   *
   * @return the length of the string (in characters)
   */
  virtual size_t get_text_length() const = 0;

  /**
   * Inserts text before the specified position. Does not automatically
   * redraw the element.
   *
   * @param pos the position (starting from 0)
   * @param txt the text to insert
   */
  virtual void insert_text(size_t pos, const char *txt) = 0;

  /**
   * Removes text at the given range. Does not automatically redraw
   * the element.
   *
   * @param pos the position (starting from 0)
   * @param len the length of the range
   */
  virtual void remove_text(size_t pos, size_t len = 1) = 0;

  /**
   * Draws the cursor just before the specified position.
   *
   * @param pos the position (0 being before the first character)
   * @param origin_x the x coordinate of the origin
   * @param origin_y the y coordinate of the origin
   * @param scaling the scaling used
   * @param xorred whether to draw the cursor using xorred mode
   */
  virtual void draw_text_cursor(size_t pos, fp16 origin_x, fp16 origin_y,
                                fp16 scaling, bool xorred=false) const = 0;
};

#endif
