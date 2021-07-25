//
// Created by robert on 22/07/2021.
//

#ifndef _INTERVENTION_H_
#define _INTERVENTION_H_

#include <unordered_set>
#include "neighbourhood_change.h"
#include "subculture_change.h"

/// This defines an intervention that can take place
struct intervention
{
  /// The day number of the intervention.
  unsigned int day;

  /// Changes in the neighbourhood.
  std::unordered_set<neighbourhood_change> neighbourhood_changes;

  /// Changes in the subculture.
  std::unordered_set<subculture_change> subculture_changes;

  /// Changes in the number of bikes.
  int change_in_number_of_bikes;

  /// Changes in the number of cars.
  int change_in_number_of_cars;
};

#endif //_INTERVENTION_H_
