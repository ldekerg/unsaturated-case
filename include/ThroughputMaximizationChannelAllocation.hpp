#ifndef THROUGHPUT_MAXIMIZATION_CHANNEL_ALLOCATION_HPP
#define THROUGHPUT_MAXIMIZATION_CHANNEL_ALLOCATION_HPP

#include "Graph.hpp"

#include <vector>

/**
 * Create a channel allocation based on the TMCA algorithm, using weights
 * @param G Graph representing the network
 * @param nbColor number of color/channel for the allocation
 */
std::vector<std::vector<int>> ThroughputMaximizationChannelAllocation(const Graph& G, int nbColor);

/**
 * Create a channel allocation based on the TMCA algorithm, using weights, with a CMCA phase at the end
 * @param G Graph representing the network
 * @param nbColor number of color/channel for the allocation
 */
std::vector<std::vector<int>> ThroughputMaximizationChannelAllocationWithCMCA(const Graph& G, int nbColor);


/**
 * Create a channel allocation based on the TMCA algorithm
 * @param G Graph representing the network
 * @param nbColor number of color/channel for the allocation
 */
std::vector<std::vector<int>> ThroughputMaximizationChannelAllocationSaturated(const Graph& G, int nbColor);


#endif