#ifndef _SRC_UTILS_HPP
#define _SRC_UTILS_HPP

#include <cmath>
#include <numeric>

namespace Utils {

size_t calculate_bin_index(double value, double bin_size) {
  return std::floor(value / bin_size);
}

size_t calculate_bin_index(double value, double bin_size, double offset) {
  return (calculate_bin_index(value, bin_size) - std::floor(offset / bin_size));
}

size_t inline ravel_index(std::vector<size_t> unravelled_indices,
                          std::vector<size_t> n_bins) {
  // index calculation: using the following formula for N dimensions:
  //   ind = ind_{N-1} + sum_{j=0}^{N-2} (ind_j * prod_{k=j+1}^{N-1} n_k)
  size_t res = unravelled_indices.back();
  for (size_t j = 0; j < unravelled_indices.size() - 1; ++j) {
    res += unravelled_indices[j] * std::accumulate(n_bins.begin() + j + 1,
                                                   n_bins.end(), 1,
                                                   std::multiplies<size_t>());
  }
  return res;
}

/*
 * \brief Check if data is within limits.
 * \param data: data value to check.
 * \param limits: the min/max values.
 */
template <typename T>
bool inline check_limits(std::vector<T> const &data,
                  std::vector<std::pair<T, T>> limits) {
  bool res = true;
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i] < limits[i].first or data[i] > limits[i].second)
      res = false;
  }
  return res;
}

} // namespace Utils
#endif
