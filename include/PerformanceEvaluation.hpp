#ifndef PERFORMANCE_EVALUATION_HPP
#define PERFORMANCE_EVALUATION_HPP

#include <vector>
#include <unordered_set>
#include "Graph.hpp"

// Function to evaluate the performance of each node in the graph based on the subcoloring
std::vector<double> evaluatePerformanceSubcoloring(const std::vector<std::vector<std::vector<int>>>& subcoloring, int numVertices);

//Function which receive a partition of the graph, and return the same partition where each color class is parittioned into its connected components
std::vector<std::vector<std::vector<int>>>  connectedComponentsPartitions(const Graph& G, const std::vector<std::vector<int>>& partition);

// Function to evaluate the performance of each node in the graph based on a given partition
std::vector<double> evaluatePerformanceArbitrarySaturated(const Graph& G, const std::vector<std::vector<std::vector<int>>>& partition, int T = 1000, double lambda = 100.0);

/**
 * Estimate the performances of an allocation using a custom model
 * @param G graph storing the network informations
 * @param allocation allocation of the vertices of the graph in different classes/colors/channels
 */
std::vector<double> unsaturatedPerformanceModel(const Graph& G, const std::vector<std::vector<int>>& allocation);

/**
 * Estimate the performances of an Subcoloring allocation using a custom model
 * @param G graph storing the network informations
 * @param allocation allocation of the vertices of the graph in different classes/colors/channels, in the form of a list of cliques for every class/color/channel
 */
std::vector<double> unsaturatedPerformanceModelForSubcoloring(const Graph& G, const std::vector<std::vector<std::vector<int>>>& allocation);

/**
 * Find all maximal cliques containing a vertex v in the given subgraph
 * @param G Graph containing infomartions about edges
 * @param v index of the vertex in the graph
 * @param subraph vector of edges of the subgraph
 */
std::vector<std::vector<int>> findAllCliquesIncludingNode(const Graph& G, const int v, std::vector<int> subgraph);

/**
 * Find all maximal cliques containing a vertex v in the given subgraph, not recursive
 * @param G Graph containing infomartions about edges
 * @param v index of the vertex in the graph
 * @param subraph vector of edges of the subgraph
 */
std::vector<std::vector<int>> findAllCliquesIncludingNode2(const Graph& G, const int v, std::vector<int> subgraph);



#endif
