// $Id: ReversiGame.hpp,v 1.1.1.1 2000-11-03 20:18:24 jle Exp $

enum ReversiChipColor { WHITE = 0, BLACK = 1, EMPTY = 2 };


/**
 * Game logic and game state wrapper for Reversi.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1.1.1 $
 */
class ReversiGame {

protected:

  /** Size of the board */
  int size;

  /** Who is the next in turn */
  ReversiChipColor next_to_move;

  /** The current game state */
  ReversiChipColor *board;

  /** Scores of both colors */
  int scores[2];

public:

  /**
   * Constructs a new game initialized to default starting position.
   *
   * @param size the size of the game board
   * @param first_to_move which color makes the first move
   */
  ReversiGame(int size = 8, ReversiChipColor first_to_move = BLACK);

  /**
   * Destructor
   */
  inline ~ReversiGame() {
    delete board;
  }

  /**
   * Returns the chip color who makes the next move.
   *
   * @return the next color to move
   */
  inline ReversiChipColor get_next_to_move() {
    return next_to_move;
  }

  /**
   * Returns score for the given color.
   *
   * @param color the color for which score was requested
   * @return the score for the specified color
   */
  inline int get_score(ReversiChipColor color) {
    return scores[color];
  }

  /**
   * Checks if the given color can make a move.
   *
   * @param color the color to move
   * @return whether the specified color can move
   */
  bool can_move(ReversiChipColor color);

  /**
   * Checks if any further moves are possible.
   *
   * @return whether a move is possible
   */
  inline bool move_possible() {
    return (can_move(next_to_move) || can_move(next_to_move ^ 1));
  }

  /**
   * Make a move. Moves the color given by get_next_to_move().
   * Gives the turn to the next color.
   *
   * @param x the x coordinate of the newly placed chip
   * @param y the y coordinate of the newly placed chip
   * @return whether the move is allowed
   */
  bool move(int x, int y);

  /**
   * Give the turn over to the next color. Can be used to skip moves.
   *
   * @return the next color to move
   */
  inline ReversiChipColor pass_turn() {
    next_to_move = (next_to_move ^ 1);
    return next_to_move;
  }
}
