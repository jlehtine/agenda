// $Id: ActionBuffer.hpp,v 1.1 2001-05-26 13:02:31 jle Exp $

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

#ifndef __ActionBuffer_hpp_INCLUDED__
#define __ActionBuffer_hpp_INCLUDED__

#include "Action.hpp"

/**
 * Length of action history (number of maximum undo levels). Some optimized
 * code assumes that this is a power of 2.
 */
#define ACTION_BUFFER_LENGTH 32

class ActionBuffer;

/** A callback which is called when the state of an action buffer changes */
typedef void (*action_buffer_callback)(ActionBuffer *buffer, void *data);

/**
 * Maintains a buffer of latest actions to enable undo. Tools operating on
 * a figure (as well as elements themselves) should save all reversible
 * modifications as actions to the ActionBuffer.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class ActionBuffer {

protected:

  /** The actual circular buffer for actions */
  Action *buffer[ACTION_BUFFER_LENGTH];

  /** The index of the next available buffer slot */
  unsigned int buffer_head;

  /** The index of oldest valid action */
  unsigned int buffer_tail;

  /** The number of valid actions in the buffer */
  unsigned int buffer_count;

  /** 
   * A dirty flag which is set every time a new action is inserted or
   * an action is reversed. The flag is typically cleared on save.
   */
  bool dirty;

  /** A callback which is called when ActionBuffer state changes */
  action_buffer_callback callback;

  /** Callback user data */
  void *cb_data;

public:

  /**
   * Constructs a new buffer.
   */
  ActionBuffer(): 
    buffer_head(0), buffer_tail(0), buffer_count(0), dirty(false),
    callback(0) {}

  /** Destructor */
  inline ~ActionBuffer() {
    clear();
  }

  /**
   * Set the callback for this action buffer.
   *
   * @param _callback the callback
   * @param _data user data for the callback
   */
  inline void set_callback(action_buffer_callback _callback, void *_data) {
    callback = _callback;
    cb_data = _data;
  }

  /**
   * Adds the specified action to this action buffer.
   *
   * @param action the new action to insert
   */
  void add_action(Action *action);

  /**
   * Checks whether it is possible to undo an action.
   *
   * @return whether undo is currently possible
   */
  bool can_undo() const;

  /**
   * Undos the latest action.
   *
   * @return whether there was something that could be undone
   */
  bool undo_action();

  /**
   * Clears this undo buffer. This is typically done after loading a new
   * figure.
   */
  void clear();

  /**
   * Checks whether this buffer is dirty.
   *
   * @return whether this buffer is dirty
   */
  inline bool is_dirty() const {
    return dirty;
  }

  /**
   * Clears the dirty flag of this buffer.
   */
  void clear_dirty();
};

#endif
