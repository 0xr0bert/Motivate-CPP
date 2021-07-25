//
// Created by robert on 22/07/2021.
//

#include "journey_type.h"

std::map<transport_mode, float> get_journey_cost(journey_type journey_type)
{
  std::map<transport_mode, float> cost;
  switch (journey_type)
  {
  case LOCAL_COMMUTE:
    cost[CAR] = 0.2f;
    cost[PUBLIC_TRANSPORT] = 0.2f;
    cost[CYCLE] = 0.2f;
    cost[WALK] = 0.2f;
    break;
  case CITY_COMMUTE:
    cost[CAR] = 0.3f;
    cost[PUBLIC_TRANSPORT] = 0.3f;
    cost[CYCLE] = 0.6f;
    cost[WALK] = 0.9f;
    break;
  case DISTANT_COMMUTE:
    cost[CAR] = 0.1f;
    cost[PUBLIC_TRANSPORT] = 0.1f;
    break;
  }

  return cost;
}