#ifndef INTEGRATION_H
#define INTEGRATION_H

#include<functional>
#include<random>

using namespace std;

class integration{
public:
    static double MonteCarlo(
        function<double(double)> function,
        double start,
        double end,
        int pointCount
    );
    static double StratificationMonteCarlo(
        function<double(double)> function,
        double start,
        double end,
        int pointCount,
        int stratCount
    )
};

#endif