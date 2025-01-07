#include "PerformanceEvaluation.hpp"
#include "utils.hpp"
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <functional> // Include for std::function
#include <iostream>
#include <map>
#include <time.h>

std::vector<double> evaluatePerformanceSubcoloring(const std::vector<std::vector<std::vector<int>>> &subcoloring, int numVertices)
{
  // Initialize a vector to store the performance for each vertex
  std::vector<double> performance(numVertices, 0.0);

  // Iterate over each color class
  for (const auto &colorClass : subcoloring)
  {
    // Iterate over each clique in the color class
    for (const auto &clique : colorClass)
    {
      int cliqueSize = clique.size();
      if (cliqueSize > 0)
      {
        // Iterate over each vertex in the clique
        for (int vertex : clique)
        {
          // The performance of a vertex is the inverse of the size of the clique it belongs to
          performance[vertex] = 1.0 / cliqueSize;
        }
      }
    }
  }

  return performance;
}

void dfs(const Graph &G, int v, std::vector<int> &component, std::vector<int> colorClass, std::vector<bool> &visited)
{
  visited[v] = true;      // Mark the vertex as visited
  component.push_back(v); // Add to the current component

  // Visit all neighbors of the current vertex
  for (int neighbor : G.getColoredNeighbors(v, colorClass))
  {
    if (!visited[neighbor])
    {
      dfs(G, neighbor, component, colorClass, visited); // Recursive DFS call
    }
  }
}

std::vector<std::vector<std::vector<int>>> connectedComponentsPartitions(const Graph &G, const std::vector<std::vector<int>> &partition)
{
  std::vector<std::vector<std::vector<int>>> components; // Store connected components for each color class
  std::vector<bool> visited(G.getNumVertices(), false);  // Track visited vertices

  for (const auto &colorClass : partition)
  {
    std::vector<std::vector<int>> colorClassComponents; // Store components for the current color class

    for (int vertex : colorClass)
    {
      if (!visited[vertex])
      {
        std::vector<int> component;                     // Store the current component
        dfs(G, vertex, component, colorClass, visited); // Start DFS from the current vertex in the induced graph
        colorClassComponents.push_back(component);      // Add the found component
      }
    }
    components.push_back(colorClassComponents); // Add the components for the current color class
  }

  return components; // Return all components
}

// Helper function to pick a random vertexstd::vector<std::vector<std::vector<int>>>  connectedComponentsPartitions(std::vector<std::vector<int>>  partition);

int pickRandomVertex(const std::vector<int> &vertices)
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dist(0, vertices.size() - 1);
  return vertices[dist(gen)];
}

std::vector<double> evaluatePerformanceArbitrarySaturated(const Graph &G, const std::vector<std::vector<std::vector<int>>> &partition, int T, double lambda)
{
  int n = G.getNumVertices();
  std::vector<double> performance(n, 0.0); // Initialize performance for all vertices to 0

  // Iterate through each color class
  for (const auto &colorClass : partition)
  {
    for (const auto &connectedComponent : colorClass)
    {
      std::vector<int> independentSet; // Current independent set, initially empty

      // Run the Markov chain for T steps
      for (int step = 0; step < T; ++step)
      {
        int u = pickRandomVertex(connectedComponent); // Pick a random vertex from the color class

        // Delete ↓: If u is in the independent set, remove it with probability 1 / (1 + lambda)
        if (std::find(independentSet.begin(), independentSet.end(), u) != independentSet.end())
        {
          double prob = 1.0 / (1.0 + lambda);
          if (((double)rand() / RAND_MAX) < prob)
          {
            independentSet.erase(std::remove(independentSet.begin(), independentSet.end(), u), independentSet.end());
          }
        }
        else
        {
          // Insert ↑: If u is not in the independent set and none of its neighbors are in the set
          bool canInsert = true;
          for (int neighbor : G.getNeighbors(u))
          {
            if (std::find(independentSet.begin(), independentSet.end(), neighbor) != independentSet.end())
            {
              canInsert = false;
              break;
            }
          }

          // If we can insert, do so with probability λ / (1 + λ)
          if (canInsert)
          {
            double prob = lambda / (1.0 + lambda);
            if (((double)rand() / RAND_MAX) < prob)
            {
              independentSet.push_back(u);
            }
          }
          else
          {
            // Drag ↔: If u has a unique neighbor v in the independent set, add u and remove v
            int uniqueNeighborInSet = -1;
            int neighborCountInSet = 0;
            for (int neighbor : G.getNeighbors(u))
            {
              if (std::find(independentSet.begin(), independentSet.end(), neighbor) != independentSet.end())
              {
                uniqueNeighborInSet = neighbor;
                neighborCountInSet++;
                if (neighborCountInSet > 1)
                  break; // More than 1 neighbor in the set, break
              }
            }
            if (neighborCountInSet == 1)
            {
              independentSet.erase(std::remove(independentSet.begin(), independentSet.end(), uniqueNeighborInSet), independentSet.end());
              independentSet.push_back(u);
            }
          }
        }

        // Update performance for all vertices in the independent set
        for (int v : independentSet)
        {
          performance[v]++;
        }
      }
    }
  }

  // Normalize performance values by the number of steps
  for (int i = 0; (unsigned)i < performance.size(); ++i)
  {
    performance[i] /= T;
  }

  return performance;
}

