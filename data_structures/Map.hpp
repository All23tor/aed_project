#pragma once

#include "Pair.hpp"
#include "Vector.hpp"

template <typename K, typename V>
class SimpleMap {
private:
  Vector<Pair<K, V>> data;

  int lower_bound(const K& key) const {
    int l = 0, r = data.size();
    while (l < r) {
      int m = (l + r) / 2;
      if (data[m].first < key)
        l = m + 1;
      else
        r = m;
    }
    return l;
  }

public:
  void insert(const K& key, const V& value) {
    int pos = lower_bound(key);
    if (pos < data.size() && data[pos].first == key)
      data[pos].second = value;
    else
      data.insert(pos, {key, value});
  }

  V* find(const K& key) {
    int pos = lower_bound(key);
    if (pos < data.size() && data[pos].first == key)
      return &data[pos].second;
    return nullptr;
  }

  const V* find(const K& key) const {
    int pos = lower_bound(key);
    if (pos < data.size() && data[pos].first == key)
      return &data[pos].second;
    return nullptr;
  }

  size_t size() const {
    return data.size();
  }

  bool erase(const K& key) {
    int pos = lower_bound(key);
    if (pos < data.size() && data[pos].first == key) {
      data.erase(data.begin() + pos);
      return true;
    }
    return false;
  }
};
