#ifndef SUBCOLORING_CHANNEL_ALLOCATION
#define SUBCOLORING_CHANNEL_ALLOCATION

#include "Graph.hpp"
#include <vector>

/**
 * Create a channel allocation based on the SCA algorithm (without using the charge level of the vertices)
 * @param G Graph representing the network
 * @param nbColor number of color/channel for the allocation
 * @param cliqueSize the maximum size for cliques. If not possible, this value is incremented
 * @return a vector of cliques for every color/channel
 */
std::vector<std::vector<std::vector<int>>> SubcoloringChannelAllocation(const Graph& G, int nbColor, int cliqueSize = 5);

/**
 * Flatten the inner level of a 3 level vector of int
 */
std::vector<std::vector<int>> flatten(const std::vector<std::vector<std::vector<int>>>& subcoloring) ;



#endif