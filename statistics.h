//
// Created by robert on 23/07/2021.
//

#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <unordered_set>
#include <map>
#include <memory>

#include "agent.h"
#include "journey_type.h"

class statistics
{
public:
  static unsigned int count_active_mode(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static unsigned int
  count_active_mode_counter_to_inactive_norm(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static unsigned int
  count_inactive_mode_counter_to_active_norm(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static unsigned int count_active_norm(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static std::map<journey_type, unsigned int>
  count_active_mode_by_commute_length(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static std::map<std::shared_ptr<subculture>, unsigned int>
  count_active_mode_by_subculture(const std::unordered_set<std::shared_ptr<agent>> &agents);

  static std::map<std::shared_ptr<neighbourhood>, unsigned int>
  count_active_mode_by_neighbourhood(const std::unordered_set<std::shared_ptr<agent>> &agents);
};

#endif //_STATISTICS_H_
