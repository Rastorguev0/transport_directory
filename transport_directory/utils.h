#pragma once

#include <iterator>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

template <typename It>
class Range {
public:
  using ValueType = typename std::iterator_traits<It>::value_type;

  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }
  size_t size() const { return std::distance(begin_, end_); }

private:
  It begin_;
  It end_;
};

template <typename C>
auto AsRange(const C& container) {
  return Range{std::begin(container), std::end(container)};
}

template <typename It>
size_t ComputeUniqueItemsCount(Range<It> range) {
  return std::unordered_set<typename Range<It>::ValueType>{
      range.begin(), range.end()
  }.size();
}

template <typename K, typename V>
const V* GetValuePointer(const std::unordered_map<K, V>& map, const K& key) {
  if (auto it = map.find(key); it != end(map)) {
    return &it->second;
  } else {
    return nullptr;
  }
}

template <typename T>
std::unordered_set<T> Intersect(const std::unordered_set<T>& set1, const std::unordered_set<T>& set2) {
  const std::unordered_set<T> *min_ptr, *max_ptr;
  if (set1.size() < set2.size()) {
    min_ptr = &set1;
    max_ptr = &set2;
  }
  else {
    min_ptr = &set2;
    max_ptr = &set1;
  }
  std::unordered_set<T> result;
  for (const auto& item : *min_ptr) {
    if (max_ptr->count(item)) result.insert(item);
  }
  return result;
}

std::string_view Strip(std::string_view line);
