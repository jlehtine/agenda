// $Id: PuzzleGame.hpp,v 1.5 2000-10-29 15:57:57 jle Exp $

#ifndef __PuzzleGame_hpp_INCLUDED__
#define __PuzzleGame_hpp_INCLUDED__

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
 * @version $Revision: 1.5 $
 */
class PuzzleGame {

protected:

  /** Size of the current game */
  int size;

  /** The current game situation */
  int *state;
  
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
};

#endif

#endif
