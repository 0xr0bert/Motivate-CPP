//
// Created by robert on 22/07/2021.
//

#ifndef MOTIVATE_CPP_NEIGHBOURHOOD_H
#define MOTIVATE_CPP_NEIGHBOURHOOD_H

#include <string>
#include <map>
#include <unordered_set>
#include <vector>
#include <memory>
#include "transport_mode.h"

// Forward declare agent
class agent;
#include "agent.h"

/// A neighbourhood in the simulation
class neighbourhood
{
private:
  /// The ID of the neighbourhood
  std::string id;

  /// The supportiveness of the neighbourhood.
  /// A score from 0 to 1 for each transport mode, on how supportive the environment is.
  std::map<transport_mode, float> supportiveness;

  /// The capacity of the neighbourhood.
  /// The maximum capacity for a transport mode, at which there is no congestion.
  std::map<transport_mode, unsigned int> capacity;

  /// The calculated congestion modifier.
  std::map<transport_mode, float> congestion_modifier;

  /// The residents who live in this neighbourhood
  std::unordered_set<std::shared_ptr<agent>> residents;

public:
  /// Creates a new neighbourhood.
  /// \param id The ID of the neighbourhood.
  /// \param supportiveness How supportive the environment is; a score from 0 to 1 for each transport mode.
  /// \param capacity The maximum capacity for a transport mode, at which there is no congestion.
  neighbourhood(const std::string &id, const std::map<transport_mode, float> &supportiveness,
                const std::map<transport_mode, unsigned int> &capacity);

  /// Get the ID of the neighbourhood.
  /// \return The ID of the neighbourhood.
  [[nodiscard]] const std::string &get_id() const;

  /// Get the supportiveness of the neighbourhood.
  /// \return A score from 0 to 1 for each transport mode.
  [[nodiscard]] const std::map<transport_mode, float> &get_supportiveness() const;

  /// Gets the capacity of the neighbourhood
  /// \return The maximum capacity for a transport mode, at which there is no congestion.
  [[nodiscard]] const std::map<transport_mode, unsigned int> &get_capacity() const;

  /// Gets the congestion modifier of the neighbourhood.
  /// \return A score from 0 to 1 where this is a negative influence on the desire to take a transport mode.
  [[nodiscard]] const std::map<transport_mode, float> &get_congestion_modifier() const;

  /// Sets the supportiveness of the neighbourhood.
  /// \param supportiveness The new supportiveness.
  void set_supportiveness(const std::map<transport_mode, float> &supportiveness);

  /// Sets the capacity of the neighbourhood.
  /// \param capacity The new supportiveness.
  void set_capacity(const std::map<transport_mode, unsigned int> &capacity);

  /// Update the congestion modifier based upon what the residents did at the previous time step.
  void update_congestion_modifier();

  /// Checks whether two neighbourhoods are the same.
  /// \param rhs The other neighbourhood.
  /// \return True iff their IDs are equal
  bool operator==(const neighbourhood &rhs) const;

  /// Checks whether two neighbourhoods are not the same.
  /// \param rhs The other neighbourhood.
  /// \return True iff their IDs are not equal.
  bool operator!=(const neighbourhood &rhs) const;
};

#endif //MOTIVATE_CPP_NEIGHBOURHOOD_H
