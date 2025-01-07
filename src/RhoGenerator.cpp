#include "RhoGenerator.hpp"
#include "utils.hpp"

#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

vector<double> generateUniformRhos(int n, double inf, double sup)
{
  srand(time(nullptr));
  vector<double> rhos(n);
  for (int i = 0; i < n; i++)
  {
    rhos[i] = getRandom();
    while(rhos[i] < inf || rhos[i] > sup) rhos[i] = getRandom();
  }
  return rhos;
}

vector<double> generateUniformRhosRounded(int n, double inf, double sup)
{
  srand(time(nullptr));
  vector<double> rhos(n);
  for (int i = 0; i < n; i++)
  {
    rhos[i] = round(getRandom()*10)/10;
    while (rhos[i] < inf || rhos[i] > sup) rhos[i] = round(getRandom()*10)/10;
  }
  return rhos;
}

vector<double> generateGaussianRhos(int n, double mu, double sigma)
{
  vector<double> rhos;
  for (int i = 0; i < n; i++)
  {
    double rho = getRandomGaussian(mu, sigma);
    while (rho < 0 || rho > 1) rho = getRandomGaussian(mu, sigma);
    rhos.push_back(rho);
  }
  return rhos;
}

vector<double> generateDoubleGaussianRhos(int n, double mu1, double sigma1, double mu2, double sigma2, double ratio)
{
  int n1 = round(n*ratio);
  int n2 = n - n1;
  vector<double> rhos = generateGaussianRhos(n1, mu1, sigma1);
  vector<double> rhos2 = generateGaussianRhos(n2, mu2, sigma2);
  rhos.insert(rhos.end(), rhos2.begin(), rhos2.end());
  random_shuffle(rhos.begin(), rhos.end());
  return rhos;
}