#include "../common.h"
#include <array>
#include <vector>

const size_t WIDTH = 320;
const size_t HEIGHT = 320;
const size_t gameSize_ = WIDTH * HEIGHT;

struct GameOfLife {

  std::array<uint8_t, gameSize_> gameMatrix_;
  std::array<uint8_t, gameSize_> updatedGameMatrix_;

  void init() {
    for (auto i = 0; i < gameSize_; i++) {
      gameMatrix_[i] = (rand() % 2);
    }
    for (auto i = 0; i < gameSize_; i++) {
      updatedGameMatrix_[i] = 0;
    }
  }

  int checkNeighbours(size_t idx) {
    auto neighbours = 0;
    size_t currIdx = idx - WIDTH - 1;
    for (auto i = 0; i < 3; i++) {
      try {
        if (gameMatrix_.at(currIdx) != 0) {
          neighbours++;
        }
      } catch (std::out_of_range const &exc) {
      }
      currIdx += 1;
    }
    try {
      if (gameMatrix_.at(idx - 1) != 0) {
        neighbours++;
      }
    } catch (std::out_of_range const &exc) {
    }

    try {
      if (gameMatrix_.at(idx + 1) != 0) {
        neighbours++;
      }
    } catch (std::out_of_range const &exc) {
    }

    currIdx = idx + WIDTH - 1;
    for (auto i = 0; i < 3; i++) {
      try {
        if (gameMatrix_.at(currIdx) != 0) {
          neighbours++;
        }
      } catch (std::out_of_range const &exc) {
      }
      currIdx += 1;
    }
    return neighbours;
  }
  void update() {
    for (auto idx = 0; idx < gameSize_; idx++) {
      auto neighbours = checkNeighbours(idx);
      // Cell is currently dead
      if (gameMatrix_[idx] == 0) {
        // Cell becomes alive if it has exactly 3 neighbours
        if (neighbours == 3) {
          updatedGameMatrix_[idx] = 1;
        }
      }
      // Cell is currently alive
      else {
        // If under 2 or over 3 neighbours, dies
        if (neighbours < 2 or neighbours > 3) {
          updatedGameMatrix_[idx] = 0;
        }
      }
    }
    gameMatrix_ = updatedGameMatrix_;
  }
};