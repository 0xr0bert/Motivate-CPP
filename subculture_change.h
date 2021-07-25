//
// Created by robert on 22/07/2021.
//

#ifndef _SUBCULTURE_CHANGE_H_
#define _SUBCULTURE_CHANGE_H_

#include <map>
#include <string>
#include "transport_mode.h"

/// This defines changes to a subculture that may form part of intervention.
struct subculture_change
{
  /// Subculture ID.
  std::string id;

  /// Be very careful that this does not make desirability > 1 or < 0.
  /// This represents an increase in desirability of v for transport mode k where (k,v) are elements of the Map.
  std::map<transport_mode, float> increase_in_desirability;
};

#endif //_SUBCULTURE_CHANGE_H_
