#ifndef LINEAR_PROGRAMMING_HPP
#define LINEAR_PROGRAMMING_HPP

#include "Graph.hpp"

#include "/opt/gurobi1103/linux64/include/gurobi_c++.h"

std::vector<std::vector<int>> quadProgramming(const Graph&, int, double timeLimit = 60.0);
std::vector<std::vector<int>> quadProgramming(const Graph&, int, const std::vector<std::vector<int>>&, double timeLimit = 60.0 );


#endif