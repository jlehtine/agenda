// $Id: PuzzleBox.hpp,v 1.5 2001-01-28 15:49:20 jle Exp $

/*
* NPuzzle
* Copyright (C) 2000 Johannes Lehtinen
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* The license can be found in the package directory in file COPYING.
*/

#ifndef __PuzzleBox_hpp_INCLUDED__
#define __PuzzleBox_hpp_INCLUDED__

#include <stdio.h>
#include <FL/Fl_Box.H>
#include "PuzzleGame.hpp"

#ifdef __cplusplus

typedef struct TileLayout TileLayout;

/**
 * Defines the layout for the tile set.
 */
struct TileLayout {
  
  /** Length of a tile edge */
  int edge;

  /** Offset of the tile set from the left edge of the box */
  int x_offset;

  /** Offset of the tile set from the top edge of the box */
  int y_offset;
};

/**
 * Box to render n-puzzle board.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.5 $
 */
class PuzzleBox : public Fl_Box {
  
protected:
  
  /** The current game to display */
  PuzzleGame *game;

  /** Whether to use alphabets (instead of numbers) */
  bool use_alphabets;

  /** Callback for moves */
  void (*move_callback)();

  /**
   * Calculates the size of tiles and offsets of tile box according to
   * the current box size.
   *
   * @param layout Filled with values describing the layout
   */
  void layout(TileLayout *l);

  /**
   * Draws a tile to the specified position.
   *
   * @param size the size of the game board
   * @param x the x coordinate (in pixels) of tile
   * @param y the y coordinate (in pixels) of tile
   * @edge the length of the edge
   * @param tile the number of tile
   */
  void draw_tile(int size, int x, int y, int edge, int tile);

public:

  /**
   * Constructs a new PuzzleBox without any associated game.
   */
  inline PuzzleBox(int x, int y, int w, int h, const char *l = 0)
    : Fl_Box(x, y, w, h, l) {
    game = NULL;
    use_alphabets = false;
    move_callback = NULL;
  }
  
  /**
   * Constructs a new PuzzleBox without any associated game.
   */
  inline PuzzleBox(Fl_Boxtype b, int x, int y, int w, int h, const char *l)
    : Fl_Box(b, x, y, w, h, l) {
    game = NULL;
    use_alphabets = false;
    move_callback = NULL;
  }

  /**
   * Extends the draw method in Fl_Box.
   */
  virtual void draw();

  /**
   * Extends the event handling method in Fl_Box.
   */
  virtual int handle(int event);

  /**
   * Associates new game with this box. The box is redrawn to
   * reflect the change.
   *
   * @param game the new game to be associated
   */
  inline void set_game(PuzzleGame *game) {
    this->game = game;
    redraw();
  }

  /**
   * Sets whether to use alphabets or not.
   *
   * @param flag whether to use alphabets
   */
  inline void set_alphabets(bool flag) {
    use_alphabets = flag;
    redraw();
  }

  /**
   * Sets the callback function to be called when move is made.
   *
   * @param cb the callback function to call
   */
  inline void set_move_callback(void (*cb)()) {
    move_callback = cb;
  }
};

#endif

#endif
