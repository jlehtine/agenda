// $Id: PuzzleBox.cc,v 1.2 2000-10-28 11:57:05 jle Exp $

#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PuzzleBox.hpp"

void PuzzleBox::draw() {
  Fl_Box::draw();
  if (game == NULL || settings == NULL) {
    return;
  }

  // Select tile size
  int size = game->get_size();
  int ww = w();
  int wh = h();
  int smaller_dim = (ww < wh ? ww : wh);
  int tile_size = smaller_dim / size;
  
  // Calculate adjustment
  int offx = x() + (ww - tile_size*size)/2;
  int offy = y() + (wh - tile_size*size)/2;

  // Draw the tile borders
  fl_font(FL_HELVETICA | FL_BOLD, 14);
  bool use_numbers = settings->use_numbers();
  int x, y;
  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++) {
      int t = game->get_tile(x, y);
      if (t != 0) {

        // Draw tile border
        bool dark = ((((t-1)%size + (t-1)/size) & 1) != 0);
        if (dark) {
          fl_color(FL_GRAY);
        } else {
          fl_color(FL_WHITE);
        }
        fl_yxline(offx + tile_size*x, offy + tile_size*(y+1) - 1,
                  offy + tile_size*y,
                  offx + tile_size*(x+1) - 1);
        fl_yxline(offx + tile_size*x + 1, offy + tile_size*(y+1) - 2,
                  offy + tile_size*y + 1,
                  offx + tile_size*(x+1) - 2);
        fl_color(FL_BLACK);
        fl_xyline(offx + tile_size*x, offy + tile_size*(y+1) - 1,
                  offx + tile_size*(x+1) - 1,
                  offy + tile_size*y);
        fl_xyline(offx + tile_size*x + 1, offy + tile_size*(y+1) - 2,
                  offx + tile_size*(x+1) - 2,
                  offy + tile_size*y + 1);
        
        // Fill tile box
        if (dark) {
          fl_color(FL_BLACK);
        } else {
          fl_color(FL_GRAY);
        }
        fl_rectf(offx + tile_size*x + 2, offy + tile_size*y + 2,
                 tile_size - 4, tile_size - 4);

        // Draw tile letter/number
        if (dark) {
          fl_color(FL_WHITE);
        } else {
          fl_color(FL_BLACK);
        }
        char str[3];
        if (use_numbers) {
          sprintf(str, "%d", t);
        } else {
          str[0] = 'A' + t - 1;
          str[1] = '\0';
        }
        fl_draw(str, offx + tile_size*x, offy + tile_size*y,
                tile_size, tile_size, FL_ALIGN_CENTER);
      } else {

        // Clear box
        fl_color(FL_WHITE);
        fl_rectf(offx + tile_size*x, offy + tile_size*y,
                 tile_size, tile_size);
      }
    }
  }
}
