// $Id: PuzzleGame.hpp,v 1.7 2000-10-31 19:52:42 jle Exp $

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

#ifndef __PuzzleGame_hpp_INCLUDED__
#define __PuzzleGame_hpp_INCLUDED__

#include "time.h"

#ifdef __cplusplus

/** Indicates that no move was possible */
#define MOVE_NONE 4

/** Get axel of move (0=x, 1=y) */
#define MOVE_AXEL(x) ((x)&1)

/** Get offset of move (-1, 1) */
#define MOVE_OFFSET(x) (((x)&2) - 1)

/**
 * 15(n)-puzzle game logic and wrapper for storing game situations.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.7 $
 */
class PuzzleGame {

protected:

  /** Size of the current game */
  int size;

  /** The current game situation */
  int *state;

  /** When the game was started */
  time_t start_time;

  /** When the puzzle was solved. 0 if not solved yet. */
  time_t solved_time;
  
  /** Number of moves made */
  int moves;
  
public:
  
  /**
   * Constructs a new <code>PuzzleGame</code> with the specified
   * parameters. The order of blocks is initially scrambled.
   *
   * @param size the size of the board (3 gives 3x3 board)
   */
  PuzzleGame(int size);

  /**
   * Destructor freeing all allocated resources.
   */
  inline ~PuzzleGame() {
    delete state;
  }

  /**
   * Checks whether the blocks are in correct order.
   *
   * @return whether blocks are currently in correct order
   */
  bool solved();

  /**
   * Moves the specified block to empty square.
   *
   * @param x the x coordinate of the block to move
   * @param y the y coordinate of the block to move
   * @return direction of move or MOVE_NONE if not possible to move
   */
  int move(int x, int y);

  /**
   * Get number of the tile in the specified position. Returns 0
   * if the position is currently free.
   *
   * @param x the x coordinate of the position
   * @param y the y coordinate of the position
   * @return number of the tile or 0 if currently free
   */
  inline int get_tile(int x, int y) {
    return state[y*size + x];
  }

  /**
   * Returns the size of the game.
   *
   * @returns the size of the game
   */
  inline int get_size() {
    return size;
  }

  /**
   * Returns the current game time.
   *
   * @returns the current game time in seconds
   */
  inline int get_time() {
    if (solved_time == 0) {
      return time(NULL) - start_time;
    } else {
      return solved_time - start_time;
    }
  }

  /**
   * Returns the number of moves made.
   *
   * @returns the number of moves made
   */
  inline int get_moves() {
    return moves;
  }
};

#endif

#endif
