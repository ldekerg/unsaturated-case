#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <iostream> 

class Graph {
public:
    /**
     * Create a Graph object
     * @param n number of vertices
     */
    Graph(int n);

    /**
     * Create a Graph object
     * @param n number of vertices
     * @param rhos list of rhos for the vertices of the graph
     */
    Graph(int n, std::vector<double> rhos);

    /**
     * Add an edge between two vertices
     */
    void addEdge(int u, int v);

    /**
     * Set rho for one vertex
     */
    void setRho(int v, double rho);

    /**
     * Set rhos for all the graph
     */
    void setRhos(std::vector<double> rhos);

    /**
     * Get rho for one vertex
     */
    double getRho(int) const;

    /**
     * Set the geographic position of one vertex 
     * @param v vertex of the graph
     * @param pos 3D position vector
     */
    void setPosition(int v, std::vector<double> pos);

    /**
     * Get the position of one vertex
     */
    std::vector<double> getPosition(int);

    /**
     * Checks if two vertices are adjacent
     */
    bool areAdjacent(int, int) const;

    /**
     * Get the number of vertices of the graph
     */
    int getNumVertices() const;

    /**
     * Get the neighbors of one vertex
     * @return a vector of neighbors of the vertex 
     */
    std::vector<int> getNeighbors(int) const;

    /**
     * Get the neighbors of one vertex of the same color 
     * @param v the vertex
     * @param colorClass vector of vertices belonging to the color class
     */
    std::vector<int> getColoredNeighbors(int v, const std::vector<int>& colorClass) const;
    
    void printEdges() const;
    void print(std::ostream&) const;

private:
    int n;  // Number of vertices
    std::vector<std::vector<bool>> adjMatrix;  // Adjacency matrix
    std::vector<double> rhoTab; // Charges
    std::vector<std::vector<double>> positions;
};

#endif // GRAPH_HPP