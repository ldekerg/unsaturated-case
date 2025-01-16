#include "LinearProgramming.hpp"
#include "gurobi_c++.h"

#include <algorithm>

using namespace std;

std::vector<std::vector<int>> quadProgramming(const Graph &G, int K, double timeLimit)
{
  return quadProgramming(G, K, {}, timeLimit);
}

std::vector<std::vector<int>> quadProgramming(const Graph &G, const int K, const std::vector<std::vector<int>> &presolution, double timeLimit)
{
  std::vector<std::vector<int>> partition(K); // k color classes

  try
  {

    // Create an environment
    GRBEnv env = GRBEnv(true);
    env.set("LogFile", "ilp.log");
    env.start();

    // Create an empty model
    GRBModel model = GRBModel(env);

    model.set(GRB_DoubleParam_TimeLimit, timeLimit);

    // Create variables
    GRBVar tabvar[G.getNumVertices()][K];
    for (int v = 0; v < G.getNumVertices(); v++)
    {
      for (int k = 0; k < K; k++)
      {
        string name = to_string(v) + "_" + to_string(k);
        tabvar[v][k] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY, name);
      }
    }
    GRBVar maxRhoClique = model.addVar(GRB_INF_OR_UNBD, GRB_INF_OR_UNBD, 0.0, GRB_CONTINUOUS, "Max_Rho_Clique");

    // Add constraint
    for (int v = 0; v < G.getNumVertices(); v++)
    {
      GRBLinExpr constr = 0;
      for (int k = 0; k < K; k++)
      {
        constr += tabvar[v][k];
      }
      model.addConstr(constr == 1, "Unicité_" + v);
    }

    for (int u = 0; u < G.getNumVertices() - 2; u++)
    {
      for (int v = u + 1; v < G.getNumVertices() - 1; v++)
      {
        for (int w = v + 1; w < G.getNumVertices(); w++)
        {
          if (G.areAdjacent(u, v) && G.areAdjacent(v, w) && !G.areAdjacent(u, w))
          {
            for (int k = 0; k < K; k++)
            {
              model.addConstr(tabvar[u][k] + tabvar[v][k] + tabvar[w][k] <= 2, "P3_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w));
            }
          }
          if (G.areAdjacent(v, u) && G.areAdjacent(u, w) && !G.areAdjacent(v, w))
          {
            for (int k = 0; k < K; k++)
            {
              model.addConstr(tabvar[u][k] + tabvar[v][k] + tabvar[w][k] <= 2, "P3_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w));
            }
          }
          if (G.areAdjacent(u, w) && G.areAdjacent(w, v) && !G.areAdjacent(u, v))
          {
            for (int k = 0; k < K; k++)
            {
              model.addConstr(tabvar[u][k] + tabvar[v][k] + tabvar[w][k] <= 2, "P3_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w));
            }
          }
        }
      }
    }

    for (int u = 0; u < G.getNumVertices(); u++)
    {
      std::vector<int> neighbors = G.getNeighbors(u);
      neighbors.push_back(u);
      for (int k = 0; k < K; k++)
      {
        GRBQuadExpr rhoSum = 0;
        for (int v : neighbors)
        {
          rhoSum += tabvar[u][k] * tabvar[v][k] * G.getRho(v);
        }
        model.addQConstr(rhoSum <= maxRhoClique, "Clique_" + std::to_string(u) + "_" + std::to_string(k));
      }
    }

    // Set presolve values
    for (int i = 0; i < presolution.size(); i++)
    {
      for (int v : presolution[i])
      {
        tabvar[v][i].set(GRB_DoubleAttr_Start, 1.0);
      }
    }

    // Set objective
    GRBQuadExpr objective = 0;
    for (int u = 0; u < G.getNumVertices(); u++)
    {
      for (int v = u; v < G.getNumVertices(); v++)
      {
        if (G.areAdjacent(u, v))
        {
          for (int k = 0; k < K; k++)
          {
            objective += tabvar[u][k] * tabvar[v][k];
          }
        }
      }
    }
    model.setObjective(objective, GRB_MINIMIZE);

    // Optimize model
    model.optimize();

    // Retrieve solution
    for (int v = 0; v < G.getNumVertices(); v++)
    {
      double max = -1;
      int bestClass = 0;
      for (int k = 0; k < K; k++)
      {
        if (tabvar[v][k].get(GRB_DoubleAttr_X) > max)
        {
          max = tabvar[v][k].get(GRB_DoubleAttr_X);
          bestClass = k;
        }
      }
      partition[bestClass].push_back(v);
      // cout << v << "_" << bestClass << " = " << max << endl;
    }

    // Print solution
    /*
    for (int i = 0; i < K; i++) {
      for (int j : partition[i])
        cout << j << "-";
      cout << endl;
    }
    */
  }
  catch (GRBException e)
  {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  }
  catch (...)
  {
    cout << "Exception during optimization" << endl;
  }

  return partition;
}
