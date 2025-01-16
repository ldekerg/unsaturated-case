#include "utils.hpp"
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>

std::default_random_engine generator;

double getRandom()
{
  return static_cast<double>(rand()) / RAND_MAX;
}

int getRandomInt(int min, int max)
{
  return rand() % (max - min) + min;
}

double getRandomGaussian(double mu, double sigma)
{
  std::normal_distribution<double> distribution(mu, sigma);
  return distribution(generator);
}

std::vector<int> findClass(const std::vector<std::vector<int>>& classes, int v)
{
  for (const auto& c: classes)
  {
    if (std::find(c.begin(), c.end(), v) != c.end())
    {
      return c;
    }
  }
  return {};
}


