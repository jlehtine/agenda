// $Id: ReversiGame.cc,v 1.1 2000-11-03 20:35:30 jle Exp $

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
