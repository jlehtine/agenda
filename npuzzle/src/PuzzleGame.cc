// $Id: PuzzleGame.cc,v 1.3 2000-10-28 11:22:26 jle Exp $

#include <stdlib.h>
#include "PuzzleGame.hpp"

PuzzleGame::PuzzleGame(int size) {
  this->size = size;
  state = new int[size*size];

  // Initialize state
  int i;
  for (i = 0; i < size*size-1; i++) {
    state[i] = i+1;
  }
  state[size*size-1] = 0;
  
  // Scramble blocks
  int rounds = size*size*100;
  int zx = size-1;
  int zy = size-1;
  while (rounds-- > 0) {

    int axel, off;

    // Check which directions are possible
    int numAvailDirs = 0;
    bool availDirs[4];
    int i;
    for (i = 0; i < 4; i++) {
      axel = (i < 2 ? 0 : 1);
      off = 2*(i&1) - 1;
      availDirs[i] = (axel == 0 ?
                      (zx + off >= 0 && zx + off < size) :
                      (zy + off >= 0 && zy + off < size));
      if (availDirs[i]) {
        numAvailDirs++;
      }
    }

    // Select random direction
    //
    // NOTE: Modulo is not the best way to get random numbers but
    // NOTE: the target platform does not have HW floating point ops
    int r = rand() % numAvailDirs;
    for (i = 0; r >= 0; i++) {
      if (availDirs[i]) {
        r--;
      }
    }
    i--;
    axel = (i < 2 ? 0 : 1);
    off = 2*(i&1) - 1;
    int x = zx + (axel == 0 ? off : 0);
    int y = zy + (axel == 0 ? 0 : off);
    state[zy*size + zx] = state[y*size + x];
    state[y*size + x] = 0;
    zx = x;
    zy = y;
  }
}

PuzzleGame::~PuzzleGame() {
  delete state;
}

bool PuzzleGame::solved() {
  int i = 0;
  for (i = 0; i < size*size - 1; i++) {
    if (state[i] != i+1) {
      return false;
    }
  }
  return true;
}

bool PuzzleGame::move(int x, int y) {
  int bxy = state[y*size + x];

  // Check that we are not trying to move an empty block
  if (bxy == 0) {
    return false;
  }
  
  // Find the neighbouring empty block if any
  bool found = false;
  int offx, offy;
  for (offy = -1; offy <= 1 && !found; offy += 2) {
    for (offx = -1; offx <= 1 && !found; offy += 2) {
      if (x + offx >= 0 && x + offx < size &&
          y + offy >= 0 && y + offy < size) {
        if (state[(y+offy)*size + x + offx] == 0) {
          found = true;
        }
      }
    }
  }
  if (!found) {
    return false;
  }

  // Move the block
  state[(y+offy)*size + x + offx] = bxy;
  state[y*size + x] = 0;
  return true;
}
