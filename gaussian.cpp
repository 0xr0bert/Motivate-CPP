//
// Created by robert on 25/07/2021.
//

#include "gaussian.h"
#include <numeric>
#include <random>
#include <stdexcept>
#include <memory>

unsigned int gaussian::pick_distribution_id(const std::vector<double> &weights)
{
  // Calculate the total of all the weights
  double total_weight = std::accumulate(weights.begin(), weights.end(), 0.0);

  // Generate a random number between 0 and total_weight
  std::random_device rd;  // This is the seed for the RNG
  std::mt19937 gen(rd()); // Use the mersenne twister engine
  std::uniform_real_distribution<double> distribution(0.0, total_weight);
  double random_value = distribution(gen);

  // Initialise CDF to 0
  double cdf = 0.0;

  // For each weight
  for (int i = 0; i < weights.size(); i++)
  {
    // Add weight to CDF
    cdf += weights[i];

    // If random number falls within the range of weight, return the index of this weight.
    if (random_value < cdf)
    {
      return i;
    }
  }

  // This should never have been sampled
  throw std::runtime_error("This should *never* have been reached!");
}

std::vector<double> gaussian::get_samples_from_gmm(unsigned int count, const std::vector<triple<double,
                                                                                                double,
                                                                                                double>> &distributions)
{
  // Create the normal distributions
  std::vector<std::unique_ptr<std::normal_distribution<double>>> gaussians;

  // Filter out just the weights
  std::vector<double> weights;

  std::transform(distributions.begin(), distributions.end(), std::back_inserter(weights), [](const triple<double, double, double> &t)
                 { return t.third; });

  // Create a vector for the samples

  std::vector<double> samples(count);

  // Generate a random number generator
  std::random_device rd;
  std::mt19937 gen(rd());

  // Generate count samples

  for (int i = 0; i < count; i++)
  {
    // Choose the gaussian
    auto &chosen_gaussian = gaussians[pick_distribution_id(weights)];

    // Then sample from it.
    samples[i] = chosen_gaussian->operator()(gen);
  }

  return samples;
}
