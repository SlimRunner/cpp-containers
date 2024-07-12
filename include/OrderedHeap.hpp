#pragma once

#include "HeapUtils.hpp"

namespace alg {

template <class T, class Compare = std::less<T>> class OrderedHeap {
public:
  OrderedHeap();
  template <class It> OrderedHeap(It begin, It end);

  void insert(T value);
  T remove();

  T top() const { return mHeap.front(); }
  size_t size() const { return mHeap.size(); }

  std::vector<T>::iterator begin() { return mHeap.begin(); }
  std::vector<T>::iterator end() { return mHeap.end(); }

  std::vector<std::string> toLines(bool rounded = false) const;

private:
  TreePrintStyle mStyle;
  std::vector<T> mHeap;
  Compare mComp;
  void balanceHeap();
  void balanceNode(size_t index);
  void balanceRoot();
  bool hasLeft(size_t index);
  bool hasRight(size_t index);

  void getPrintDiagram(std::vector<size_t> &indices,
                       std::vector<std::vector<bool>> &map) const;
  std::vector<size_t> getLevelOrder() const;
  std::vector<size_t> getPreOrder() const;
  void preOrderHelper(size_t i, std::vector<size_t> &out) const;
};

template <class T, class Compare>
inline OrderedHeap<T, Compare>::OrderedHeap()
    : mStyle{}, mHeap{}, mComp{Compare{}} {}

template <class T, class Compare>
template <class It>
inline OrderedHeap<T, Compare>::OrderedHeap(It begin, It end) : OrderedHeap{} {
  std::copy(begin, end, std::back_inserter(mHeap));
  balanceHeap();
}

template <class T, class Compare>
inline void OrderedHeap<T, Compare>::insert(T value) {
  mHeap.push_back(value);
  balanceNode(mHeap.size() - 1);
}

template <class T, class Compare> inline T OrderedHeap<T, Compare>::remove() {
  const auto value = mHeap.front();
  mHeap.front() = mHeap.back();
  mHeap.pop_back();
  balanceHeap();
  return value;
}

template <class T, class Compare>
inline std::vector<std::string>
OrderedHeap<T, Compare>::toLines(bool rounded) const {
  const size_t HEIGHT =
      static_cast<size_t>(std::ceil(std::log2(mHeap.size() + 1)));
  const size_t HEAP_SIZE = mHeap.size();
  if (!HEAP_SIZE) {
    return std::vector<std::string>{};
  }
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
  std::vector<std::string> output;

  if (rounded) {
    lPipes.at(0) = "╭";
    lPipes.at(1) = "╮";
    lPipes.at(2) = "╰";
  }

  for (auto const &item : mHeap) {
    auto num = (item < 0 ? -item : item);
    size_t offset = static_cast<size_t>(item < 0);
    spacing = static_cast<size_t>(
        std::max<double>(spacing, offset + std::ceil(std::log10(num + 1))));
  }

  switch (mStyle.orientation) {
  // case TreeStyle::H_BOXED:
  // case TreeStyle::H_TABBED:
  case PrintOrientation::HORIZONTAL:
    queue.push(0);
    enqeue(1);

    for (size_t i, lvl = 0; !queue.empty();) {
      i = queue.front();
      queue.pop();
      lvl = static_cast<size_t>(std::ceil(std::log2(i + 2))) - 1;
      oss << std::setw(0) << std::string(spacing * lvl, ' ');
      oss << std::setfill('0') << std::setw(spacing) << std::internal;
      oss << mHeap.at(i);
      output.push_back(oss.str());
      oss.str("");
    }

    break;
  // case TreeStyle::V_BOXED:
  // case TreeStyle::V_SPACED:
  case PrintOrientation::VERTICAL:
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
      output.push_back(oss.str());
      oss.str("");
    }
    break;

  default:
    break;
  }

  return output;
}

template <class T, class Compare>
void inline OrderedHeap<T, Compare>::balanceHeap() {
  std::make_heap(mHeap.begin(), mHeap.end(), mComp);
}

