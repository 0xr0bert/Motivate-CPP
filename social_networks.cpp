//
// Created by robert on 22/07/2021.
//

#include "social_networks.h"
#include <random>

std::vector<std::vector<unsigned int>> generate_social_network(unsigned int minimum, unsigned int total_number)
{
  // Create a map mapping IDs, to the IDs of their friends.
  std::vector<std::vector<unsigned int>> network;

  for (int i = 0; i < total_number; i++)
  {
    std::vector<unsigned int> i_friends;

    // If there aren't yet the minimum number of people in the network, link i to everybody already in the network.
    if (network.size() < minimum)
    {
      for (int j = 0; j < network.size(); j++)
      {
        network[j].push_back(i);
        i_friends.push_back(j);
      }
    }
    else
    {
      // Otherwise, create a weighted distribution, where the weight of being selected is equal to the size of your network.
      // This is preferential attachment.

      // Size of each person's friends
      std::vector<unsigned int> sizes;
      std::transform(network.begin(), network.end(), std::back_inserter(sizes), [](std::vector<unsigned int> friends)
                     { return friends.size(); });

      std::random_device rd;  // This is the seed for the RNG
      std::mt19937 gen(rd()); // Use the mersenne twister engine
      std::discrete_distribution<unsigned int> distribution(sizes.begin(), sizes.end());

      // For minimum IDs selected from the sample, link them together
      for (int _j = 0; _j < minimum; _j++)
      {
        auto id = distribution(gen);
        network[id].push_back(i);
        i_friends.push_back(id);
      }
    }

    network.push_back(i_friends);
  }

  return network;
}