// Kerbosch-Bron algorithm to find all cliques in a graph, as discussed in https://doi.org/10.1016/j.tcs.2008.05.010
void KB(std::unordered_set<int> R, std::vector<int> P, std::unordered_set<int> X, const Graph &G, std::vector<std::unordered_set<int>> &cliques)
{

  if (P.size() == 0 && X.size() == 0)
  {
    cliques.push_back(R);
  }
  else
  {
    for (const auto elem : P)
    {
      P.erase(std::find(P.begin(), P.end(), elem));
      std::unordered_set<int> Rnew(R);
      Rnew.insert(elem);
      std::vector<int> Pnew;
      std::unordered_set<int> Xnew;
      for (int v : G.getNeighbors(elem))
      {
        if (std::find(P.begin(), P.end(), v) != P.end())
        {
          Pnew.push_back(v);
        }
        if (X.find(v) != X.end())
        {
          Xnew.insert(v);
        }
      }
      KB(Rnew, Pnew, Xnew, G, cliques);
      X.insert(elem);
    }
  }
}

void calcSubset(std::vector<int> &A, std::vector<std::vector<int>> &res, std::vector<int> &subset, int index)
{
  // Add the current subset to the result list
  res.push_back(subset);

  // Generate subsets by recursively including and
  // excluding elements
  for (int i = index; i < A.size(); i++)
  {
    // Include the current element in the subset
    subset.push_back(A[i]);

    // Recursively generate subsets with the current
    // element included
    calcSubset(A, res, subset, i + 1);

    // Exclude the current element from the subset
    // (backtracking)
    subset.pop_back();
  }
}

std::vector<std::vector<int>> subsets(std::vector<int> &A)
{
  std::vector<int> subset;
  std::vector<std::vector<int>> res;
  int index = 0;
  calcSubset(A, res, subset, index);
  return res;
}

// find all clique containing v in the given subgraph
std::vector<std::vector<int>> findAllCliquesIncludingNode(const Graph &G, const int v, std::vector<int> subgraph)
{
  std::vector<int> neighbours = G.getNeighbors(v);
  neighbours.push_back(v);
  std::vector<int> neighboursInSubgraph;
  for (int nei : neighbours)
  {
    if (std::find(subgraph.begin(), subgraph.end(), nei) != subgraph.end())
    {
      neighboursInSubgraph.push_back(nei);
    }
  }

  std::vector<std::vector<int>> allSubset = subsets(neighboursInSubgraph);

  std::vector<std::vector<int>> cliques;
  for (const auto &subset : allSubset)
  {
    bool isClique = true;
    int i = 0;
    while (isClique && i < subset.size())
    {
      int j = i + 1;
      while (isClique && i != subset.size() && j < subset.size())
      {
        if (!G.areAdjacent(subset[i], subset[j]))
        {
          isClique = false;
        }
        j++;
      }
      i++;
    }
    if (isClique)
    {
      cliques.push_back(subset);
    }
  }
  // remove non maximal and double
  std::map<int, std::vector<int>> correctCliques;
  for (int i = 0; i < cliques.size(); i++)
  {
    correctCliques[i] = cliques[i];
  }
  for (int i = 0; i < cliques.size(); i++)
  {
    bool included = false;
    int j = 0;
    // loop through every other cliques to check if cliques[i] is included in it
    while (!included && j < cliques.size())
    {
      if (j != i && (correctCliques.find(j) != correctCliques.end()))
      {
        // assume cliques[i] is included in cliques[j]
        included = true;
        for (const auto &elem : cliques[i])
        {
          // if one element is not found, cliques[i] is not included in cliques[j]
          if (std::find(correctCliques[j].begin(), correctCliques[j].end(), elem) == correctCliques[j].end())
          {
            included = false;
          }
        }
      }
      j++;
    }
    if (included)
    {
      correctCliques.erase(i);
    }
  }

  // reshape solution and return it
  std::vector<std::vector<int>> solution;
  for (const auto &clique : correctCliques)
  {
    std::vector<int> cliqueAsVector;
    for (const auto v : clique.second)
    {
      cliqueAsVector.push_back(v);
    }
    solution.push_back(cliqueAsVector);
  }

  return solution;
}

