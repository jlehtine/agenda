// $Id: PuzzleBox.hpp,v 1.2 2000-10-29 15:58:33 jle Exp $

#ifndef __PuzzleBox_hpp_INCLUDED__
#define __PuzzleBox_hpp_INCLUDED__

#include <stdio.h>
#include <FL/Fl_Box.H>
#include "PuzzleGame.hpp"
#include "SettingsWindow.hpp"

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
 * @version $Revision: 1.2 $
 */
class PuzzleBox : public Fl_Box {
  
protected:
  
  /** The current game to display */
  PuzzleGame *game;

  /** Settings for displaying the board */
  SettingsWindow *settings;

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
    settings = NULL;
  }
  
  /**
   * Constructs a new PuzzleBox without any associated game.
   */
  inline PuzzleBox(Fl_Boxtype b, int x, int y, int w, int h, const char *l)
    : Fl_Box(b, x, y, w, h, l) {
    game = NULL;
    settings = NULL;
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
   * Specify the settings to be used. Has to be called before set_game.
   *
   * @param settings to be used
   */
  inline void set_settings(SettingsWindow *settings) {
    this->settings = settings;
  }

  /**
   * Associates new game with this box. The box is redrawn to
   * reflect the change. Method set_settings must be called before
   * first call to set_game.
   *
   * @param game the new game to be associated
   */
  inline void set_game(PuzzleGame *game) {
    this->game = game;
    redraw();
  }
};

#endif

#endif
