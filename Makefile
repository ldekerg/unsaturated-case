CXX = g++
CXXFLAGS = -Wall -std=c++14 $(INCLUDE)
INCLUDE = -I include 
GUROBI_INCLUDE = -I $(GUROBI_HOME)/include
LDFLAGS = -L $(GUROBI_HOME)/lib -lgurobi_c++ -lgurobi110

TARGET = main
OBJS = bin/main.o bin/utils.o bin/test.o bin/Graph.o bin/GraphGenerator.o bin/RhoGenerator.o bin/PerformanceEvaluation.o bin/SubcoloringChannelAllocation.o bin/ConflictMinizationChannelAllocation.o bin/ThroughputMaximizationChannelAllocation.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

bin/main.o: src/main.cpp include/test.hpp include/Graph.hpp include/GraphGenerator.hpp  include/PerformanceEvaluation.hpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o bin/main.o

bin/utils.o: src/utils.cpp include/utils.hpp
	$(CXX) $(CXXFLAGS) -c src/utils.cpp -o bin/utils.o

bin/test.o: src/test.cpp include/test.hpp
	$(CXX) $(CXXFLAGS) -c src/test.cpp -o bin/test.o

bin/Graph.o: src/Graph.cpp include/Graph.hpp
	$(CXX) $(CXXFLAGS) -c src/Graph.cpp -o bin/Graph.o

bin/GraphGenerator.o: src/GraphGenerator.cpp include/GraphGenerator.hpp
	$(CXX) $(CXXFLAGS) -c src/GraphGenerator.cpp -o bin/GraphGenerator.o

bin/RhoGenerator.o: src/RhoGenerator.cpp include/RhoGenerator.hpp
	$(CXX) $(CXXFLAGS) -c src/RhoGenerator.cpp -o bin/RhoGenerator.o

bin/PerformanceEvaluation.o: src/PerformanceEvaluation.cpp include/PerformanceEvaluation.hpp
	$(CXX) $(CXXFLAGS) -c src/PerformanceEvaluation.cpp -o bin/PerformanceEvaluation.o

bin/SubcoloringChannelAllocation.o: src/SubcoloringChannelAllocation.cpp include/SubcoloringChannelAllocation.hpp 
	$(CXX) $(CXXFLAGS) -c src/SubcoloringChannelAllocation.cpp -o bin/SubcoloringChannelAllocation.o
	
bin/ConflictMinizationChannelAllocation.o: src/ConflictMinizationChannelAllocation.cpp include/ConflictMinizationChannelAllocation.hpp 
	$(CXX) $(CXXFLAGS) -c src/ConflictMinizationChannelAllocation.cpp -o bin/ConflictMinizationChannelAllocation.o

bin/ThroughputMaximizationChannelAllocation.o: src/ThroughputMaximizationChannelAllocation.cpp include/ThroughputMaximizationChannelAllocation.hpp 
	$(CXX) $(CXXFLAGS) -c src/ThroughputMaximizationChannelAllocation.cpp -o bin/ThroughputMaximizationChannelAllocation.o

clean:
	rm -f $(TARGET) bin/*.o
