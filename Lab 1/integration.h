#ifndef INTEGRATION_H
#define INTEGRATION_H

#include<functional>
#include<random>

using namespace std;

enum WeightingScheme {
    BALANCE_HEURISTIC, 
    POWER_HEURISTIC    
};
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
    );
    static double ImportanceSamplingMonteCarlo(
        std::function<double(double)> function,
        double start,
        double end,
        int pointCount,
        std::function<double(double)> pdf
    );
    static double RussianRouletteMonteCarlo(
        std::function<double(double)> function,
        double start,
        double end,
        int pointCount,
        double R
    );
    static double MultipleImportanceSamplingMonteCarlo(
        std::function<double(double)> function,
        double start,
        double end,
        int pointCount,
        std::vector<std::function<double(double)>> pdfs,
        int weightFunctionOption
    );
};

#endif