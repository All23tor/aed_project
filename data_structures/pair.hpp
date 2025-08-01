#ifndef PAIR_H
#define PAIR_H

template <typename T1, typename T2>
struct Pair {
  T1 first;
  T2 second;

  bool operator==(const Pair& other) const = default;
  auto operator<=>(const Pair& other) const = default;
};
#endif
