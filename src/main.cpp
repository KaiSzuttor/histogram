#include "histogram.hpp"
#include <iostream>

int main() {
  std::vector<size_t> n_bins{10, 10};
  std::vector<std::pair<double, double>> limits{
      {std::make_pair(1.0, 20.0), std::make_pair(5.0, 10.0)}};
  Histogram::Histogram<double> myhist(n_bins, 2, limits);
  auto lim = myhist.get_limits();
  for (auto &l : lim) {
    std::cout << "limits: min " << l.first << " max: " << l.second << std::endl;
  }
  std::vector<double> data{1.0, 5.0};
  std::vector<double> weights{3.0, 2.0};
  myhist.update(data, weights);
  std::vector<double> result = myhist.get_histogram();
  for (auto const &r : result) {
    std::cout << r << " ";
  }
  std::cout << std::endl;
}
