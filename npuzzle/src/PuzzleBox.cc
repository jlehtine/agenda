// $Id: PuzzleBox.cc,v 1.4 2000-10-29 17:22:14 jle Exp $

#include <stdio.h>
#include <FL/fl_draw.H>
#include <FL/Enumerations.H>
#include "PuzzleBox.hpp"

void PuzzleBox::draw() {

  if (game == NULL || settings == NULL) {
    return;
  }

  // Calculate layout
  TileLayout l;
  layout(&l);
  int edge = l.edge;
  int offx = l.x_offset;
  int offy = l.y_offset;
  int size = game->get_size();

  // Clear left over borders with background color
  fl_color(color());
  if (offx > x()) {
    fl_rectf(x(), y(), offx - x(), h());
  }
  if (offx + edge*size < x() + w()) {
    fl_rectf(offx + edge*size, y(), 
             x() + w() - offx - edge*size, h());
  }
  if (offy > y()) {
    fl_rectf(offx, y(), edge*size, offy - y());
  }
  if (offy + edge*size < y() + h()) {
    fl_rectf(offx, offy + edge*size,
             edge*size, y() + h() - offy - edge*size);
  }

  // Draw the tile borders
  int x, y;
  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++) {
      int t = game->get_tile(x, y);
      if (t != 0) {
        
        // Draw tile
        draw_tile(size, offx + x*edge, offy + y*edge, edge, t);
      } else {

        // Clear box
        fl_color(color());
        fl_rectf(offx + edge*x, offy + edge*y, edge, edge);
      }
    }
  }
}

int PuzzleBox::handle(int event) {

  // Ignore if not mouse push/drag/release or if not initialized yet
  if (game == NULL || settings == NULL || event != FL_PUSH) {
    return Fl_Box::handle(event);
  }

  // Get the position of click and map it to tile position
  int px = Fl::event_x();
  int py = Fl::event_y();
  TileLayout l;
  layout(&l);
  int x = (px - l.x_offset) / l.edge;
  int y = (py - l.y_offset) / l.edge;

  // Check if position outside the tile set
  int size = game->get_size();
  if (x < 0 || x >= size || y < 0 || y >= size) {
    return 0;
  }
  
  // Try to move the tile
  int move = game->move(x, y);
  if (move != MOVE_NONE) {
    
    // Update view
    window()->make_current();
    int old_pos[2] = { x, y };
    int new_pos[2] = { x, y };
    new_pos[MOVE_AXEL(move)] += MOVE_OFFSET(move);
    draw_tile(size, 
              l.x_offset + new_pos[0]*l.edge, 
              l.y_offset + new_pos[1]*l.edge,
              l.edge, game->get_tile(new_pos[0], new_pos[1]));
    fl_color(color());
    fl_rectf(l.x_offset + old_pos[0]*l.edge, 
             l.y_offset + old_pos[1]*l.edge, 
             l.edge, l.edge);
  }
  return 1;
}

void PuzzleBox::layout(TileLayout *l) {
  int size = game->get_size();
  int ww = w();
  int wh = h();
  int smaller_dim = (ww < wh ? ww : wh);
  l->edge = smaller_dim / size;
  l->x_offset = x() + (ww - (l->edge)*size)/2;
  l->y_offset = y() + (wh - (l->edge)*size)/2;
}

void PuzzleBox::draw_tile(int size, int x, int y, int edge, int tile) {

  // Draw tile border
  bool dark = ((((tile-1)%size + (tile-1)/size) & 1) != 0);
  if (dark) {
    fl_color(FL_GRAY);
  } else {
    fl_color(FL_WHITE);
  }
  fl_yxline(x, y + edge - 1, y, x + edge - 1);
  fl_yxline(x + 1, y + edge - 2, y + 1, x + edge - 2);
  fl_color(FL_BLACK);
  fl_xyline(x, y + edge - 1, x + edge - 1, y);
  fl_xyline(x + 1, y + edge - 2, x + edge - 2, y + 1);
  
  // Fill tile box
  if (dark) {
    fl_color(FL_BLACK);
  } else {
    fl_color(FL_GRAY);
  }
  fl_rectf(x + 2, y + 2, edge - 4, edge - 4);
  
  // Draw tile letter/number
  if (dark) {
    fl_color(FL_WHITE);
  } else {
    fl_color(FL_BLACK);
  }
  char str[3];
  if (settings->use_numbers()) {
    sprintf(str, "%d", tile);
  } else {
    str[0] = 'A' + tile - 1;
    str[1] = '\0';
  }
  fl_font(FL_HELVETICA | FL_BOLD, 14);
  fl_draw(str, x, y, edge, edge, FL_ALIGN_CENTER);
}
