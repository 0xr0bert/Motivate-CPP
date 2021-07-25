#ifndef MOTIVATE_CPP_SCENARIO_H
#define MOTIVATE_CPP_SCENARIO_H

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <fstream>
#include "subculture.h"
#include "intervention.h"
#include "neighbourhood.h"

// A scenario for a simulation run
class scenario {
    private:
    /// The subcultures in the scenario
    std::vector<std::shared_ptr<subculture>> subcultures;
    /// The neighbourhoods in the scenario;
    std::vector<std::shared_ptr<neighbourhood>> neighbourhoods;
    /// The intervention
    intervention scenario_intervention;
    public:
    /// The scenario ID
    const std::string id;
    /// The number of bikes in the scenario
    const unsigned int number_of_bikes;
    /// The number of cars in the scenario
    const unsigned int number_of_cars;
};

#endif