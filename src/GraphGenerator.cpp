// src/GraphGenerator.cpp
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include "GraphGenerator.hpp"
#include "utils.hpp"


// Function to generate a random unit disk graph
Graph generateUnitDiskGraph(int n, double r)
{
  Graph G(n);
  std::vector<std::pair<double, double>> points(n);

  // Generate random points
  for (int i = 0; i < n; ++i)
  {
    points[i] = {getRandom(), getRandom()};
    G.setPosition(i, {points[i].first, points[i].second, 0.0});
  }

  double numberEdges = 0;
  // Connect points based on the distance threshold (r)
  for (int i = 0; i < n; ++i)
  {
    for (int j = i + 1; j < n; ++j)
    {
      double distance = sqrt(pow(points[i].first - points[j].first, 2) +
                             pow(points[i].second - points[j].second, 2));
      if (distance <= 2 * r)
      {
        G.addEdge(i, j);
        numberEdges = numberEdges + 1;
      }
    }
  }
  // double density = 2*numberEdges /(n*(n-1));
  // std::cout <<  "Density : " << density << std::endl ;
  return G;
}

Graph generateUnitDiskGraph(int n, double r, std::vector<double> rhos)
{
  Graph G = generateUnitDiskGraph(n, r);
  G.setRhos(rhos);
  return G;
}


// Function to generate graphs and save them to a file
Graph generateUnitDiskGraphsToFile( int n, double r, const std::string &filename)
{
  std::ofstream outFile(filename);

  if (!outFile.is_open()) {
      std::cerr << "Error: Unable to open file " << filename << std::endl;
      return Graph(n);
  }

  
  Graph G = generateUnitDiskGraph(n, r);
  for (int u=0; u < n-1; u++)
  {
    for (int v = u+1; v < n; v++){
      if (G.areAdjacent(u,v)) outFile << u << " " << v << "\n";
    }
  }

  outFile.close();
  std::cout << "Graph has been written to " << filename << std::endl;
  return G;
}

// Function to generate a random unit ball graph
Graph generateUnitBallGraph(int n, double r)
{
  Graph G(n);
  std::vector<std::vector<double>> points(n); // Store points in 3D

  // Generate random points in the unit ball
  // Generate random points
  for (int i = 0; i < n; ++i)
  {
    points[i] = {getRandom(), getRandom(), getRandom()};
    G.setPosition(i, points[i]);
  }

  float numberEdges = 0;
  // Connect points based on the distance threshold (r)
  for (int i = 0; i < n; ++i)
  {
    for (int j = i + 1; j < n; ++j)
    {
      double dx = points[i][0] - points[j][0];
      double dy = points[i][1] - points[j][1];
      double dz = points[i][2] - points[j][2];
      double distance = std::sqrt(dx * dx + dy * dy + dz * dz);

      if (distance <= 2 * r)
      { // Adjusting for radius
        G.addEdge(i, j);
        numberEdges = numberEdges + 1;
      }
    }
  }
  return G;
}

// Helper function to tokenize a string based on a delimiter
std::vector<std::string> tokenize(std::string line, std::string del)
{
  std::vector<std::string> tokens;
  size_t pos = 0;
  std::string token;
  while ((pos = line.find(del)) != std::string::npos)
  {
    token = line.substr(0, pos);
    tokens.push_back(token);
    line.erase(0, pos + del.size());
  }
  tokens.push_back(line);

  return tokens;
}

// Function to generate a graph form a graphml file
Graph fromGraphml(const std::string filename)
{
  std::ifstream graphml(filename);
  std::string line;

  std::unordered_map<std::string, int> nodes;
  int nextID = 0;
  std::vector<std::pair<std::string, std::string>> edges;

  while (getline(graphml, line))
  {
    // tokenize line
    std::vector<std::string> tokens = tokenize(line, " ");

    if (tokens[0] == "<node")
    {
      std::string id = tokenize(tokens[1], "\"")[1];
      nodes[id] = nextID++;
    }

    if (tokens[0] == "<edge")
    {
      std::string source = tokenize(tokens[1], "\"")[1];
      std::string target = tokenize(tokens[2], "\"")[1];

      // in case a node wasn't defined explicitly with a <node>
      if (nodes.count(source) == 0)
        nodes[source] = nextID++;
      if (nodes.count(target) == 0)
        nodes[target] = nextID++;

      edges.push_back(std::make_pair(source, target));
    }
  }

  Graph G(nextID);
  for (std::pair<std::string, std::string> e : edges)
  {
    G.addEdge(nodes[e.first], nodes[e.second]);
  }

  return G;
}
