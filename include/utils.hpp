#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>

/**
 * Generate a random floating-point number between 0 and 1 using a uniform law
 */
double getRandom();

/**
 * Generate a random int between min (included) and max (excluded)
 */
int getRandomInt(int, int);

/**
 * Generate a random floating number using a Gaussian law
 * @param mu, sigma parameter of the Gaussian law
 */
double getRandomGaussian(double mu, double sigma);

/**
 * Find the first class containing an object
 * @param classes list of classes containing int
 * @param v object to find
 * @return the first vector containting v, a empty vector instead
 */
std::vector<int> findClass(const std::vector<std::vector<int>>& classes, int v);

#endif