std::vector<std::vector<int>> findAllCliquesIncludingNode2(const Graph &G, const int v, std::vector<int> subgraph)
{
  std::vector<int> neighbours = G.getNeighbors(v);
  std::vector<int> neighboursInSubgraph;
  for (int nei : neighbours)
  {
    if (std::find(subgraph.begin(), subgraph.end(), nei) != subgraph.end())
    {
      neighboursInSubgraph.push_back(nei);
    }
  }
  std::vector<std::vector<std::vector<int>>> maximalCliques;

  // Induction init, create all cliques of size 2 (edges of v)
  std::vector<std::vector<int>> cliquesOfSize2;
  for (int nei : neighboursInSubgraph)
    cliquesOfSize2.push_back({v, nei});
  maximalCliques.push_back(cliquesOfSize2);

  // Inductive search of maximal cliques
  bool inductionStop = false;
  while (!inductionStop)
  {
    std::vector<std::vector<int>> cliquesOfSizeNPlus1;
    // loop through the cliques of size n (we will try to add a vertex)
    for (const auto clique : maximalCliques.back())
    {
      // check if one vertex (not in the clique) can be added to the clique
      for (int nei : neighboursInSubgraph)
      {
        bool isInClique = true;
        for (int c : clique)
        {
          if (!G.areAdjacent(nei, c))
            isInClique = false;
        }
        if (isInClique)
        {
          std::vector<int> biggerClique = clique;
          biggerClique.push_back(nei);
          cliquesOfSizeNPlus1.push_back(biggerClique);
        }
      }
    }
    // if cliquesOfSizeNPlus1 is empty, we reached the end, all cliques are maximal
    if (cliquesOfSizeNPlus1.size() == 0)
      inductionStop = true;
    else
    {
      // Clean cliques of size n that are no longer maximal
      std::vector<std::vector<int>>::iterator it = maximalCliques.back().begin();
      // loop through every clique (it) of size n
      while (it != maximalCliques.back().end())
      {
        bool included = false;
        int j = 0;
        // loop through every cliques of size n+1 to check if clique (it) is included in it
        while (!included && j < cliquesOfSizeNPlus1.size())
        {
          //assume clique (it) is included in CliquesOfSizeNPlus1[j]
          included = true;
          // loop through every element of clique (it)
          for (const auto v : *it)
          {
            // check if this element can be found in the clique of size n+1
            if(std::find(cliquesOfSizeNPlus1[j].begin(), cliquesOfSizeNPlus1[j].end(), v) == cliquesOfSizeNPlus1[j].end())
              included = false;
          }
          j++;
        }
        if (included)
        {
          it = maximalCliques.back().erase(it);
        }
        else
          ++it;
      }

      // Clean identical cliques of size n+1
      std::vector<std::vector<int>>::iterator it2 = cliquesOfSizeNPlus1.begin();
      // loop through every clique of size n+1
      while (it2 != cliquesOfSizeNPlus1.end())
      {
        bool included = false;
        std::vector<std::vector<int>>::iterator it3 = it2;
        ++it3;
        //loop through every cliques of size n+1
        while (!included && it3 != cliquesOfSizeNPlus1.end())
        {
          //assume clique (it2) is included in CliquesOfSizeNPlus1[j]
          included = true;
          // loop through every element of clique (it2)
          for (const auto v : *it2)
          {
            // check if this element can be found in the clique of size n+1
            if(std::find(it3->begin(), it3->end(), v) == it3->end())
              included = false;
          }
          ++it3;
        }
        if (included)
        {
          it2 = cliquesOfSizeNPlus1.erase(it2);
        }
        else
          ++it2;
      }

      //end of induction step, add cliques of size n+1 to solution
      maximalCliques.push_back(cliquesOfSizeNPlus1);
    }
  }

  //flatten solution before return
  std::vector<std::vector<int>> solution;
  for (const auto& cliqueGroup : maximalCliques)
  {
    for (const auto& cliques : cliqueGroup)
    {
      solution.push_back(cliques);
    }
  }

  return solution;
}

