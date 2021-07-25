//
// Created by robert on 22/07/2021.
//

#ifndef MOTIVATE_CPP_WEATHER_H
#define MOTIVATE_CPP_WEATHER_H

#include <vector>
#include <map>

/// The weather for a given day.
enum weather
{
    GOOD_WEATHER,
    BAD_WEATHER
};

/// This generates a weather pattern using a Markov Chain.
/// \param transition_matrix This states if in state x, the chance of moving to state y is z, where x and y are weather conditions that may be equal.
/// \param chance_of_rain The chance of rain on day 0.
/// \param days The number of days to generate.
/// \return A weather pattern as a vector where the index is the day.
std::vector<weather> make_weather_pattern(
    std::map<weather, std::map<weather, double>> &transition_matrix,
    double chance_of_rain,
    int days);

#endif //MOTIVATE_CPP_WEATHER_H
