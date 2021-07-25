//
// Created by robert on 22/07/2021.
//

#ifndef MOTIVATE_CPP_SOCIAL_NETWORKS_H
#define MOTIVATE_CPP_SOCIAL_NETWORKS_H

#include <map>
#include <vector>

/// Generate a scale free network.
/// <https://en.wikipedia.org/wiki/Barab%C3%A1si%E2%80%93Albert_model>
/// \param minimum The minimum number of links an agent may have.
/// \param total_number The total number of nodes in the network.
/// \return A vector where the index is the ID of the agent, and this contains the IDs of their friends
std::vector<std::vector<unsigned int>> generate_social_network(unsigned int minimum, unsigned int total_number);

#endif //MOTIVATE_CPP_SOCIAL_NETWORKS_H
