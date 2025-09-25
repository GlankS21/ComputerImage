#include "integration.h"
#include<iostream>
#include<random>
#include<ctime>

using namespace std;

static default_random_engine generator(time(0));
static uniform_real_distribution<double> distribution(0.0, 1.0);

double integration::MonteCarlo(
    function<double(double)> function,
    double start,
    double end,
    int pointCount) 
{
    double sum = 0;   
    for (int i = 0; i < pointCount; ++i) {
        double x = start + (end - start) * distribution(generator);
        sum += function(x);
    }
    return (end - start) * sum / pointCount;
}