// $Id: PuzzleBox.hpp,v 1.1 2000-10-28 11:23:03 jle Exp $

#ifndef __PuzzleBox_hpp_INCLUDED__
#define __PuzzleBox_hpp_INCLUDED__

#include <stdio.h>
#include <FL/Fl_Box.H>
#include "PuzzleGame.hpp"
#include "SettingsWindow.hpp"

#ifdef __cplusplus

/**
 * Box to render n-puzzle board.
 *
 * @author Johannes Lehtinen <johannes.lehtinen@iki.fi>
 * @version $Revision: 1.1 $
 */
class PuzzleBox : public Fl_Box {
  
protected:
  
  /** The current game to display */
  PuzzleGame *game;

  /** Settings for displaying the board */
  SettingsWindow *settings;

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
