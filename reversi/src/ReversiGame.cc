// $Id: ReversiGame.cc,v 1.2 2000-11-03 21:04:50 jle Exp $

#include "ReversiGame.hpp"

ReversiGame::ReversiGame(int size, ReversiChipColor first_to_move) {
  this->size = size;
  this->next_to_move = first_to_move;

  board = new ReversiChipColor[size*size];
  int i;
  for (i = 0; i < size*size; i++) {
    board[i] = EMPTY;
  }

  int half = size/2 - 1;
  set_chip(half, half, BLACK);
  set_chip(half+1, half, WHITE);
  set_chip(half+1, half, WHITE);
  set_chip(half+1, half+1, BLACK);
}

bool ReversiGame::can_move(ReversiChipColor color) {
  int x, y;
  for (y = 0; y < size; y++) {
    for (x = 0; x < size; x++) {
      if (get_chip(x, y) == color &&
          can_move_chip(x, y)) {
        return true;
      }
    }
  }
  return false;
}

bool ReversiGame::move(int x, int y) {

  // Check that the target position is empty
  if (get_chip(x, y) != EMPTY) {
    return false;
  }

  // Check all eight possible directions for legal jump
  int gain = 0;
  int dx, dy;
  for (dy = -1; dy <= 1; dy++) {
    for (dx = -1; dx <= 1; dx++) {
      int l = 1;
      while (x + l*dx >= 0 && x + l*dx < size &&
             y + l*dy >= 0 && y + l*dy < size) {

        // Check if found deadend
        if (get_chip(x + l*dx, y + l*dy) == EMPTY) {
          break;

        // Check if found match
        } else if (get_chip(x + l*dx, y+l*dy) == next_to_move) {
          int ll;
          for (ll = 1; ll < l; ll++) {
            set_chip(x + ll*dx, y + ll*dy, next_to_move);
          }
          gain += l-1;
          break;
        }

        l++;
      }
    }
  }

  // Check if we could really jump to the position
  if (gain == 0) {
    return false;
  }

  // Occupy the position and update scores
  set_chip(x, y, next_to_move);
  scores[next_to_move] += (gain + 1);
  scores[opponent(next_to_move)] -= gain;

  // Hand over the turn
  pass_turn();
  return true;
}

bool ReversiGame::can_move_chip(int x, int y) {

  // Check all eight directions for possible jumps
  ReversiChipColor color = get_chip(x, y);
  int dx, dy;
  for (dy = -1; dy <= 1; dy++) {
    for (dx = -1; dx <= 1; dx++) {
      int l = 1;
      while (x + l*dx >= 0 && x + l*dx < size &&
             y + l*dy >= 0 && y + l*dy < size) {

        // Check if deadend
        if (get_chip(x + l*dx, y + l*dy) == color) {
          break;

        // Check if possible move
        } else if (l > 1 && get_chip(x + l*dx, y + l*dy) == EMPTY) {
          return true;
        }

        l++;
      }
    }
  }
  return false;
}
