#ifndef _SRC_HISTOGRAM_HPP
#define _SRC_HISTOGRAM_HPP

#include <functional>
#include <numeric>
#include <vector>

#include "utils.hpp"

namespace Histogram {

/**
 * \brief Calculate the bin sizes.
 * \param limits: containts min/max values for each dimension.
 * \param nbins: number of bins for each dimension.
 * \return The bin sizes for each dimension.
 */
template <typename T>
std::vector<T> calc_bin_sizes(std::vector<std::pair<T, T>> const &limits,
                              std::vector<size_t> const &n_bins) {
  std::vector<T> tmp;
  for (size_t ind = 0; ind < limits.size(); ++ind) {
    tmp.push_back((limits[ind].second - limits[ind].first) / n_bins[ind]);
  }
  return tmp;
}

template <typename T> class Histogram {
public:
  explicit Histogram(std::vector<size_t> n_bins, size_t n_dims_data, std::vector<std::pair<T, T> > limits);
  std::vector<size_t> get_n_bins() const;
  std::vector<T> get_histogram() const;
  std::vector<std::pair<T, T>> get_limits() const;
  std::vector<T> bin_sizes() const;
  void update(std::vector<T> const &data);
  void update(std::vector<T> const &data, std::vector<T> const &weights);

private:
  // Number of bins for each dimension.
  std::vector<size_t> m_n_bins;
  // Number of dimensions for a single data point.
  size_t m_n_dims_data;
  // Min and max values for each dimension.
  std::vector<std::pair<T, T>> m_limits;
  // Bin sizes for each dimension.
  std::vector<T> m_bin_sizes;
  // Flat histogram data.
  std::vector<T> m_hist;
};

/**
 * \brief Histogram constructor.
 * \param n_bins: the number of bins in each histogram dimension.
 * \param n_dims_data: the number of dimensions the data has (e.g. 3 for
 *        vector field).
 * \param limits: the minimum/maximum data values to consider for the
 *        histogram.
 */
template <typename T>
Histogram<T>::Histogram(std::vector<size_t> n_bins, size_t n_dims_data,
                   std::vector<std::pair<T, T> > limits)
    : m_n_bins(n_bins), m_n_dims_data(n_dims_data), m_limits(limits) {
  if (n_bins.size() != limits.size()) {
    throw std::invalid_argument("Argument for number of bins and limits do "
                                "not have same number of dimensions!");
  }
  m_bin_sizes = calc_bin_sizes<T>(limits, n_bins);
  size_t n_bins_total =
      m_n_dims_data * std::accumulate(std::begin(n_bins), std::end(n_bins), 1,
                                      std::multiplies<size_t>());
  m_hist = std::vector<T>(n_bins_total);
};

/**
 * \brief Add data to the histogram.
 * \param data: vector of single data value with type T.
 *              The size of the given vector has to match the number
 *              of dimensions of the histogram.
 */
template <typename T>
void Histogram<T>::update(std::vector<T> const &data) {
  if (Utils::check_limits(data, m_limits)) {
    std::vector<size_t> index;
    for (size_t dim = 0; dim < m_n_bins.size(); ++dim) {
      index.push_back(Utils::calculate_bin_index(data[dim], m_bin_sizes[dim],
                                                 m_limits[dim].first));
    }
    size_t flat_index = Utils::ravel_index(index, m_n_bins);
    m_hist[flat_index] += static_cast<T>(1);
  }
}

/**
 * \brief Add data to the histogram.
 * \param data: vector of single data value with type T.
 *              The size of the given vector has to match the number
 *              of dimensions of the histogram.
 * \param weights: m_n_dims_data dimensional weights.
 */
template <typename T>
void Histogram<T>::update(std::vector<T> const &data,
                          std::vector<T> const &weights) {
  if (Utils::check_limits(data, m_limits)) {
    std::vector<size_t> index;
    for (size_t dim = 0; dim < m_n_bins.size(); ++dim) {
      index.push_back(Utils::calculate_bin_index(data[dim], m_bin_sizes[dim],
                                                 m_limits[dim].first));
    }
    size_t flat_index = m_n_dims_data * Utils::ravel_index(index, m_n_bins);
    if (weights.size() != m_n_dims_data)
      throw std::invalid_argument("Wrong dimensions of given weights!");
    for (size_t ind = 0; ind < m_n_dims_data; ++ind) {
      m_hist[flat_index + ind] += static_cast<T>(weights[ind]);
    }
  }
}

/**
 * \brief Get the bin sizes.
 */
template <typename T> std::vector<T> Histogram<T>::bin_sizes() const {
  return m_bin_sizes;
}

/**
 * \brief Get the number of bins for each dimension.
 */
template <typename T> std::vector<size_t> Histogram<T>::get_n_bins() const {
  return m_n_bins;
}

/**
 * \brief Get the ranges (min, max) for each dimension.
 */
template <typename T>
std::vector<std::pair<T, T>> Histogram<T>::get_limits() const {
  return m_limits;
}

/**
 * \brief Get the histogram data.
 */
template <typename T> std::vector<T> Histogram<T>::get_histogram() const {
  return m_hist;
}

} // namespace Histogram
#endif
