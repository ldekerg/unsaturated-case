#ifndef GRAPH_GENERATOR_HPP
#define GRAPH_GENERATOR_HPP

#include "Graph.hpp"
#include <string>
#include <vector>

/**
 * Generate a random disk graph in a 1*1 space
 * @param n number of nodes
 * @param r radius of the disks
 */
Graph generateUnitDiskGraph(int n, double r);

/**
 * Generate a random disk graph in a 1*1 space
 * @param n number of nodes
 * @param radius of the disks
 * @param rhos list of rhos to use in the graph
 */
Graph generateUnitDiskGraph(int n, double r, std::vector<double> rhos);

/**
 * Generate a random disk graphs in a 1*1 space, and store them in a file
 * @param n number of nodes
 * @param r radius of the disks
 * @param filename name of the file
 * @return the graph generated
 */
Graph generateUnitDiskGraphsToFile(int n, double r, const std::string& filename);

/**
 * Generate a random ball graph in a 1*1*1 space
 * @param n number of nodes
 * @param r radius of the disks
 */
Graph generateUnitBallGraph(int n, double r);

/** Generate a graph from a .graphml file
 * @param filename name of the file
 */
Graph fromGraphml (const std::string filename);

#endif // GRAPH_GENERATOR_HPP
