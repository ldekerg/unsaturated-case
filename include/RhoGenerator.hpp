#ifndef RHO_GENERATOR_HPP
#define RHO_GENERATOR_HPP

#include "Graph.hpp"

/**
 * Generate Rhos using a uniform probability
 * @param n number of nodes
 * @param inf inferior bound for rhos
 * @param sup superior bound for rhos
 */
std::vector<double> generateUniformRhos(int n, double inf, double sup);

/**
 * Generate Rhos rounded to 0.1 using a uniform probability
 * @param n number of nodes
 * @param inf inferior bound for rhos
 * @param sup superior bound for rhos
 */
std::vector<double> generateUniformRhosRounded(int n, double inf, double sup);

/**
 * Generate Rhos using a Gaussian Distribution
 * @param n number of nodes
 * @param mu, sigma parameter of the Gaussian distribution
 */
std::vector<double> generateGaussianRhos(int n, double mu, double sigma);

/**
 * Generate Rhos using two Gaussian distributions
 * @param n nulber of node
 * @param mu1, sigma1 parameter of the first Gaussian distribution
 * @param mu2, sigma2 parameter of the second Gaussian distrubtion
 * @param ratio ratio of nodes generated using the first distribution over the second
 */
std::vector<double> generateDoubleGaussianRhos(int n, double mu1, double sigma1, double mu2, double sigma2, double ratio);

#endif // GRAPH_GENERATOR_HPP
