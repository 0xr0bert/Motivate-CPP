//
// Created by robert on 22/07/2021.
//

#include "agent.h"
#include <limits>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

transport_mode agent::get_last_mode() const
{
  return last_mode;
}
std::map<transport_mode, unsigned int> agent::calculate_mode_budget()
{
  // This is the percentage of people in the social network who take a given mode, weighted by social connectivity.
  auto social_vals = count_agents_in_subgroup(social_network, social_connectivity);

  // This is the percentage of neighbours who take a given mode, weighted by neighbourhood connectivity.
  auto neighbour_vals = count_agents_in_subgroup(neighbours, neighbourhood_connectivity);

  // Weight the subculture desirability by subculture connectivity.
  std::map<transport_mode, float> subculture_vals;
  auto subculture_desirability = agent_subculture->get_desirability();

  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    subculture_vals[mode] = subculture_desirability[mode] * subculture_connectivity;
  }

  // Initial value of the norm is the lowest possible float.
  float norm_value = std::numeric_limits<float>::lowest();

  // The max of social_vals, neighbour_vals, and subculture_vals is the norm.
  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    float v = social_vals[mode] + neighbour_vals[mode] + subculture_vals[mode];
    if (v > norm_value)
    {
      norm = mode;
      norm_value = v;
    }
  }

  // Get the existing habit
  std::map<transport_mode, float> habit_vals = habit;

  // Weight the habit by consistency.
  for (auto &pair : habit_vals)
  {
    habit_vals[pair.first] = pair.second * consistency;
  }

  // Calculate the average of social_vals, neighbour_vals, subculture_vals, and habit_vals.
  std::map<transport_mode, float> average;

  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    average[mode] = (social_vals[mode] + neighbour_vals[mode] + subculture_vals[mode] + habit_vals[mode]) / 4.0f;
  }

  // If they don't own a car or bike, the mode is impossible.
  std::map<transport_mode, float> ownership_modifier;
  ownership_modifier[CAR] = owns_car ? 1.0f : 0.0f;
  ownership_modifier[CYCLE] = owns_bike ? 1.0f : 0.0f;
  ownership_modifier[WALK] = 1.0f;
  ownership_modifier[PUBLIC_TRANSPORT] = 1.0f;

  std::map<transport_mode, float> intermediate_budget;
  const auto &congestion = agent_neighbourhood->get_congestion_modifier();

  // The budget is the mode by the congestion by the ownership modifier.
  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    intermediate_budget[mode] = average[mode] * congestion.at(mode) * ownership_modifier[mode];
  }

  // Sort the budget in reverse.
  std::vector<std::pair<transport_mode,
                        float>>
      reversed_budget(intermediate_budget.begin(), intermediate_budget.end());

  std::sort(reversed_budget.begin(), reversed_budget.end(), [](const auto &pair1, const auto &pair2)
            { return pair1.second >= pair2.second; });

  // Rank the reversed budget.
  std::map<transport_mode, unsigned int> return_value;

  for (int i = 0; i < reversed_budget.size(); i++)
  {
    return_value[reversed_budget[i].first] = i;
  }

  return return_value;
}
std::map<transport_mode, unsigned int> agent::calculate_cost(weather weather_today, bool change_in_weather) const
{
  // Get the supportiveness of the neighbourhood
  std::map<transport_mode, float> neighbourhood_vals = agent_neighbourhood->get_supportiveness();

  // Calculate 1 - supportiveness, so a lower supportiveness = higher cost
  for (auto &pair : neighbourhood_vals)
  {
    neighbourhood_vals[pair.first] = 1.0f - pair.second;
  }

  const auto &commute_cost = get_journey_cost(commute_length);

  // Average of journey cost and neighbourhood vals
  std::map<transport_mode, float> average;

  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    if (neighbourhood_vals.contains(mode) && commute_cost.contains(mode))
    {
      average[mode] = (neighbourhood_vals[mode] + commute_cost.at(mode)) / 2;
    }
  }

  // If the weather is bad this has an impact on cost.
  if (weather_today == BAD_WEATHER)
  {
    // resolve is 0.1 if it was bad yesterday & they cycled and walked.
    // it is -0.1 if bad yesterday & they cycled and walked.
    // 0.0 if good yesterday.
    float resolve = (!change_in_weather) ? ((last_mode == WALK || last_mode == CYCLE) ? -0.1f : 0.1f) : 0.0f;

    average[CYCLE] *= 1.0f + weather_sensitivity + resolve;
    average[WALK] *= 1.0f + weather_sensitivity + resolve;
  }

  // Convert average to vector of pairs.
  std::vector<std::pair<transport_mode, float>> cost_ordered(average.begin(), average.end());

  // Order by cost
  std::sort(cost_ordered.begin(), cost_ordered.end(), [](auto &pair1, auto &pair2)
            { return pair1.second < pair2.second; });

  std::map<transport_mode, unsigned int> return_value;

  for (int i = 0; i < cost_ordered.size(); i++)
  {
    return_value[cost_ordered[i].first] = i;
  }

  return return_value;
}
void agent::update_habit()
{
  last_mode = current_mode;
  std::map<transport_mode, float> new_habit = habit;
  for (auto &pair : new_habit)
  {
    // For the mode taken yesterday, new habit is (1 - average_weight) * habit + average_weight
    // otherwise it is just (1 - average_weight) * habit
    new_habit[pair.first] = (pair.first == last_mode) ? (1 - average_weight) * pair.second + average_weight : (1 - average_weight) * pair.second;
  }

  habit = new_habit;
}
void agent::choose(weather weather_today, bool change_in_weather)
{
  // Get the budget and cost
  auto budget = calculate_mode_budget();
  auto cost = calculate_cost(weather_today, change_in_weather);
  std::map<transport_mode, unsigned int> sum;

  // Sum the budget and cost
  for (auto &pair : budget)
  {
    if (cost.contains(pair.first))
    {
      sum[pair.first] = pair.second * cost[pair.first];
    }
  }

  // If they don't own a car or bike remove that as a valid option.
  if (!owns_car)
  {
    sum.erase(CAR);
  }

  if (!owns_bike)
  {
    sum.erase(CYCLE);
  }

  // Convert sum to a map from cost to a vector with all modes of that cost.
  std::map<unsigned int, std::vector<transport_mode>> sums;

  for (auto &pair : sum)
  {
    sums[pair.second].push_back(pair.first);
  }

  // Convert to a vec of pairs.
  std::vector<std::pair<unsigned int, std::vector<transport_mode>>> sums_vector(sums.begin(), sums.end());

  // Sort by cost
  std::sort(sums_vector.begin(), sums_vector.end(), [](auto &pair1, auto &pair2)
            { return pair1.first < pair2.first; });

  // If only one optino has that cost, choose it.
  if (sums_vector.at(0).second.size() == 1)
  {
    current_mode = sums_vector[0].second[0];
  }
  else
  {
    // Otherwise, choose the mode with the highest cost.
    std::vector<transport_mode> potential_modes = sums_vector[0].second;

    std::sort(potential_modes.begin(), potential_modes.end(), [&budget](auto first, auto second)
              { return budget[first] < budget[second]; });

    current_mode = potential_modes[0];
  }
}
std::map<transport_mode, float>
agent::count_agents_in_subgroup(const std::unordered_set<std::shared_ptr<agent>> &agents, float weight)
{
  std::map<transport_mode, unsigned int> n_agents;

  // Initialise all values as 0.
  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    n_agents[mode] = 0;
  }

  // Count the number of agents who took that mode.
  for (auto &agent : agents)
  {
    n_agents[agent->get_last_mode()]++;
  }

  // Weight the counts.
  std::map<transport_mode, float> weighted_n_agents;
  for (int transport_mode_int = CAR; transport_mode_int != WALK; transport_mode_int++)
  {
    auto mode = static_cast<transport_mode>(transport_mode_int);
    weighted_n_agents[mode] = ((float)n_agents[mode]) * weight / ((float)agents.size());
  }

  return weighted_n_agents;
}
transport_mode agent::get_current_mode() const
{
  return current_mode;
}
transport_mode agent::get_norm() const
{
  return norm;
}
journey_type agent::get_commute_length() const
{
  return commute_length;
}
const std::shared_ptr<subculture> &agent::get_agent_subculture() const
{
  return agent_subculture;
}
const std::shared_ptr<neighbourhood> &agent::get_agent_neighbourhood() const
{
  return agent_neighbourhood;
}
std::vector<std::shared_ptr<agent>>
agent::load_unlinked_agents_from_file(std::ifstream file, const std::unordered_set<std::shared_ptr<subculture>> &subcultures, const std::unordered_set<std::shared_ptr<neighbourhood>> &neighbourhoods)
{
  // Get subcultures from their ID
  std::map<std::string, std::shared_ptr<subculture>> subcultures_kvp;

  for (auto s : subcultures)
  {
    subcultures_kvp[s->get_id()] = s;
  }

  // Do the same for neighbourhoods

  std::map<std::string, std::shared_ptr<neighbourhood>> neighbourhoods_kvp;

  for (auto n : neighbourhoods)
  {
    neighbourhoods_kvp[n->get_id()] = n;
  }

  // Read JSON
  char readBuffer[65526];
  rapidjson::IStreamWrapper isw(file);

  rapidjson::Document d;
  d.ParseStream(isw);

  // Root of the document should be an array
  if (!d.IsArray())
    throw new std::runtime_error("JSON is in the wrong format -- array not at base");

  // Create vector of agents

  std::vector<std::shared_ptr<agent>> agents;

  // For each agent
  for (auto &a : d.GetArray())
  {
    // Process last mode
    rapidjson::Value::ConstMemberIterator itr = a.FindMember("last_mode");
    if (itr == a.MemberEnd() || !itr->value.IsString())
      throw new std::runtime_error("JSON is in the wrong format -- see last_mode");

    transport_mode last_mode;

    auto last_mode_string = itr->value.GetString();

    if (last_mode_string == "WALK")
    {
      last_mode = WALK;
    }
    else if (last_mode_string == "CYCLE")
    {
      last_mode = CYCLE;
    }
    else if (last_mode_string == "PUBLIC_TRANSPORT")
    {
      last_mode = PUBLIC_TRANSPORT;
    }
    else if (last_mode_string == "CAR")
    {
      last_mode = CAR;
    }
    else
    {
      throw new std::runtime_error("JSON is in the wrong format -- see last_mode");
    }

    // Process subculture
    itr = a.FindMember("subculture_id");
    if (itr == a.MemberEnd() || !itr->value.IsString())
      throw new std::runtime_error("JSON is in the wrong format -- see subculture_id");

    std::shared_ptr<subculture> agent_subculture;

    auto subculture_string = itr->value.GetString();

    if (!subcultures_kvp.contains(std::string(subculture_string)))
      throw new std::runtime_error("JSON is in the wrong format -- see subculture_id");

    agent_subculture = subcultures_kvp[std::string(subculture_string)];

    // Process neighbourhood
    itr = a.FindMember("neighbourhood_id");
    if (itr == a.MemberEnd() || !itr->value.IsString())
      throw new std::runtime_error("JSON is in the wrong format -- see neighbourhood_id");

    std::shared_ptr<neighbourhood> agent_neighbourhood;

    auto neighbourhood_string = itr->value.GetString();

    if (!neighbourhoods_kvp.contains(std::string(neighbourhood_string)))
      throw new std::runtime_error("JSON is in the wrong format -- see neighbourhood_id");

    agent_neighbourhood = neighbourhoods_kvp[std::string(neighbourhood_string)];

    // Process commute length
    itr = a.FindMember("commute_length");
    if (itr == a.MemberEnd() || !itr->value.IsString())
      throw new std::runtime_error("JSON is in the wrong format -- see commute_length");

    auto commute_length_str = itr->value.GetString();
    journey_type commute_length;

    if (commute_length_str == "LOCAL_COMMUTE")
    {
      commute_length = LOCAL_COMMUTE;
    }
    else if (commute_length_str == "CITY_COMMUTE")
    {
      commute_length = CITY_COMMUTE;
    }
    else if (commute_length_str == "DISTANT_COMMUTE")
    {
      commute_length = DISTANT_COMMUTE;
    }
    else
    {
      throw new std::runtime_error("JSON is in the wrong format -- see commute_length");
    }

    // Process weather sensitivity
    itr = a.FindMember("weather_sensitivity");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see weather_sensitivity");

    float weather_sensitivity = itr->value.GetFloat();

    // Process consistency
    itr = a.FindMember("consistency");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see consistency");

    float consistency = itr->value.GetFloat();

    // Process social_connectivity
    itr = a.FindMember("social_connectivity");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see social_connectivity");

    float social_connectivity = itr->value.GetFloat();

    // Process subculture_connectivity
    itr = a.FindMember("subculture_connectivity");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see subculture_connectivity");

    auto subculture_connectivity = itr->value.GetFloat();

    // Process neighbourhood_connectivity
    itr = a.FindMember("neighbourhood_connectivity");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see neighbourhood_connectivity");

    auto neighbourhood_connectivity = itr->value.GetFloat();

    // Process average_weight
    itr = a.FindMember("average_weight");
    if (itr == a.MemberEnd() || !itr->value.IsFloat())
      throw new std::runtime_error("JSON is in the wrong format -- see average_weight");

    auto average_weight = itr->value.GetFloat();

    // Process habit
    std::map<transport_mode, float> habit;
    habit[WALK] = 0.0f;
    habit[CYCLE] = 0.0f;
    habit[PUBLIC_TRANSPORT] = 0.0f;
    habit[CAR] = 0.0f;

    // Process current_mode and norm
    auto current_mode = last_mode;
    auto norm = last_mode;

    // Process owns bike
    itr = a.FindMember("owns_bike");
    if (itr == a.MemberEnd() || !itr->value.IsBool())
      throw new std::runtime_error("JSON is in the wrong format -- see owns_bike");

    bool owns_bike = itr->value.GetBool();

    // Process owns car
    itr = a.FindMember("owns_car");
    if (itr == a.MemberEnd() || itr->value.IsBool())
      throw new std::runtime_error("JSON is in the wrong format -- see owns_car");

    bool owns_car = itr->value.GetBool();

    // Process friends and neighbours
    std::unordered_set<std::shared_ptr<agent>> social_network;
    std::unordered_set<std::shared_ptr<agent>> neighbours;

    auto a_ptr = std::make_shared<agent>(last_mode, agent_subculture, agent_neighbourhood,
                             commute_length, weather_sensitivity, consistency, social_connectivity, subculture_connectivity,
                             neighbourhood_connectivity, average_weight, habit, current_mode, norm, owns_bike, owns_car,
                             social_network, neighbours);
    agents.push_back(a_ptr);
  }

  return agents;
}

agent::agent(transport_mode last_mode, const std::shared_ptr<subculture> &agent_subculture,
             const std::shared_ptr<neighbourhood> &agent_neighbourhood, journey_type commute_length,
             float weather_sensitivity, float consistency, float social_connectivity, float subculture_connectivity,
             float neighbourhood_connectivity, float average_weight, const std::map<transport_mode, float> &habit, transport_mode current_mode,
             transport_mode norm, bool owns_bike, bool owns_car, const std::unordered_set<std::shared_ptr<agent>> &social_network,
             const std::unordered_set<std::shared_ptr<agent>> &neighbours) : last_mode(last_mode), agent_subculture(agent_subculture),
                                                                             agent_neighbourhood(agent_neighbourhood), commute_length(commute_length), weather_sensitivity(weather_sensitivity),
                                                                             consistency(consistency), social_connectivity(social_connectivity), subculture_connectivity(subculture_connectivity),
                                                                             neighbourhood_connectivity(neighbourhood_connectivity), average_weight(average_weight), habit(habit),
                                                                             current_mode(current_mode), norm(norm), owns_bike(owns_bike), owns_car(owns_car), social_network(social_network),
                                                                             neighbours(neighbours) {}