template <class T, class Compare>
inline void OrderedHeap<T, Compare>::balanceNode(size_t index) {
  size_t iNode = index + 1;
  while (true) {
    size_t iSwap = iNode / 2;

    if (iSwap == 0 || mComp(mHeap.at(iNode - 1), mHeap.at(iSwap - 1))) {
      break;
    }

    std::swap(mHeap.at(iNode - 1), mHeap.at(iSwap - 1));
    iNode = iSwap;
  };
}

template <class T, class Compare>
inline void OrderedHeap<T, Compare>::balanceRoot() {
  const size_t HEAP_SIZE = mHeap.size();
  size_t iNode = 1; // Start with the root node

  while (true) {
    size_t iLeft = iNode * 2;
    size_t iRight = iLeft + 1;
    size_t iSwap = iNode; // Assume no swap is needed initially

    if (iLeft <= HEAP_SIZE && mComp(mHeap.at(iSwap - 1), mHeap.at(iLeft - 1))) {
      iSwap = iLeft;
    }

    if (iRight <= HEAP_SIZE &&
        mComp(mHeap.at(iSwap - 1), mHeap.at(iRight - 1))) {
      iSwap = iRight;
    }

    if (iSwap == iNode) {
      break;
    }

    std::swap(mHeap.at(iNode - 1), mHeap.at(iSwap - 1));
    iNode = iSwap; // Move down the heap
  }
}

template <class T, class Compare>
inline bool OrderedHeap<T, Compare>::hasLeft(size_t index) {
  return (index + 1) * 2 >= mHeap.size();
}

template <class T, class Compare>
inline bool OrderedHeap<T, Compare>::hasRight(size_t index) {
  return (index + 1) * 2 + 1 >= mHeap.size();
}

template <class T, class Compare>
inline void OrderedHeap<T, Compare>::getPrintDiagram(
    std::vector<size_t> &indices, std::vector<std::vector<bool>> &map) const {
  const size_t HEIGHT =
      static_cast<size_t>(std::ceil(std::log2(mHeap.size() + 1)));
  const size_t HEAP_SIZE = mHeap.size();
  std::vector<std::vector<bool>> outDiag;
  std::queue<size_t> queue;
  std::function<void(size_t)> enqeue;

  if (!HEAP_SIZE) {
    return;
  }

  size_t levelRows, count;
  size_t termOffset, delta; // terms of linear sequence

  switch (mStyle.orientation) {
  case PrintOrientation::HORIZONTAL:
    indices = getPreOrder();
    for (const auto &i : indices) {
      auto lvl = static_cast<size_t>(std::ceil(std::log2(i + 2))) - 1;
      map.push_back({});
      for (size_t n = 0; n < HEIGHT; ++n) {
        map.back().push_back(n == lvl);
      }
    }
    break;
  case PrintOrientation::VERTICAL:
    delta = 1 << HEIGHT;
    termOffset = (delta >> 1) - 1;
    levelRows = delta - 1; // trim last gap
    indices = getLevelOrder();

    count = 0;
    for (size_t lvl = 0; lvl < HEIGHT; ++lvl) {
      map.push_back({});
      for (size_t n = 0, ni; n < levelRows; ++n) {
        ni = ((n - termOffset) / delta) * delta + termOffset;
        map.back().push_back(n == ni && count < HEAP_SIZE);
        count += map.back().back();
      }
      delta >>= 1;
      termOffset = (delta >> 1) - 1;
    }
    break;

  default:
    break;
  }
}

template <class T, class Compare>
inline std::vector<size_t> OrderedHeap<T, Compare>::getLevelOrder() const {
  std::vector<size_t> out;
  for (size_t i = 0; i < mHeap.size(); ++i) {
    out.push_back(i);
  }
  return out;
}

template <class T, class Compare>
inline std::vector<size_t> OrderedHeap<T, Compare>::getPreOrder() const {
  std::vector<size_t> out{0};
  preOrderHelper(1, out);
  return out;
}

template <class T, class Compare>
inline void
OrderedHeap<T, Compare>::preOrderHelper(size_t i,
                                        std::vector<size_t> &out) const {
  i <<= 1;
  if (i <= mHeap.size()) {
    out.push_back(i - 1);
    preOrderHelper(i, out);
  }
  ++i;
  if (i <= mHeap.size()) {
    out.push_back(i - 1);
    preOrderHelper(i, out);
  }
}

} // namespace alg
