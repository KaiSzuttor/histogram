#ifndef _SRC_UTILS_HPP
#define _SRC_UTILS_HPP

#include <cmath>
#include <numeric>

/** Pi. */
#define PI 3.14159265358979323846264338328

namespace Utils {

size_t calculate_bin_index(double value, double bin_size) {
  return std::floor(value / bin_size);
}

size_t calculate_bin_index(double value, double bin_size, double offset) {
  return (calculate_bin_index(value, bin_size) - std::floor(offset / bin_size));
}

inline size_t ravel_index(std::vector<size_t> unravelled_indices,
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

/**
 * \brief Returns the unravelled index of the provided flat index.
 *        Therefore is the inversion of flattening an ndims dimensional index.
 * @param len_dims an int array of length ndims containing the lengths of the
 * dimensions. (Input)
 * @param ndims int denoting the number of dimensions. (Input)
 * @flattened_index an int denoting the flat index. (Input)
 * @unravelled_index_out an int array with length ndims where the unflat indices
 * are written to. (Output)
 */
inline void unravel_index(std::vector<size_t> const &len_dims, const int ndims,
                          const int flattened_index,
                          int *unravelled_index_out) {
  // idea taken from
  // http://codinghighway.com/2014/02/22/c-multi-dimensional-arrays-part-2-flattened-to-unflattened-index/
  if (len_dims.size() != ndims)
    throw std::invalid_argument("Length of len_dims is not ndims!");
  std::vector<int> mul(ndims);
  mul[ndims - 1] = 1;
  for (int j = ndims - 2; j >= 0; j--)
    mul[j] = mul[j + 1] * len_dims[j + 1];
  for (int j = 0; j < ndims; j++)
    unravelled_index_out[j] = (flattened_index / mul[j]) % len_dims[j];
}

/*
 * \brief Check if data is within limits.
 * \param data: data value to check.
 * \param limits: the min/max values.
 */
template <typename T>
bool inline check_limits(std::vector<T> const &data,
                         std::vector<std::pair<T, T>> limits) {
  if (data.size() != limits.size()) {
    throw std::invalid_argument("Dimension of data and limits not the same!");
  }
  bool res = true;
  for (size_t i = 0; i < data.size(); ++i) {
    if (data[i] < limits[i].first or data[i] > limits[i].second)
      res = false;
  }
  return res;
}

} // namespace Utils
#endif
