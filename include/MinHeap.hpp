#pragma once

#include "HeapUtils.hpp"

namespace alg {

template <class T> class MinHeap {
public:
  MinHeap();
  template <class It> MinHeap(It begin, It end);

  void insert(T value);
  T remove();

  std::vector<T>::iterator begin() { return mHeap.begin(); }
  std::vector<T>::iterator end() { return mHeap.end(); }

  std::string toString(TreeStyle style, bool rounded = false) const;

private:
  std::vector<T> mHeap;
  void balanceHeap();
  void balanceNode(size_t index);
  void balanceRoot();
  bool hasLeft(size_t index);
  bool hasRight(size_t index);
};

template <class T> inline MinHeap<T>::MinHeap() : mHeap{} {}

template <class T>
template <class It>
inline MinHeap<T>::MinHeap(It begin, It end) : MinHeap{} {
  std::copy(begin, end, std::back_inserter(mHeap));
  balanceHeap();
}

template <class T> inline void MinHeap<T>::insert(T value) {
  mHeap.push_back(value);
  balanceNode(mHeap.size() - 1);
}

template <class T> inline T MinHeap<T>::remove() {
  const auto value = mHeap.front();
  mHeap.front() = mHeap.back();
  mHeap.pop_back();
  balanceHeap();
  return value;
}

template <class T>
inline std::string MinHeap<T>::toString(TreeStyle style, bool rounded) const {
  const size_t HEIGHT =
      static_cast<size_t>(std::ceil(std::log2(mHeap.size() + 1)));
  const size_t HEAP_SIZE = mHeap.size();
  std::array<std::string, 3> lPipes = {"┌", "┐", "└"};
  std::array<std::string, 2> endCaps = {"╴", "╶"};
  std::queue<size_t> queue;

  std::function<void(size_t)> enqeue;

  enqeue = [&queue, &enqeue, HEAP_SIZE](size_t i) {
    i <<= 1;
    if (i <= HEAP_SIZE) {
      queue.push(i - 1);
      enqeue(i);
    }
    ++i;
    if (i <= HEAP_SIZE) {
      queue.push(i - 1);
      enqeue(i);
    }
  };

  size_t spacing = 0;
  std::ostringstream oss;

  if (rounded) {
    lPipes.at(0) = "╭";
    lPipes.at(1) = "╮";
    lPipes.at(2) = "╰";
  }

  for (auto const &item : mHeap) {
    if (item >= 0) {
      spacing = static_cast<size_t>(
          std::max<double>(spacing, std::ceil(std::log10(item + 1))));
    } else {
      spacing = static_cast<size_t>(
          std::max<double>(spacing, 1 + std::ceil(std::log10(-item + 1))));
    }
  }

  switch (style) {
  case TreeStyle::H_BOXED:
  case TreeStyle::H_TABBED:
    queue.push(0);
    enqeue(1);

    for (size_t i, lvl = 0; !queue.empty();) {
      i = queue.front();
      queue.pop();
      lvl = static_cast<size_t>(std::ceil(std::log2(i + 2))) - 1;
      oss << std::setw(0) << std::string(spacing * lvl, ' ');
      oss << std::setfill('0') << std::setw(spacing) << std::internal;
      oss << mHeap.at(i) << '\n';
    }

    break;
  case TreeStyle::V_BOXED:
  case TreeStyle::V_SPACED:
    for (size_t lvl = 0, N0 = 0, N1 = 0; lvl < HEIGHT; ++lvl) {
      N1 = N0 + (1 << lvl);
      oss << std::setw(0);
      oss << std::string(spacing * ((1 << (HEIGHT - lvl - 1)) - 1U), ' ');
      for (size_t i = N0; i < N1 && i < HEAP_SIZE; ++i) {
        oss << std::setfill('0') << std::setw(spacing) << std::internal;
        oss << mHeap.at(i);
        oss << std::string(spacing * ((1 << (HEIGHT - lvl)) - 1U), ' ');
      }
      N0 += N1 - N0;
      oss << '\n';
    }
    break;

  default:
    break;
  }

  return oss.str();
}

template <class T> void inline MinHeap<T>::balanceHeap() {
  std::make_heap(mHeap.begin(), mHeap.end(), std::greater<T>{});
}

template <class T> inline void MinHeap<T>::balanceNode(size_t index) {
  bool notSorted = true;
  // indices here are treated as 1-based
  size_t iNode = index + 1, iParent = index + 1;
  do {
    iNode = iParent;
    iParent = iNode / 2;
    notSorted = iParent != 0 && mHeap.at(iNode - 1) < mHeap.at(iParent - 1);
    if (notSorted) {
      std::swap(mHeap.at(iNode - 1), mHeap.at(iParent - 1));
    }
  } while (notSorted);
}

template <class T> inline void MinHeap<T>::balanceRoot() {
  const size_t HEAP_SIZE = mHeap.size();
  bool leftNotSorted = true, rightNotSorted = true;
  // indices here are treated as 1-based
  size_t iNode = 1, iLeft = 1, iRight = 1;

  do {
    iLeft = iNode * 2;
    iRight = iLeft + 1;
    leftNotSorted = iLeft <= HEAP_SIZE && mHeap.at(iNode - 1) > mHeap.at(iLeft - 1);
    rightNotSorted = iRight <= HEAP_SIZE && mHeap.at(iNode - 1) > mHeap.at(iRight - 1);
    if (leftNotSorted && rightNotSorted) {
      if (mHeap.at(iLeft - 1) <= mHeap.at(iRight - 1)) {
        std::swap(mHeap.at(iNode - 1), mHeap.at(iLeft - 1));
        iNode = iLeft;
      } else {
        std::swap(mHeap.at(iNode - 1), mHeap.at(iRight - 1));
        iNode = iRight;
      }
    } else if (leftNotSorted) {
      std::swap(mHeap.at(iNode - 1), mHeap.at(iLeft - 1));
      iNode = iLeft;
    } else if (rightNotSorted) {
      std::swap(mHeap.at(iNode - 1), mHeap.at(iRight - 1));
      iNode = iRight;
    }
  } while (leftNotSorted || rightNotSorted);
}

template <class T> inline bool MinHeap<T>::hasLeft(size_t index) {
  return (index + 1) * 2 >= mHeap.size();
}

template <class T> inline bool MinHeap<T>::hasRight(size_t index) {
  return (index + 1) * 2 + 1 >= mHeap.size();
}

} // namespace alg