std::vector<double> unsaturatedPerformanceModel(const Graph &G, const std::vector<std::vector<int>> &allocation)
{
  std::vector<double> performance(G.getNumVertices(), -1);
  double timeStartMarkov = (double) clock();
  std::vector<double> perfSaturated = evaluatePerformanceArbitrarySaturated(G, connectedComponentsPartitions(G, allocation));
  double timeEndMarkov = (double) clock();


  // 1. if rho < p_v, u_v = p_v
  for (int v = 0; v < G.getNumVertices(); v++)
  {
    if (G.getRho(v) < perfSaturated[v])
    {
      performance[v] = G.getRho(v);
    }
  }

  double totalTimeFind = 0.0;
  double totalTimeFind2 = 0.0;
  double totalTimeCliques = 0.0;
  // 2. loop over all remaining vertex
  while (std::find(performance.begin(), performance.end(), -1) != performance.end())
  {
    double min_u = 2;
    int min_v = -1;
    // compute the possible utilsation for each vertex
    for (int i = 0; i < performance.size(); i++)
    {
      if (performance[i] == -1)
      {
        // the possible utilisation is the product of the utilisation available in each clique this node belongs to
        double timeStartFind = (double) clock();
        std::vector<std::vector<int>> cliques = findAllCliquesIncludingNode(G, i, findClass(allocation, i));
        double timeEndFind = (double) clock();
        totalTimeFind += timeEndFind - timeStartFind;

        double timeStartCliques = (double) clock();
        double prod = 1.0;
        for (const auto &clique : cliques)
        {
          double nb_unset = 0.0;
          double sum_set = 0.0;
          for (int c : clique)
          {
            if (performance[c] == -1)
              nb_unset++;
            else
              sum_set += performance[c];
          }
          prod *= ((1.0 - sum_set) / nb_unset);
        }
        prod = std::min(prod, G.getRho(i));

        // update only the minimum
        if (prod < min_u)
        {
          min_u = prod;
          min_v = i;
        }
        double timeEndCliques = (double) clock();
        totalTimeCliques += timeEndCliques - timeStartCliques;

      }
    }
    performance[min_v] = min_u;
  }

  // std::cout << "Markov : " << (timeEndMarkov - timeStartMarkov) / CLOCKS_PER_SEC << std::endl;
  // std::cout << "Find : " << totalTimeFind / CLOCKS_PER_SEC << std::endl;
  // std::cout << "Find 2 : " << totalTimeFind2 / CLOCKS_PER_SEC << std::endl;
  // std::cout << "Cliques : " << totalTimeCliques / CLOCKS_PER_SEC << std::endl;

  return performance;
}


std::vector<double> unsaturatedPerformanceModelForSubcoloring(const Graph& G, const std::vector<std::vector<std::vector<int>>>& allocation)
{
  std::vector<double> performance(G.getNumVertices());
  for (const auto& colorClass : allocation)
  {
    for (auto clique : colorClass)
    {
      double medium_remaining = 1.0;
      
      // First phase, nodes with rho < 1/n get their rho
      bool iterate = true;
      while (iterate && clique.size() != 0)
      {
        std::vector<int> low_nodes;
        for (int c : clique)
        {
          if (G.getRho(c) <= (medium_remaining / clique.size()))
          {
            low_nodes.push_back(c);
          }
        }
        if (low_nodes.size() != 0)
        {
          for (int v : low_nodes)
          {
            performance[v] = G.getRho(v);
            medium_remaining -= G.getRho(v);
            clique.erase(std::find(clique.begin(), clique.end(), v));
          }
          iterate = true;
        }
        else
        {
          iterate = false;
        }
      }

      // Second phase
      for (int c : clique)
      {
        performance[c] = medium_remaining / clique.size();
      }
    }
  }
  return performance;
}