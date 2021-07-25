//
// Created by robert on 23/07/2021.
//

#include "statistics.h"
unsigned int statistics::count_active_mode(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  unsigned int count = 0;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    switch (agent->get_current_mode())
    {
    case WALK:
    case CYCLE:
      count++;
      break;
    default:
      break;
    }
  }

  return count;
}

unsigned int
statistics::count_active_mode_counter_to_inactive_norm(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  unsigned int count = 0;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    auto agent_mode = agent->get_current_mode();
    auto agent_norm = agent->get_norm();

    if ((agent_mode == WALK || agent_mode == CYCLE) && (agent_norm != WALK && agent_norm != CYCLE))
      count++;
  }

  return count;
}

unsigned int
statistics::count_inactive_mode_counter_to_active_norm(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  unsigned int count = 0;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    auto agent_mode = agent->get_current_mode();
    auto agent_norm = agent->get_norm();

    if ((agent_mode != WALK && agent_mode != CYCLE) && (agent_norm == WALK || agent_norm == CYCLE))
      count++;
  }

  return count;
}
unsigned int statistics::count_active_norm(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  unsigned int count = 0;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    switch (agent->get_norm())
    {
    case WALK:
    case CYCLE:
      count++;
      break;
    default:
      break;
    }
  }

  return count;
}
std::map<journey_type, unsigned int>
statistics::count_active_mode_by_commute_length(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  std::map<journey_type, unsigned int> counts;

  for (int journey_type_int = LOCAL_COMMUTE; journey_type_int != DISTANT_COMMUTE; journey_type_int++)
  {
    auto type = static_cast<journey_type>(journey_type_int);
    counts[type] = 0;
  }

  for (const std::shared_ptr<agent> &agent : agents)
  {
    switch (agent->get_current_mode())
    {
    case WALK:
    case CYCLE:
      counts[agent->get_commute_length()]++;
      break;
    default:
      break;
    }
  }

  return counts;
}

std::map<std::shared_ptr<subculture>, unsigned int>
statistics::count_active_mode_by_subculture(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  std::map<std::shared_ptr<subculture>, unsigned int> counts;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    std::shared_ptr<subculture> culture = agent->get_agent_subculture();
    auto mode = agent->get_current_mode();
    if (mode == WALK || mode == CYCLE)
    {
      try
      {
        counts[culture] = counts.at(culture) + 1;
      }
      catch (std::out_of_range &)
      {
        counts[culture] = 1;
      }
    }
  }

  return counts;
}

std::map<std::shared_ptr<neighbourhood>, unsigned int>
statistics::count_active_mode_by_neighbourhood(const std::unordered_set<std::shared_ptr<agent>> &agents)
{
  std::map<std::shared_ptr<neighbourhood>, unsigned int> counts;

  for (const std::shared_ptr<agent> &agent : agents)
  {
    std::shared_ptr<neighbourhood> hood = agent->get_agent_neighbourhood();
    auto mode = agent->get_current_mode();

    if (mode == WALK || mode == CYCLE)
    {
      try
      {
        counts[hood] = counts.at(hood) + 1;
      }
      catch (std::out_of_range &)
      {
        counts[hood] = 1;
      }
    }
  }

  return counts;
}
