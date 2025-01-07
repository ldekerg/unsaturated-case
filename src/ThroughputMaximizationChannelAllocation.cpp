#include "ThroughputMaximizationChannelAllocation.hpp"
#include "Graph.hpp"

#include <vector>
#include <set>
#include <algorithm>

using namespace std;

vector<int> greedyWMIS(const Graph &G, set<int> subgraph)
{
  vector<int> WMIS;
  vector<int> excluded;
  while (WMIS.size() + excluded.size() < subgraph.size())
  {
    double maxValue = -1;
    int imax = subgraph.size();
    // find vertex that maximises weight(v)/(1+degree(v))
    for (int i : subgraph)
    {
      if (std::find(WMIS.begin(), WMIS.end(), i) == WMIS.end() && std::find(excluded.begin(), excluded.end(), i) == excluded.end())
      {
        int degcount = 0;
        for (int j : subgraph)
        {
          if (std::find(WMIS.begin(), WMIS.end(), j) == WMIS.end() && std::find(excluded.begin(), excluded.end(), j) == excluded.end())
          {
            if (G.areAdjacent(i, j))
              degcount++;
          }
        }
        double val = G.getRho(i) / (1.0 + degcount);
        if (val > maxValue)
        {
          maxValue = val;
          imax = i;
        }
      }
    }
    // add max value to WMIS
    WMIS.push_back(imax);
    // remove vertex and neighbourgs
    for (int i : subgraph)
    {
      if (G.areAdjacent(i, imax))
      {
        if (std::find(WMIS.begin(), WMIS.end(), i) == WMIS.end() && std::find(excluded.begin(), excluded.end(), i) == excluded.end())
        {
          excluded.push_back(i);
        }
      }
    }
  }
  return WMIS;
}

vector<int> greedyMIS(const Graph &G, set<int> subgraph)
{
  vector<int> MIS;
  vector<int> excluded;
  while (MIS.size() + excluded.size() < subgraph.size())
  {
    int degmin = subgraph.size();
    int imin = -1;
    // find minimum degree
    for (int i : subgraph)
    {
      if (count(MIS.begin(), MIS.end(), i) == 0 && count(excluded.begin(), excluded.end(), i) == 0)
      {
        int degcount = 0;
        for (int j : subgraph)
        {
          if (G.areAdjacent(i, j) && count(MIS.begin(), MIS.end(), j) == 0 && count(excluded.begin(), excluded.end(), j) == 0)
          {
            degcount++;
          }
        }
        if (degcount < degmin)
        {
          degmin = degcount;
          imin = i;
        }
      }
    }
    // add min degree to MIS
    MIS.push_back(imin);
    // remove neighbourgs
    for (int i : subgraph)
    {
      if (G.areAdjacent(i, imin) && count(MIS.begin(), MIS.end(), i) == 0 && count(excluded.begin(), excluded.end(), i) == 0)
      {
        excluded.push_back(i);
      }
    }
  }
  return MIS;
}

vector<vector<int>> ThroughputMaximizationChannelAllocation(const Graph &G, int k)
{
  vector<vector<int>> partition(k);
  set<int> subgraph;
  for (int i = 0; i < G.getNumVertices(); i++)
  {
    subgraph.insert(i);
  }
  for (int i = 0; i < k; i++)
  {
    partition[i] = greedyWMIS(G, subgraph);
    
    for (int v : partition[i])
    {
      subgraph.erase(v);
    }
  }
  // Random channel for remaining APs
  srand(time(nullptr));
  for (int v : subgraph)
  {
    partition[rand() % k].push_back(v);
  }

  return partition;
}

vector<vector<int>> ThroughputMaximizationChannelAllocationWithCMCA(const Graph &G, int k)
{
  vector<vector<int>> partition(k);
  set<int> subgraph;
  for (int i = 0; i < G.getNumVertices(); i++)
  {
    subgraph.insert(i);
  }
  for (int i = 0; i < k; i++)
  {
    partition[i] = greedyWMIS(G, subgraph);
    
    for (int v : partition[i])
    {
      subgraph.erase(v);
    }
  }
  
  // CMCA for remaining APs
  // Iterate through each vertex
  for (int v : subgraph) {
      int bestClass = -1;
      double minNeighbors = G.getNumVertices();

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

vector<vector<int>> ThroughputMaximizationChannelAllocationSaturated(const Graph &G, int k)
{
  vector<vector<int>> partition(k);
  set<int> subgraph;
  for (int i = 0; i < G.getNumVertices(); i++)
  {
    subgraph.insert(i);
  }
  for (int i = 0; i < k; i++)
  {
    partition[i] = greedyMIS(G, subgraph);

    for (int v : partition[i])
    {
      subgraph.erase(v);
    }
  }
  // Random channel for remaining APs
  srand(time(nullptr));
  for (int v : subgraph)
  {
    partition[rand() % k].push_back(v);
  }

  return partition;
}