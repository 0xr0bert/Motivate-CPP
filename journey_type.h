//
// Created by robert on 22/07/2021.
//

#ifndef _JOURNEY_TYPE_H_
#define _JOURNEY_TYPE_H_

#include <map>
#include "transport_mode.h"

/// A categorical distance for commuting.
enum journey_type
{
    LOCAL_COMMUTE,
    CITY_COMMUTE,
    DISTANT_COMMUTE
};

/// Gets the (economic) cost for different transport modes for the journey type.
/// Where there is no entry, the journey is impossible
/// \param journey_type The journey type.
/// \return A map from transport mode to the cost of commuting with that mode.
std::map<transport_mode, float> get_journey_cost(journey_type journey_type);

#endif //_JOURNEY_TYPE_H_
