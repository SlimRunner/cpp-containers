#pragma once

#include <algorithm> // copy, make_heap
#include <array>
#include <cmath> // log10, log2, ceil
#include <cstddef> // size_t
#include <deque> // as container
#include <functional> // lambda with recursion
#include <iomanip>
#include <iterator> // back_inserter
#include <queue>
#include <sstream>
#include <string>
#include <utility> // swap
#include <vector>

namespace alg {
enum class TreeStyle {
  V_SPACED,
  V_BOXED,
  H_TABBED,
  H_BOXED,
};
} // namespace alg
