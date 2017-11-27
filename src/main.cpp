#include "histogram.hpp"
#include <iostream>

int main() {
  std::vector<size_t> n_bins{100, 100};
  std::vector<std::pair<double, double>> limits{
      {std::make_pair(1.0, 20.0), std::make_pair(5.0, 10.0)}};
  Histogram::Histogram<double> myhist(n_bins, 1, limits);
  auto lim = myhist.get_limits();
  for (auto &l : lim) {
    std::cout << "limits: min " << l.first << " max: " << l.second << std::endl;
  }
}
