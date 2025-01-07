#include "SubcoloringChannelAllocation.hpp"
#include "Graph.hpp"

#include <iostream>
#include <climits>

bool canAddToClique(int v, const std::vector<int>& clique, const Graph& G) {
    for (int u : clique) {
        if (!G.areAdjacent(v, u)) {
            return false;
        }
    }
    return true;
}

void canAddToColorClass(int v, const std::vector<std::vector<int>> colorClass, const Graph& G, bool& createP3, int& candidate){
      // Test whether v will create a P3 in the color class
      bool satured = false;

      for (int j = 0; (unsigned)j < colorClass.size(); ++j) {
        auto& clique = colorClass[j];
        int numberNeighbors = 0 ;
        for (int u : clique) {
            if (G.areAdjacent(v, u)) {
                numberNeighbors ++;
            }
        }
        if ((satured || (unsigned)numberNeighbors< clique.size()) && numberNeighbors>0){
          createP3 = true ;
        }

        if (numberNeighbors >0){
          satured = true;
          candidate = j ;
        }
      }
}

void insertVertex(int v, const Graph& G, std::vector<std::vector<std::vector<int>>>& subcoloring, int maxCliqueSize) {
    for (int i = 0; (unsigned)i < subcoloring.size(); ++i) {
        auto& colorClass = subcoloring[i];


        // Test whether v will create a P3 in the color class
        bool changeColor = false;
        int candidate = -1 ;

        canAddToColorClass(v, colorClass, G, changeColor, candidate);

        // If v can be added to a clique of the color class
        if (!changeColor && candidate !=-1) {
            auto& clique = colorClass[candidate];
            if (clique.size() < (unsigned)maxCliqueSize) {
                clique.push_back(v);
                return;
            } else {
              changeColor = true; //v cannot be added anymore to this color class
            }
        }

        // If v can be added to the color class by creating a new clique
        if (!changeColor) {
            std::vector<int> newClique = {v};
            colorClass.push_back(newClique);
            return;
      }

  }

    std::vector<int> newClique = {v};
    std::vector<std::vector<int>> newColorClass = {newClique};
    subcoloring.push_back(newColorClass);
}

std::vector<std::vector<std::vector<int>>> getSubcoloring(const Graph& G, const std::vector<int>& vertices, int maxCliqueSize) {
    std::vector<std::vector<std::vector<int>>> subcoloring;
    for (int v : vertices) {
        insertVertex(v, G, subcoloring, maxCliqueSize);
    }
    return subcoloring;
}




// Function to decrease the size of each clique
void localSearchSubcoloring(const Graph& G, std::vector<std::vector<std::vector<int>>>& subcoloring, int numberColors){


    // Ensure subcoloring has at least `numberColors` entries
    if ((int)subcoloring.size() < numberColors) {
      subcoloring.resize(numberColors);  // Add empty entries if necessary
    }

    bool improved = true;

    // Initialize the arrays to store the color and clique index of each vertex
    std::vector<int> vertexColor(G.getNumVertices());
    std::vector<int> vertexClique(G.getNumVertices());

    // Traverse through each color class and each clique to initialize vertexColors and vertexClique
    for (int color = 0; (size_t)color < subcoloring.size(); ++color) {
        const auto& colorClass = subcoloring[color];

        for (int cliqueIndex = 0; (size_t)cliqueIndex < colorClass.size(); ++cliqueIndex) {
            const auto& clique = colorClass[cliqueIndex];

            for (int vertex : clique) {
                vertexColor[vertex] = color;  // Assign the vertex its color class
                vertexClique[vertex] = cliqueIndex; // Assign the vertex its clique index within the color class
            }
        }
    }



    // Continue until no improvement is possible
    while (improved) {
       improved = false;

       // Iterate over each vertex in the graph
       for (int v = 0; v < G.getNumVertices(); ++v) {
           int currentColor = vertexColor[v];  // Get the current color class of vertex v
           int currentCliqueIndex = vertexClique[v]; //get the current clique index of vertex v in its color class
           int currentCliqueSize = subcoloring[currentColor][currentCliqueIndex].size();  // Get current clique size

           if (currentCliqueSize>1){
               // Try to move the vertex to another color class
               for (int color = 0; color < numberColors; ++color) {
                   if (color != currentColor) {  // Skip the current color class

                       int newCliqueSize ;  // Track the new clique size
                       bool createP3 = false;
                       int candidate = -1 ;

                       canAddToColorClass(v, subcoloring[color], G, createP3, candidate);

                       if (!createP3){
                           if (candidate==-1){
                              newCliqueSize = 1 ;
                           } else {
                              newCliqueSize = subcoloring[color][candidate].size() + 1;
                           }
                       }

                       // If moving the vertex reduces the size of the clique, move it
                       if (!createP3 && newCliqueSize < currentCliqueSize) {
                           // Create a new clique without vertex v
                           auto& oldClique = subcoloring[currentColor][currentCliqueIndex];
                           std::vector<int> newOldClique;

                           // Copy all vertices from oldClique except for v
                           for (int vertex : oldClique) {
                               if (vertex != v) {
                                   newOldClique.push_back(vertex);
                               }
                           }

                           // Update the subcoloring entry for the old clique
                           subcoloring[currentColor][currentCliqueIndex] = newOldClique;

                           // Add v to its new clique
                           if (candidate == -1){
                             std::vector<int> newClique = {v};
                             subcoloring[color].push_back(newClique);
                             candidate = subcoloring[color].size() -1 ;
                           } else {
                             subcoloring[color][candidate].push_back(v);
                           }
                           vertexColor[v] = color ;
                           vertexClique[v] = candidate ;

                           improved = true;
                           break;
                       }

                   }
               }
            }
       }
    }


}




