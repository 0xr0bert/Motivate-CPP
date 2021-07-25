//
// Created by robert on 22/07/2021.
//

#ifndef _NEIGHBOURHOOD_CHANGE_H_
#define _NEIGHBOURHOOD_CHANGE_H_

#include <string>
#include <map>
#include "transport_mode.h"

/// This defines the changes in the neighbourhood that may form part of an intervention.
struct neighbourhood_change
{
  /// Neighbourhood ID.
  std::string id;

  /// Be very careful that this does not make the supportiveness > 1 or < 0.
  /// This represents an increase in supportiveness of v for transport mode k where (k,v) is an element of the map.
  std::map<transport_mode, float> increase_in_supportiveness;

  /// Be very careful that this does not make the capacity > the max size of an unsigned int, or < 0.
  /// This represents an increase in capacity of v for transport mode k where (k,v) is an element of the map.
  std::map<transport_mode, long> increase_in_capacity;
};

#endif //_NEIGHBOURHOOD_CHANGE_H_
