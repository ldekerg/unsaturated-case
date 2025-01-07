#include "GraphGenerator.hpp"
#include "Graph.hpp"
#include "RhoGenerator.hpp"
#include "PerformanceEvaluation.hpp"
#include "ConflictMinizationChannelAllocation.hpp"
#include "test.hpp"

#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

int main(){
  // Graph G = fromGraphml("graph/config_6.graphml");
  // G.setRhos(generateUniformRhos(G.getNumVertices(), 0.2, 0.8));
  // for (int i = 0; i < G.getNumVertices(); i++){
  //   cout << i << ": ";
  //   for (int n : G.getNeighbors(i)){
  //     cout<<n<<", ";
  //   }
  //   cout << endl;
  // }
  // vector<int> subgraph;
  // for (int i = 0; i < G.getNumVertices(); i++) subgraph.push_back(i);
  // vector<vector<int>> cliques = findAllCliquesIncludingNode2(G, 4,subgraph);
  // for (const auto& clique_1 : cliques)
  // {
  //   cout << "[";
  //   for (const auto &v : clique_1)
  //   {
  //     cout << v << ",";
  //   }
  //   cout << "],";
  // }
  // cout << endl;
  // vector<vector<int>> allocation = ConflictMinizationChannelAllocation(G, 11);
  // std::vector<double> performance = unsaturatedPerformanceModel(G, allocation);

  // Graph G = fromGraphml("graph/config_6.graphml");
  // G.setRhos({0.3,0.4,0.9,0.5,0.9,0.9,0.4,0.8,0.1,0.4,0.5,0.4,0.7});
  
  // for (int i = 0; i < G.getNumVertices(); i++){
  //   cout << i << ": ";
  //   for (int n : G.getNeighbors(i)){
  //     cout<<n<<", ";
  //   }
  //   cout << endl;
  // }

  runTest(200, 11, 20, 2);


  return 0;
}