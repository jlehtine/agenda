// $Id: Action.hpp,v 1.2 2001-05-26 14:25:19 jle Exp $

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

#ifndef __Action_hpp_INCLUDED__
#define __Action_hpp_INCLUDED__

/**
 * An interface for buffered actions performed on a figure. The tools and
 * other entities actually providing the functionality should subclass this
 * interface and use it to update the ActionBuffer object.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.2 $
 */
class Action {

public:

  /**
   * Destructor for the action object. Notice that action objects are deleted
   * when the effects become permanent _OR_ when they have been undone.
   */
  virtual ~Action() {};

  /**
   * Returns whether this action is reversible (undoable). The default
   * implementation returns true but tools providing irreversible operations
   * should put irreversible action objects to the buffer.
   *
   * @returns whether this action is reversible
   */
  virtual bool isReversible() const;

  /**
   * Undos the action. This method is called when the action is undone.
   */
  virtual void undo() = 0;

  /**
   * Commits the action record making the effects permanent. This method
   * is called when the action is thrown out of the buffer therefore
   * committing to the action effects.
   */
  virtual void commit() {}
};

#endif
