// $Id: PuzzleGame.hpp,v 1.1.1.1 2000-10-16 17:36:22 jle Exp $

#ifndef __PuzzleGame_h_INCLUDED__
#define __PuzzleGame_h_INCLUDED__

#ifdef __cplusplus

/**
 * 15(n)-puzzle game logic and wrapper for storing game situations.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1.1.1 $
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
  ~PuzzleGame(void);

  /**
   * Checks whether the blocks are in correct order.
   *
   * @return whether blocks are currently in correct order
   */
  bool solved(void);

  /**
   * Moves the specified block to empty square.
   *
   * @param x the x coordinate of the block to move
   * @param y the y coordinate of the block to move
   * @return whether move was possible or not
   */
  bool move(int x, int y);

  /**
   * Get number of the block in the specified position. Returns 0
   * if the position is currently free.
   *
   * @param x the x coordinate of the position
   * @param y the y coordinate of the position
   * @return number of the block or 0 if currently free
   */
  inline int getBlock(int x, int y) {
    return state[y*size + x];
  }
};

#endif

#endif
