#ifndef CONFLICT_MINIZATION_CHANNEL_ALLOCATION_HPP
#define CONFLICT_MINIZATION_CHANNEL_ALLOCATION_HPP

#include "Graph.hpp"
#include <vector>

/**
 * Create a channel allocation based on the CMCA algorithm, using weights
 * @param G Graph representing the network
 * @param k number of color/channel for the allocation
 */
std::vector<std::vector<int>> ConflictMinizationChannelAllocation(const Graph& G, int k);

/**
 * Create a channel allocation based on the CMCA algorithm
 * @param G Graph representing the network
 * @param k number of color/channel for the allocation
 */
std::vector<std::vector<int>> ConflictMinizationChannelAllocationSaturated(const Graph& G, int k);

#endif
