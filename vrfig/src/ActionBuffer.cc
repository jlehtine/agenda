// $Id: ActionBuffer.cc,v 1.1 2001-05-26 13:02:31 jle Exp $


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

#include "ActionBuffer.hpp"


void ActionBuffer::add_action(Action *action) {
    
  // Check if we need to cleanup the oldest action
  if (buffer_count >= ACTION_BUFFER_LENGTH) {
    Action *old = buffer[buffer_tail];
    delete old;
    buffer_tail++;
    buffer_tail &= ACTION_BUFFER_LENGTH - 1;
    buffer_count--;
  }
  
  // Insert the new action
  buffer[buffer_head] = action;
  buffer_head++;
  buffer_head &= ACTION_BUFFER_LENGTH - 1;
  buffer_count++;
  
  // Mark the buffer as dirty
  dirty = true;
  if (callback)
    callback(this, cb_data);
}

bool ActionBuffer::can_undo() const {
  if (buffer_count == 0)
    return false;
  unsigned int ai = (buffer_head - 1) & (ACTION_BUFFER_LENGTH - 1);
  return buffer[ai]->isReversible();
}

bool ActionBuffer::undo_action() {
  if (!can_undo())
    return false;
  unsigned int ai = (buffer_head - 1) & (ACTION_BUFFER_LENGTH - 1);
  buffer[ai]->undo();
  delete buffer[ai];
  buffer_head = ai;
  buffer_count--;
  dirty = true;
  if (callback)
    callback(this, cb_data);
  return true;
}

void ActionBuffer::clear() {
  while (buffer_count > 0) {
    delete buffer[buffer_tail];
    buffer_tail++;
    buffer_tail &= ACTION_BUFFER_LENGTH - 1;
    buffer_count--;
  }
  if (callback)
    callback(this, cb_data);
}

void ActionBuffer::clear_dirty() {
  if (dirty) {
    dirty = false;
    if (callback)
      callback(this, cb_data);
  }
}
