#pragma once

#include "OrderedHeap.hpp"

namespace alg {

template <class T>
using MaxHeap = OrderedHeap<T, std::less<T>>;

} // namespace alg