// Function to check if a subcoloring is valid
bool isValidSubcoloring(const Graph &G, const std::vector<std::vector<std::vector<int>>> &subcoloring) {
    // Iterate through each color class
    for (const auto &colorClass : subcoloring) {
        // For each clique in the color class
        for (const auto &clique : colorClass) {
            // Check if all vertices in the clique are pairwise adjacent
            for (size_t i = 0; i < clique.size(); ++i) {
                for (size_t j = i + 1; j < clique.size(); ++j) {
                    if (!G.areAdjacent(clique[i], clique[j])) {
                        // If two vertices in the same clique are not adjacent, it's not valid
                        return false;
                    }
                }
            }
        }

        // Check if there are no edges between different cliques within the same color class
        for (size_t i = 0; i < colorClass.size(); ++i) {
            for (size_t j = i + 1; j < colorClass.size(); ++j) {
                for (int v1 : colorClass[i]) {
                    for (int v2 : colorClass[j]) {
                        if (G.areAdjacent(v1, v2)) {
                            // If two vertices from different cliques are adjacent, it's not valid
                            std::cout << v1 << v2 ;
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;  // If all checks pass, it's a valid subcoloring
}


std::vector<std::vector<std::vector<int>>> SubcoloringChannelAllocation(const Graph& G, int nbColor, int cliqueSize)
{
  std::vector<int> vertices(G.getNumVertices());
  for (int i = 0; i < G.getNumVertices(); ++i) {
      vertices[i] = i;
  }
  std::vector<std::vector<std::vector<int>>> subcoloring ;
  subcoloring = getSubcoloring(G, vertices, cliqueSize);
  int numColorsSubcoloring = subcoloring.size();
  while (numColorsSubcoloring > nbColor){
    cliqueSize++;
    subcoloring = getSubcoloring(G, vertices, cliqueSize);
    numColorsSubcoloring = subcoloring.size();
  }
  localSearchSubcoloring(G, subcoloring, nbColor);

  return subcoloring;
}


// Function to flatten a 3D subcoloring structure (color classes containing cliques) into a 2D structure
std::vector<std::vector<int>> flatten(const std::vector<std::vector<std::vector<int>>>& subcoloring) {
    std::vector<std::vector<int>> flattenedSubcoloring;

    // Iterate through each color class
    for (const auto& colorClass : subcoloring) {
        std::vector<int> flattenedClass;

        // Iterate through each clique in the color class
        for (const auto& clique : colorClass) {
            // Add all vertices of the clique to the flattened class
            flattenedClass.insert(flattenedClass.end(), clique.begin(), clique.end());
        }

        // Add the flattened class to the flattened subcoloring
        flattenedSubcoloring.push_back(flattenedClass);
    }

    return flattenedSubcoloring;
}
