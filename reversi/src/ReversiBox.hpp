// $Id: ReversiBox.hpp,v 1.1 2004-11-10 14:00:21 jle Exp $

#ifndef __ReversiBox_hpp_INCLUDED__
#define __ReversiBox_hpp_INCLUDED__

#include <FL/Fl_Box.H>
#include "ReversiGame.hpp"

class ReversiBox : public Fl_Box {

protected:

  /** The associated game situation or NULL if none. */
  ReversiGame *game;

public:

  /**
   * Constructs a new box for drawing Reversi board. No game is initially
   * associated with the new box.
   */
  inline ReversiBox(int x, int y, int w, int h, const char *l = 0)
    : Fl_Box(x, y, w, h, l) {
    game = NULL;
  }

  /**
   * Constructs a new box for drawing Reversi board. No game is initially
   * associated with the new box.
   */
  inline ReversiBox(Fl_Boxtype b, int x, int y, int w, int h, const char *l)
    : Fl_Box(b, x, y, w, h, l) {
    game = NULL;
  }
};

#endif
