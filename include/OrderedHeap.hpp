#pragma once

#include "HeapUtils.hpp"

namespace alg {

template <class T, class Compare = std::less<T>> class OrderedHeap {
public:
  OrderedHeap();
  OrderedHeap(TreePrintStyle style);
  template <class It> OrderedHeap(It begin, It end);
  template <class It> OrderedHeap(It begin, It end, TreePrintStyle style);

  struct IndexEntry {
    size_t index;
    bool enable;
  };

  void insert(T value);
  T remove();

  T top() const { return mHeap.front(); }
  size_t size() const { return mHeap.size(); }

  std::vector<T>::iterator begin() { return mHeap.begin(); }
  std::vector<T>::iterator end() { return mHeap.end(); }

  std::vector<std::string> toLines(bool rounded = false) const;
  std::vector<size_t> getLevelOrderIndices() const;
  std::vector<size_t> getPreOrderIndices() const;
  void getPrintDiagram(std::vector<std::vector<IndexEntry>> &map) const;
  std::vector<T> toVector() const;

private:
  TreePrintStyle mStyle;
  std::vector<T> mHeap;
  Compare mComp;

  void balanceHeap();
  void balanceNode(size_t index);
  void balanceRoot();
  bool hasLeft(size_t index);
  bool hasRight(size_t index);

  void preOrderHelper(size_t i, std::vector<size_t> &out) const;
};

template <class T, class Compare>
inline OrderedHeap<T, Compare>::OrderedHeap()
    : mStyle{}, mHeap{}, mComp{Compare{}} {}

template <class T, class Compare>
inline OrderedHeap<T, Compare>::OrderedHeap(TreePrintStyle style)
    : OrderedHeap{} {
  mStyle = style;
}

template <class T, class Compare>
template <class It>
inline OrderedHeap<T, Compare>::OrderedHeap(It begin, It end) : OrderedHeap{} {
  std::copy(begin, end, std::back_inserter(mHeap));
  balanceHeap();
}

template <class T, class Compare>
template <class It>
inline OrderedHeap<T, Compare>::OrderedHeap(It begin, It end,
                                            TreePrintStyle style)
    : OrderedHeap{begin, end} {
  mStyle = style;
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
inline std::vector<size_t>
OrderedHeap<T, Compare>::getLevelOrderIndices() const {
  std::vector<size_t> out(mHeap.size());
  std::iota(out.begin(), out.end(), 0);
  return out;
}

template <class T, class Compare>
inline std::vector<size_t> OrderedHeap<T, Compare>::getPreOrderIndices() const {
  std::vector<size_t> out;
  preOrderHelper(0, out);
  return out;
}

template <class T, class Compare>
inline std::vector<T> OrderedHeap<T, Compare>::toVector() const {
  std::vector<T> out;
  std::copy(mHeap.cbegin(), mHeap.cend(), std::back_inserter(out));
  return out;
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
    std::vector<std::vector<IndexEntry>> &map) const {
  const size_t HEIGHT =
      static_cast<size_t>(std::ceil(std::log2(mHeap.size() + 1)));
  const size_t HEAP_SIZE = mHeap.size();
  std::vector<size_t> indices;
  std::queue<size_t> queue;
  std::function<void(size_t)> enqeue;

  if (!HEAP_SIZE) {
    return;
  }

  size_t levelRows;
  size_t termOffset, delta; // terms of linear sequence
  std::vector<size_t>::const_iterator idxIt;

  switch (mStyle.orientation) {
  case PrintOrientation::HORIZONTAL:
    indices = getPreOrderIndices();
    for (const auto &i : indices) {
      auto lvl = static_cast<size_t>(std::ceil(std::log2(i + 2))) - 1;
      map.push_back({});
      for (size_t n = 0; n < HEIGHT; ++n) {
        if (n == lvl) {
          map.back().push_back({i, true});
        } else {
          map.back().push_back({0U, false});
        }
      }
    }
    break;
  case PrintOrientation::VERTICAL:
    delta = 1 << HEIGHT;
    termOffset = (delta >> 1) - 1;
    levelRows = delta - 1; // trim last gap
    indices = getLevelOrderIndices();
    idxIt = indices.cbegin();

    for (size_t lvl = 0; lvl < HEIGHT; ++lvl) {
      map.push_back({});
      for (size_t n = 0, ni; n < levelRows; ++n) {
        ni = ((n - termOffset) / delta) * delta + termOffset;
        if (n == ni && idxIt != indices.end()) {
          map.back().push_back({*idxIt, true});
          idxIt++;
        } else {
          map.back().push_back({0U, false});
        }
      }
      delta >>= 1;
      termOffset = (delta >> 1) - 1;
    }
    break;

  default:
    break;
  }

  if (mStyle.order == PrintOrder::BOTTOM_UP) {
    std::reverse(map.begin(), map.end());
  }
  if (mStyle.levelOrder == LevelOrder::RIGHT_TO_LEFT) {
    for (auto && line: map) {
      std::reverse(line.begin(), line.end());
    }
  }
}

template <class T, class Compare>
inline void
OrderedHeap<T, Compare>::preOrderHelper(size_t i,
                                        std::vector<size_t> &out) const {
  if (i >= mHeap.size()) {
    return;
  }
  out.push_back(i);
  i <<= 1;
  preOrderHelper(++i, out);
  preOrderHelper(++i, out);
}

} // namespace alg
