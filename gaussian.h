//
// Created by robert on 25/07/2021.
//

#ifndef _GAUSSIAN_H_
#define _GAUSSIAN_H_

#include <vector>
#include "triple.h"

/// This class contains a static method to draw from a Gaussian Mixture Model.
class gaussian
{
private:
  /// Picks a distribution ID to draw from.
  /// \param weights The weights of each distribution, where the index corresponds to the index of a distribution in
  /// another vector.
  /// \return The chosen distribution ID.
  static unsigned int pick_distribution_id(const std::vector<double> &weights);

public:
  /// This gets samples from a Gaussian Mixture Model.
  /// \param count The number of samples
  /// \param distributions A vector of triples (mean, sd, weight)
  /// \return The samples.
  [[nodiscard]] static std::vector<double>
  get_samples_from_gmm(unsigned int count, const std::vector<triple<double, double, double>> &distributions);
};

#endif //_GAUSSIAN_H_
