#include "histogram.hpp"
#include "utils.hpp"
#include <iostream>

class CylindricalHistogram : public Histogram::Histogram<double> {
public:
  using Histogram::Histogram;

private:
  void do_normalize() override {
    std::vector<double> bin_volumes(m_hist.size(), 1.0);
    int unravelled_index[4];
    int r_bin;
    double min_r, r_bin_size, phi_bin_size, z_bin_size, bin_volume;
    std::vector<size_t> len_bins = get_n_bins();
    len_bins.push_back(3);
    for (size_t ind = 0; ind < m_hist.size(); ind += 3) {
      // Get the unravelled indices and calculate the bin volume.
      Utils::unravel_index(len_bins, 4, ind, unravelled_index);
      r_bin = unravelled_index[0];
      min_r = get_limits()[0].first;
      r_bin_size = get_bin_sizes()[0];
      phi_bin_size = get_bin_sizes()[1];
      z_bin_size = get_bin_sizes()[2];
      bin_volume =
          PI *
          ((min_r + (r_bin + 1) * r_bin_size) *
               (min_r + (r_bin + 1) * r_bin_size) -
           (min_r + r_bin * r_bin_size) * (min_r + r_bin * r_bin_size)) *
          z_bin_size * phi_bin_size / (2 * PI);
      m_hist[ind] /= bin_volume;
      m_hist[ind + 1] /= bin_volume;
      m_hist[ind + 2] /= bin_volume;
    }
  }
};

int main() {
  std::vector<size_t> n_bins{3, 3, 3};
  std::vector<std::pair<double, double>> limits{{std::make_pair(1.0, 20.0),
                                                 std::make_pair(-PI, PI),
                                                 std::make_pair(5.0, 10.0)}};
  CylindricalHistogram myhist(n_bins, 3, limits);
  auto lim = myhist.get_limits();
  for (auto &l : lim) {
    std::cout << "limits: min " << l.first << " max: " << l.second << std::endl;
  }
  std::vector<double> data{1.0, 3.0, 5.0};
  std::vector<double> weights{3.0, 2.0, 1.0};
  myhist.update(data, weights);
  myhist.update(data);
  std::vector<double> result = myhist.get_histogram();
  for (auto const &r : result) {
    std::cout << r << " ";
  }
  std::cout << "\nAfter normalization: " << std::endl;
  // Perform simple normalization.
  myhist.normalize();
  result = myhist.get_histogram();
  for (auto const &r : result) {
    std::cout << r << " ";
  }
  std::cout << std::endl;
}
