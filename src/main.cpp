#include "MinHeap.hpp"
#include "MaxHeap.hpp"
#include <cstddef> // size_t
#include <iostream>
#include <array>

int main(int, char **) {
  std::array values = {5,3,8,1,9,4};
  alg::MaxHeap<int> heap(values.begin(), values.end());
  heap.insert(69);
  heap.insert(57);
  heap.insert(-4);
  heap.insert(-300);

  std::cout << std::endl;
  std::cout << heap.toString(alg::TreeStyle::H_BOXED);
  // std::cout << heap.toString(alg::TreeStyle::V_BOXED);
  std::cout << std::endl;

// #if _WIN32
// #endif
  return 0;
}