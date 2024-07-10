#pragma once

#include "MaxHeap.hpp"
#include "MinHeap.hpp"

namespace alg {

template <class T, class S> class MedHeap {
public:
  MedHeap();
  template <class It> MedHeap(It begin, It end);

  void insert(T value);
  S median();

  std::string toString();

private:
  size_t mSize;
  MinHeap<T> mTopHeap;
  MaxHeap<T> mBtmHeap;

  void balanceHeaps();
};

template <class T, class S> inline MedHeap<T, S>::MedHeap() : mTopHeap{}, mBtmHeap{} {}

template <class T, class S>
template <class It>
inline MedHeap<T, S>::MedHeap(It begin, It end) : MedHeap{} {
  for (It it = begin; it != end; ++it) {
    insert(*it);
  }
}

template <class T, class S> inline void MedHeap<T, S>::insert(T value) {
  if (mTopHeap.size() == 0 || value > mTopHeap.top()) {
    mTopHeap.insert(value);
  } else {
    mBtmHeap.insert(value);
  }
  balanceHeaps();
  ++mSize;
}

template <class T, class S> inline S MedHeap<T, S>::median() {
  if (mTopHeap.size() > mBtmHeap.size()) {
    return mTopHeap.top();
  } else if (mTopHeap.size() < mBtmHeap.size()) {
    return mBtmHeap.top();
  }
  return static_cast<S>(mTopHeap.top() + mBtmHeap.top()) / 2;
}

template <class T, class S> inline std::string MedHeap<T, S>::toString() {
  std::ostringstream oss;
  auto top = mTopHeap.toLines(TreeStyle::H_BOXED);
  auto bottom = mBtmHeap.toLines(TreeStyle::H_BOXED);
  size_t lineSpan = 0;

  for (auto it = top.rbegin(); it != top.rend(); ++it) {
    oss << *it << '\n';
    if (lineSpan < it->length()) {
      lineSpan = it->length();
    }
  }
  oss << std::string(lineSpan, '-') << '\n';
  for (auto it = bottom.begin(); it != bottom.end(); ++it) {
    oss << *it << '\n';
  }

  return oss.str();
}

template <class T, class S> inline void MedHeap<T, S>::balanceHeaps() {
  auto H = mTopHeap.size();
  auto L = mBtmHeap.size();
  if (std::abs(static_cast<T>(H - L)) > 1) {
    if (H > L) {
      mBtmHeap.insert(mTopHeap.remove());
    } else {
      mTopHeap.insert(mBtmHeap.remove());
    }
  }
}

} // namespace alg
