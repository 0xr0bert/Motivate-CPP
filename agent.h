//
// Created by robert on 22/07/2021.
//

#ifndef MOTIVATE_CPP_AGENT_H
#define MOTIVATE_CPP_AGENT_H

#include "transport_mode.h"
#include "subculture.h"
#include "journey_type.h"
#include "weather.h"
#include <map>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <fstream>

// Forward declare neighbourhood
class neighbourhood;
#include "neighbourhood.h"

/// The agent in the model.
class agent
{
private:
  /// How the agent travelled to work on the previous day.
  transport_mode last_mode;

  /// The demographic subculture of the agent.
  std::shared_ptr<subculture> agent_subculture;

  /// The neighbourhood the agent lives in.
  std::shared_ptr<neighbourhood> agent_neighbourhood;

  /// The distance of the agent's commute.
  journey_type commute_length;

  /// How sensitive the agent is to the weather.
  float weather_sensitivity;

  /// How consistent the agent is (used as a weighting for habit).
  float consistency;

  /// How connected the agent is to its social network.
  float social_connectivity;

  /// How connected the agent is to its subculture.
  float subculture_connectivity;

  /// How connected the agent is to its neighbourhood.
  float neighbourhood_connectivity;

  /// The weight used for the average.
  float average_weight;

  /// The habit of the agent, mapping the transport-mode to an recency-weighted average where 1 was used,
  /// if the transport mode was used on a given day, 0 if not.
  std::map<transport_mode, float> habit;

  /// How the agent is currently travelling to work.
  transport_mode current_mode;

  /// The maximum of the joint effects of social network, subculture, and neighbours, appropriately weighted.
  transport_mode norm;

  /// Whether the agent owns a bike.
  bool owns_bike;

  /// Whether the agent owns a car.
  bool owns_car;

  /// The friends of the agent.
  std::unordered_set<std::shared_ptr<agent>> social_network;

  /// The neighbours of the agent.
  std::unordered_set<std::shared_ptr<agent>> neighbours;

  /// Counts in a subgroup of agents the percentage of people taking each travel mode.
  /// \param agents The subgroup of agents.
  /// \param weight The weight to apply to the percentage.
  /// \return For each transport mode, the weighted counts.
  static std::map<transport_mode, float>
  count_agents_in_subgroup(const std::unordered_set<std::shared_ptr<agent>> &agents, float weight);

public:
  /// Gets the last mode the agent used.
  /// \return The last method the agent used to commute to work.
  [[nodiscard]] transport_mode get_last_mode() const;

  /// Calculates the mode budget for the agent. Will also update the norm.
  /// \return The mode budget as a map from transport mode to rank.
  std::map<transport_mode, unsigned int> calculate_mode_budget();

  /// Calculates the (economic) cost of travel.
  /// \param weather_today The current weather.
  /// \param change_in_weather True if there has been a change in weather, false otherwise.
  /// \return A map from transport mode to its cost ranking.
  std::map<transport_mode, unsigned int> calculate_cost(weather weather_today, bool change_in_weather) const;

  /// Updates the habit. This should be called at the start of each day.
  /// Also updates the last mode.
  void update_habit();

  /// Choose a mode of travel. Updates current mode.
  /// \param weather_today The current weather.
  /// \param change_in_weather True if there has been a change in weather, false otherwise.
  void choose(weather weather_today, bool change_in_weather);

  /// Gets the current mode of the agent.
  /// \return How the agent is currently commuting to work.
  transport_mode get_current_mode() const;

  /// Gets the norm of the agent.
  /// \return The maximum of the joint effects of social network, subculture, and neighbours, appropriately weighted.
  transport_mode get_norm() const;

  /// Gets the commute length of the agent.
  /// \return How far the agent has to commute to work.
  journey_type get_commute_length() const;

  /// Gets the subculture of the agent.
  /// \return The demographic subculture of the agent.
  const std::shared_ptr<subculture> &get_agent_subculture() const;

  /// Gets the neighbourhood of the agent.
  /// \return The neighbourhood the agent lives in.
  const std::shared_ptr<neighbourhood> &get_agent_neighbourhood() const;

  static std::vector<std::shared_ptr<agent>> load_unlinked_agents_from_file(std::ifstream file, const std::unordered_set<std::shared_ptr<subculture>> &subcultures, const std::unordered_set<std::shared_ptr<neighbourhood>> &neighbourhoods);

  agent(transport_mode last_mode, const std::shared_ptr<subculture> &agent_subculture,
        const std::shared_ptr<neighbourhood> &agent_neighbourhood, journey_type commute_length,
        float weather_sensitivity, float consistency, float social_connectivity, float subculture_connectivity,
        float neighbourhood_connectivity, float average_weight, const std::map<transport_mode, float> &habit, transport_mode current_mode,
        transport_mode norm, bool owns_bike, bool owns_car, const std::unordered_set<std::shared_ptr<agent>> &social_network,
        const std::unordered_set<std::shared_ptr<agent>> &neighbours);
};

#endif //MOTIVATE_CPP_AGENT_H
