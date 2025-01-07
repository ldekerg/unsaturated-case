#include "test.hpp"
#include "Graph.hpp"
#include "GraphGenerator.hpp"
#include "SubcoloringChannelAllocation.hpp"
#include "ConflictMinizationChannelAllocation.hpp"
#include "ThroughputMaximizationChannelAllocation.hpp"
#include "PerformanceEvaluation.hpp"
#include "RhoGenerator.hpp"

#include <vector>
#include <fstream>
#include <string>
#include <iostream>


void runTest(int graphSize, int nbColor, int nbRepet, int testingKey)
{

  // List of vertices
  std::vector<int> vertices(graphSize);
  for (int i = 0; i < graphSize; ++i)
  {
    vertices[i] = i;
  }

  // Radius corresponding to densities {0,0.05,0.1,...,0.45,0.5}
  std::vector<std::pair<double, double>> Radii = {{0, 0.001}, {0.025, 0.0047}, {0.05, 0.066}, {0.075, 0.083}, {0.1, 0.097}, {0.15, 0.122}, {0.2, 0.144}, {0.25, 0.164}, {0.3, 0.183}, {0.35, 0.202}, {0.4, 0.22}/*, {0.45, 0.238}, {0.5, 0.256}*/};

  std::string paramStr = std::to_string(graphSize) + "_" + std::to_string(nbColor) + "_" + std::to_string(nbRepet) + "_DG3010701050.txt";

  std::string SCAfilename = "results/Perf_SCA_";
  std::ofstream SCAFile(SCAfilename + paramStr);

  // std::string SCAModfilename = "results/Perf_SCAMod_";
  // std::ofstream SCAModFile(SCAModfilename + paramStr);

  // std::string SCAMfilename = "results/Perf_SCAM_";
  // std::ofstream SCAMFile(SCAMfilename + paramStr);

  std::string CMCAfilename = "results/Perf_CMCA_";
  std::ofstream CMCAFile(CMCAfilename + paramStr);

  // std::string CMCASfilename = "results/Perf_CMCAS_";
  // std::ofstream CMCASFile(CMCASfilename + paramStr);

  // std::string CMCASMfilename = "results/Perf_CMCASM_";
  // std::ofstream CMCASMFile(CMCASMfilename + paramStr);

  std::string TMCAfilename = "results/Perf_TMCA_";
  std::ofstream TMCAFile(TMCAfilename + paramStr);

  std::string TMCACMCAfilename = "results/Perf_TMCACMCA_";
  std::ofstream TMCACMCAFile(TMCACMCAfilename + paramStr);

  std::string TMCASfilename = "results/Perf_TMCAS_";
  std::ofstream TMCASFile(TMCASfilename + paramStr);

  // std::string TMCASMfilename = "results/Perf_TMCASM_";
  // std::ofstream TMCASMFile(TMCASMfilename + paramStr);

  for (int i = 0; i < Radii.size(); i++)
  {
    double density = Radii[i].first;
    double radius = Radii[i].second;
    std::cout << "Density " << density << std::endl;
    for (int r = 0; r < nbRepet; ++r)
    {
      std::cout << r << std::endl;
      // std::vector<double> rhos = generateUniformRhosRounded(graphSize, 0.2, 0.8);
      // std::vector<double> rhos = generateGaussianRhos(graphSize, 0.99, 0.01);
      std::vector<double> rhos = generateDoubleGaussianRhos(graphSize, 0.3, 0.1, 0.7, 0.1, 0.5);
      std::string paramIndStr = std::to_string(graphSize)+"_"+std::to_string(nbColor)+"_"+std::to_string(density)+"_"+std::to_string(r);
      std::string edgeFilename = "/home/chassin/testing_data/graph/"+std::to_string(testingKey)+"/UnitDisk_"+paramIndStr+".txt";
      // Graph G = generateUnitDiskGraphsToFile(graphSize, radius, edgeFilename);
      Graph G = generateUnitDiskGraph(graphSize, radius);
      G.setRhos(rhos);

      

      // Run SCA
      std::vector<std::vector<std::vector<int>>> SCAPartition = SubcoloringChannelAllocation(G, nbColor);
      // Run CMCA
      std::vector<std::vector<int>> CMCAPartition = ConflictMinizationChannelAllocation(G, nbColor);
      // run CMCAS
      // std::vector<std::vector<int>> CMCASPartition = ConflictMinizationChannelAllocationSaturated(G, nbColor);
      // Run TMCA
      std::vector<std::vector<int>> TMCAPartition = ThroughputMaximizationChannelAllocation(G, nbColor);
      // Run TMCA with CMCA
      std::vector<std::vector<int>> TMCACMCAPartition = ThroughputMaximizationChannelAllocationWithCMCA(G, nbColor);
      // Run TMCAS
      std::vector<std::vector<int>> TMCASPartition = ThroughputMaximizationChannelAllocationSaturated(G, nbColor);

      // Performances
      std::vector<double> performanceSCA = unsaturatedPerformanceModelForSubcoloring(G, SCAPartition);
      // std::vector<double> performanceSCAMod = unsaturatedPerformanceModel(G, flatten(SCAPartition));
      // std::vector<double> performanceSCAM = evaluatePerformanceArbitrarySaturated(G,SCAPartition);
      std::vector<double> performanceCMCA = unsaturatedPerformanceModel(G, CMCAPartition);
      // std::vector<double> performanceCMCAS = unsaturatedPerformanceModel(G, CMCASPartition);
      // std::vector<double> performanceCMCASM = evaluatePerformanceArbitrarySaturated(G, connectedComponentsPartitions(G, CMCASPartition));
      std::vector<double> performanceTMCA = unsaturatedPerformanceModel(G, TMCAPartition);
      std::vector<double> performanceTMCACMCA = unsaturatedPerformanceModel(G, TMCACMCAPartition);
      std::vector<double> performanceTMCAS = unsaturatedPerformanceModel(G, TMCASPartition);
      // std::vector<double> performanceTMCASM = evaluatePerformanceArbitrarySaturated(G, connectedComponentsPartitions(G, TMCASPartition));


      // // Graph to Individual file
      // std::ofstream outFile("/home/chassin/testing_data/graph/"+std::to_string(testingKey)+"/Rhos_"+paramIndStr+".txt");
      // if (!outFile.is_open()) {
      //     std::cerr << "Error: Unable to open file " << "/home/chassin/testing_data/graph/"+std::to_string(testingKey)+"/Rhos_"+paramIndStr+".txt" << std::endl;
      //     return;
      // }
      // for (int u=0; u < graphSize; u++)
      // {
      //   outFile << u << " " << rhos[u] << "\n";
      // }
      // outFile.close();
      // // Alloc to individual file
      // std::string SCAallocFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/allocation/SCA"+paramIndStr+".txt";
      // std::string CMCAallocFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/allocation/CMCA"+paramIndStr+".txt";
      // std::string TMCAallocFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/allocation/TMCA"+paramIndStr+".txt";
      // std::ofstream SCAallocFile(SCAallocFilename);
      // std::ofstream CMCAallocFile(CMCAallocFilename);
      // std::ofstream TMCAallocFile(TMCAallocFilename);
      // for (int i = 0; i < nbColor; i++) {
      //   for (const auto& cl : SCAPartition[i]) {
      //     for (int v : cl)
      //     {
      //       SCAallocFile << v << " " << i << std::endl;
      //     }
      //   }
      //   for (int v : CMCAPartition[i]) {
      //     CMCAallocFile << v << " " << i << std::endl;
      //   }
      //   for (int v : TMCAPartition[i]) {
      //     TMCAallocFile << v << " " << i << std::endl;
      //   }
      // }
      // SCAallocFile.close();
      // CMCAallocFile.close();
      // TMCAallocFile.close();

      // // Perf to individual file
      // std::string SCAPerfFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/performance/SCA"+paramIndStr+".txt";
      // std::string CMCAPerfFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/performance/CMCA"+paramIndStr+".txt";
      // std::string TMCAPerfFilename = "/home/chassin/ns3-results/"+std::to_string(testingKey)+"/performance/TMCA"+paramIndStr+".txt";
      // std::ofstream SCAPerfFile(SCAPerfFilename);
      // std::ofstream CMCAPerfFile(CMCAPerfFilename);
      // std::ofstream TMCAPerfFile(TMCAPerfFilename);
      // for (int v : vertices)
      // {
      //   SCAPerfFile << v << " " << performanceSCA[v] << std::endl;
      //   CMCAPerfFile << v << " " << performanceCMCA[v] << std::endl;
      //   TMCAPerfFile << v << " " << performanceTMCA[v] << std::endl;
      // }
      // SCAPerfFile.close();
      // CMCAPerfFile.close();
      // TMCAPerfFile.close();


      // Perf to global file
      for (int v : vertices)
      {
        SCAFile << performanceSCA[v] << std::endl;
        // SCAModFile << performanceSCAMod[v] << std::endl;
        // SCAMFile << performanceSCAM[v] << std::endl;
        CMCAFile << performanceCMCA[v] << std::endl;
        // CMCASFile << performanceCMCAS[v] << std::endl;
        // CMCASMFile << performanceCMCASM[v] << std::endl;
        TMCAFile << performanceTMCA[v] << std::endl;
        TMCACMCAFile << performanceTMCACMCA[v] << std::endl;
        TMCASFile << performanceTMCAS[v] << std::endl;
        // TMCASMFile << performanceTMCASM[v] << std::endl;
      }

      SCAFile << std::endl;
      // SCAModFile << std::endl;
      // SCAMFile << std::endl;
      CMCAFile << std::endl;
      // CMCASFile << std::endl;
      // CMCASMFile << std::endl;
      TMCAFile << std::endl;
      TMCACMCAFile << std::endl;
      TMCASFile << std::endl;
      // TMCASMFile << std::endl;
    }
  }

  SCAFile.close();
  // SCAModFile.close();
  // SCAMFile.close();
  CMCAFile.close();
  // CMCASFile.close();
  // CMCASMFile.close();
  TMCAFile.close();
  TMCACMCAFile.close();
  TMCASFile.close();
  // TMCASMFile.close();

  std::cout << "Done" << std::endl;
}