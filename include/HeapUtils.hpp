#pragma once

#include <algorithm> // copy, make_heap
#include <array>
#include <cmath>      // log10, log2, ceil
#include <cstddef>    // size_t
#include <deque>      // as container
#include <functional> // lambda with recursion
#include <iomanip>
#include <iterator> // back_inserter
#include <queue>
#include <sstream>
#include <string>
#include <utility> // swap
#include <vector>

namespace alg {

enum class PrintOrientation {
  HORIZONTAL, // levels are printed left-to-right or vice versa
  VERTICAL,   // levels are printed top-to-bottom or viceversa
};

enum class PrintOrder {
  TOP_DOWN,  // first print the root and go down the tree
  BOTTOM_UP, // otherwise
};

enum class LevelOrder {
  LEFT_TO_RIGHT, // tree is printed left-to-right
  RIGHT_TO_LEFT, // otherwise
};

enum class LevelSpacing {
  SPACED,              // levels are tabulated with spaces
  BOX_DRAWING,         // levels are connected with box-drawing characters
  BOX_DRAWING_ROUNDED, // same as box-drawing but with rounded corners
  CUSTOM,              // levels are connected with a custom set of connectors
};

// box-drawing set is:
// {─, │, ┌, ┐, └, ┘, ├, ┤, ┬, ┴, ┼, ╴, ╵, ╶, ╷}
enum class DrawingSet {
  horzPipe,
  vertPipe,
  seTurn,
  swTurn,
  neTurn,
  nwTurn,
  leftTee,
  rightTee,
  bottomTee,
  topTee,
  intersection,
  horzPipeHalfLeft,
  vertPipeHalfTop,
  horzPipeHalfRight,
  vertPipeHalfBottom,
};

enum class TreeDensity {
  NORMAL, // connectors run in their own separate lines
  COMPACT // each row/column contains a level
};

enum class DataPadding {
  LEFT,
  RIGHT,
  INTERNAL, // used for padding negative numbers with 0s
  CENTER,
};

struct TreePrintStyle {
  PrintOrientation orientation;
  PrintOrder order;
  LevelOrder levelOrder;
  LevelSpacing spacing;
  TreeDensity density;
  DataPadding padding;
  std::array<char, 15> drawingSet;
  char padChar = ' ';
};

enum class TreeStyle {
  V_SPACED,
  V_BOXED,
  H_TABBED,
  H_BOXED,
};

} // namespace alg
