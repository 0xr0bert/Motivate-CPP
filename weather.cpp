//
// Created by robert on 22/07/2021.
//

#include "weather.h"
#include <random>

std::vector<weather> make_weather_pattern(
    std::map<weather, std::map<weather, double>> &transition_matrix,
    double chance_of_rain,
    int days)
{
  std::random_device rd;  // This is the seed for the RNG
  std::mt19937 gen(rd()); // Use the mersenne twister engine
  std::uniform_real_distribution<> distribution(0.0, 1.0);

  // Create an empty weather pattern
  std::vector<weather> pattern(days);

  // On day 0 consider if there is rain
  if (distribution(gen) > chance_of_rain)
  {
    pattern[0] = GOOD_WEATHER;
  }
  else
  {
    pattern[0] = BAD_WEATHER;
  }

  // For each day
  for (int i = 1; i < days; i++)
  {
    // Using the weather from the previous day,
    // get the probability of good weather
    // calculate a random float,
    // if this is this is less than that probability
    // then the weather for day i is good
    // otherwise it is bad.
    if (distribution(gen) < transition_matrix[pattern[i - 1]][GOOD_WEATHER])
    {
      pattern[i] = GOOD_WEATHER;
    }
    else
    {
      pattern[i] = BAD_WEATHER;
    }
  }
  return pattern;
}