#pragma once

#include "OrderedHeap.hpp"

namespace alg {

template <class T> using MinHeap = OrderedHeap<T, std::greater<T>>;

} // namespace alg