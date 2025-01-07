#include <vector>
#include <limits>
#include "Graph.hpp"
#include "ConflictMinizationChannelAllocation.hpp"

std::vector<std::vector<int>> ConflictMinizationChannelAllocation(const Graph& G, int k) {
    int n = G.getNumVertices();
    std::vector<std::vector<int>> partition(k);  // k color classes

    // Iterate through each vertex
    for (int v = 0; v < n; ++v) {
        int bestClass = -1;
        double minNeighbors = std::numeric_limits<int>::max();

        // Find the color class with the lowest charged neighbors of v
        for (int i = 0; i < k; ++i) {
            double neighborRhoCount = 0;

            // Add charge of neighbors of v in color class i
            for (int u : partition[i]) {
                if (G.areAdjacent(v, u)) {
                    neighborRhoCount+=G.getRho(u);
                }
            }

            // Choose the color class with the lowest charge among neighbors
            if (neighborRhoCount < minNeighbors) {
                minNeighbors = neighborRhoCount;
                bestClass = i;
            }
        }

        // Assign vertex v to the best color class found
        partition[bestClass].push_back(v);
    }

    return partition;
}

std::vector<std::vector<int>> ConflictMinizationChannelAllocationSaturated(const Graph& G, int k) {
    int n = G.getNumVertices();
    std::vector<std::vector<int>> partition(k);  // k color classes

    // Iterate through each vertex
    for (int v = 0; v < n; ++v) {
        int bestClass = -1;
        int minNeighbors = std::numeric_limits<int>::max();

        // Find the color class with the fewest neighbors of v
        for (int i = 0; i < k; ++i) {
            int neighborCount = 0;

            // Count neighbors of v in color class i
            for (int u : partition[i]) {
                if (G.areAdjacent(v, u)) {
                    neighborCount++;
                }
            }

            // Choose the color class with the least number of neighbors
            if (neighborCount < minNeighbors) {
                minNeighbors = neighborCount;
                bestClass = i;
            }
        }

        // Assign vertex v to the best color class found
        partition[bestClass].push_back(v);
    }

    return partition;
}
