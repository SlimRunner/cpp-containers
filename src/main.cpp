#include "MaxHeap.hpp"
#include "MinHeap.hpp"
#include "MedHeap.hpp"
#include <array>
#include <cstddef> // size_t
#include <iostream>
#include <iterator>

template <class Iteratable>
void printVec(const Iteratable &container, bool reversed = false) {
  if (reversed) {
    auto begin = std::rbegin(container);
    auto end = std::rend(container);
    for (auto it = begin; it != end; ++it) {
      std::cout << *it << std::endl;
    }
  } else {
    auto begin = std::begin(container);
    auto end = std::end(container);
    for (auto it = begin; it != end; ++it) {
      std::cout << *it << std::endl;
    }
  }
}

int main(int, char **) {
  // std::array values = {5, 3, 8, 1, 9, 4};
  // alg::MinHeap<int> heap(values.begin(), values.end());
  // heap.insert(69);
  // heap.insert(57);
  // heap.insert(-4);
  // heap.insert(-300);
  // std::cout << std::endl;
  // printVec(heap.toLines(alg::TreeStyle::H_BOXED));
  // std::cout << std::endl;

  std::array values = {1,22,16,4,46,29,11,37,7,2};
  // alg::MedHeap<int, float> mheap(values.begin(), values.end());
  // std::cout << std::endl;
  // std::cout << "median: " << mheap.median();
  // std::cout << std::endl;
  // std::cout << mheap.toString();
  // std::cout << std::endl;
  alg::MedHeap<int, float> mheap;
  for (auto const &v : values) {
    mheap.insert(v);
    std::cout << std::endl;
    std::cout << "median: " << mheap.median();
    std::cout << std::endl;
    std::cout << mheap.toString();
    std::cout << std::endl;
  }

  return 0;
}