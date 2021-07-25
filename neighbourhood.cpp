//
// Created by robert on 22/07/2021.
//

#include "neighbourhood.h"
#include <map>

neighbourhood::neighbourhood(const std::string &id, const std::map<transport_mode, float> &supportiveness,
                             const std::map<transport_mode, unsigned int> &capacity) : id(id),
                                                                                       supportiveness(supportiveness),
                                                                                       capacity(capacity)
{
  congestion_modifier = std::map<transport_mode, float>();
  congestion_modifier[WALK] = 1.0;
  congestion_modifier[CYCLE] = 1.0;
  congestion_modifier[PUBLIC_TRANSPORT] = 1.0;
  congestion_modifier[CAR] = 1.0;
}

const std::string &neighbourhood::get_id() const
{
  return id;
}

const std::map<transport_mode, float> &neighbourhood::get_supportiveness() const
{
  return supportiveness;
}

const std::map<transport_mode, unsigned int> &neighbourhood::get_capacity() const
{
  return capacity;
}

const std::map<transport_mode, float> &neighbourhood::get_congestion_modifier() const
{
  return congestion_modifier;
}

void neighbourhood::set_supportiveness(const std::map<transport_mode, float> &supportiveness)
{
  neighbourhood::supportiveness = supportiveness;
}

void neighbourhood::set_capacity(const std::map<transport_mode, unsigned int> &capacity)
{
  neighbourhood::capacity = capacity;
}

void neighbourhood::update_congestion_modifier()
{
  // Group agents by last mode
  // Count the agents
  // Map ->
  //   if count <= capacity then 1.0
  //   else
  //     maximum_excess_demand = agents_in_neighbourhood.size() - capacity
  //     actual_excess_demand = count - capacity
  //     1.0 - (actual_excess_demand / maximum_excess_demand)

  std::map<transport_mode, unsigned int> counts;

  for (auto &resident : residents)
  {
    counts[resident->get_last_mode()]++;
  }

  std::map<transport_mode, float> new_congestion_modifier;

  for (auto &pair : counts)
  {
    auto capacity_for_mode = capacity[pair.first];
    if (counts[pair.first] <= capacity_for_mode)
    {
      new_congestion_modifier[pair.first] = 1.0;
    }
    else
    {
      unsigned int maximum_excess_demand = residents.size() - capacity_for_mode;
      unsigned int actual_excess_demand = counts[pair.first] - capacity_for_mode;
      new_congestion_modifier[pair.first] = 1.0 - ((float)actual_excess_demand / (float)maximum_excess_demand);
    }
  }

  congestion_modifier = new_congestion_modifier;
}
bool neighbourhood::operator==(const neighbourhood &rhs) const
{
  return id == rhs.id;
}
bool neighbourhood::operator!=(const neighbourhood &rhs) const
{
  return !(rhs == *this);
}
