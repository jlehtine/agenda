// $Id: Movable.hpp,v 1.1 2001-05-19 16:54:22 jle Exp $

#ifndef __Movable_hpp_INCLUDED__
#define __Movable_hpp_INCLUDED__

/**
 * An interface for elements that can be moved.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class Movable {

public:

  /**
   * Moves the element according to the specified offset.
   *
   * @param xoff the x offset
   * @param yoff the y offset
   */
  virtual void move(fp16 xoff, fp16 yoff) = 0;
};

#endif
