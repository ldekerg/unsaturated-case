#include "Graph.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <string>

Graph::Graph(int n) : n(n), adjMatrix(n, std::vector<bool>(n, false)), rhoTab(n, 1), positions(n) {}

Graph::Graph(int n, std::vector<double> rho) : n(n), adjMatrix(n, std::vector<bool>(n, false)), rhoTab(rho), positions(n) {}

void Graph::addEdge(int u, int v)
{
  adjMatrix[u][v] = true;
  adjMatrix[v][u] = true;
}

void Graph::setRho(const int u, const double rho)
{
  rhoTab[u] = rho;
}

void Graph::setRhos(const std::vector<double> rho)
{
  rhoTab = rho;
}

double Graph::getRho(const int u) const
{
  return rhoTab[u];
}

void Graph::setPosition(const int u, const std::vector<double> pos)
{
  positions[u] = pos;
}

std::vector<double> Graph::getPosition(const int u)
{
  return positions[u];
}

bool Graph::areAdjacent(int u, int v) const
{
  return adjMatrix[u][v];
}

int Graph::getNumVertices() const
{
  return n;
}

// Implementation of the getNeighbors method
std::vector<int> Graph::getNeighbors(int vertex) const
{
  std::vector<int> neighbors;
  for (int i = 0; i < n; ++i)
  { // Use n instead of numVertices
    if (adjMatrix[vertex][i])
    {
      neighbors.push_back(i);
    }
  }
  return neighbors;
}

// Implementation of the getColoredNeighbors method

std::vector<int> Graph::getColoredNeighbors(int v, const std::vector<int> &colorClass) const
{
  // Check if vertex v belongs to the color class
  if (std::find(colorClass.begin(), colorClass.end(), v) == colorClass.end())
  {
    throw std::invalid_argument("Vertex does not belong to the specified color class.");
  }

  std::vector<int> coloredNeighbors;

  // Iterate over all vertices in the color class
  for (int u : colorClass)
  {
    if (u != v && areAdjacent(v, u))
    { // Check if u is a neighbor of v in the graph
      coloredNeighbors.push_back(u);
    }
  }

  return coloredNeighbors; // Return the list of neighbors of the same color
}

// Function to print all edges in the graph
void Graph::printEdges() const
{
  std::cout << "Graph edges:" << std::endl;
  for (int i = 0; i < n; ++i)
  {
    for (int j = i + 1; j < n; ++j)
    { // Only check upper triangle of matrix to avoid duplicate edges
      if (adjMatrix[i][j])
      {
        std::cout << "(" << i << ", " << j << ")" << std::endl;
      }
    }
  }
}

// Function to print the adjacency matrix to an output stream
void Graph::print(std::ostream &out) const
{
  for (const auto &row : adjMatrix)
  {
    for (bool val : row)
    {
      out << (val ? 1 : 0) << " ";
    }
    out << "\n";
  }
}
