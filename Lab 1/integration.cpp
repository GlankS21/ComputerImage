#include "integration.h"
#include <iostream>
#include <random>
#include <ctime>
#include <stdexcept> 

using namespace std;

static default_random_engine generator(random_device{}());
static uniform_real_distribution<double> distribution(0.0, 1.0);

// 1. Метод Простого Монте-Карло
double integration::MonteCarlo(
    function<double(double)> function,
    double start,
    double end,
    int pointCount) 
{
    // Integral = (b-a)/N * sum(f(ui))
    double sum = 0;   
    for (int i = 0; i < pointCount; ++i) {
        // x = a + (b - a) * U(0, 1)
        double x = start + (end - start) * distribution(generator); 
        sum += function(x);
    }
    return (end - start) * sum / pointCount;
}

// 2. Метод Монте-Карло со Стратификацией (ИСПРАВЛЕНО)
double integration::StratificationMonteCarlo( 
    function<double(double)> function,
    double start,
    double end,
    int pointCount,
    int stratCount)
{
    if (stratCount <= 0 || pointCount <= 0) throw invalid_argument("stratCount and pointCount must be greater than zero");
    
    // Рассчитываем длину слоя
    double stratumLength = (end - start) / stratCount;
    // Рассчитываем количество выборок на один слой (N_j = N / m)
    int N_per_stratum = pointCount / stratCount;
    if (N_per_stratum == 0) N_per_stratum = 1; 

    double sum = 0;
    for (int i = 0; i < stratCount; i++){
        double localA = start + i * stratumLength;
        double localB = localA + stratumLength;
        if (i == stratCount - 1) localB = end;
        sum += MonteCarlo(function, localA, localB, N_per_stratum);
    }
    return sum;
}

// 3. Интегрированием методом Монте-Карло с выборкой по значимости
double integration::ImportanceSamplingMonteCarlo(
    function<double(double)> function,
    double start,
    double end,
    int pointCount,
    std::function<double(double)> pdf) 
{
    double sum = 0; 

    // I = (b-a)/N * sum(f(Xi)/p(Xi))
    for (int i = 0; i < pointCount; i++) {
        double x = start + (end - start) * distribution(generator); 
        double pdf_value = pdf(x);
        if (pdf_value == 0.0) continue; 
        double ratio = function(x) / pdf_value;
        sum += ratio;
    }

    return sum / pointCount;
}

double integration::MultipleImportanceSamplingMonteCarlo(
    function<double(double)> function,
    double start,
    double end,
    int pointCount,
    std::vector<std::function<double(double)>> pdfs,
    int weightFunctionOption) 
{
     // N_j = N / m
    int N_per_pdf = pointCount / 2;
    if (N_per_pdf == 0) N_per_pdf = 1;

    double total_sum = 0;
    
    for (size_t j = 0; j < pdfs.size(); ++j) {
        double sum_j = 0;
        for (int i = 0; i < N_per_pdf; ++i) {
            double x = start + (end - start) * distribution(generator); 

            double f_x = function(x);
            double p1_x = pdfs[0](x);
            double p2_x = pdfs[1](x);
            
            double w_j_x;
            double p_j_x; 

            if (j == 0) { 
                p_j_x = p1_x;
                if (weightFunctionOption == 1) w_j_x = p1_x / (p1_x + p2_x);
                else if (weightFunctionOption == 2) w_j_x = (p1_x * p1_x) / (p1_x * p1_x + p2_x * p2_x);
                else throw invalid_argument("Invalid weightFunctionOption. Use 1 or 2.");
            } else { 
                p_j_x = p2_x;
                if (weightFunctionOption == 1) w_j_x = p2_x / (p1_x + p2_x);
                else if (weightFunctionOption == 2) w_j_x = (p2_x * p2_x) / (p1_x * p1_x + p2_x * p2_x);
                else throw invalid_argument("Invalid weightFunctionOption. Use 1 or 2.");
            }
            
            if (p_j_x != 0) sum_j += (f_x * w_j_x) / p_j_x;
        }
        total_sum += sum_j / N_per_pdf;
    }
    return total_sum;
}

// 5.	Интегрированием методом Монте-Карло с использованием русской рулетки
double integration::RussianRouletteMonteCarlo(
    function<double(double)> function,
    double start,
    double end,
    int pointCount,
    double R)
{
    if (pointCount <= 0 || R <= 0 || R > 1) throw invalid_argument("pointCount must be greater than zero, R must be (0, 1]");
    double sum = 0; 
    // I = (b - a)/N * sum(f(xi) / R) if xi < R
    for (int i = 0; i < pointCount; i++) {
        double X = start + (end - start) * distribution(generator); 
        double Xi = distribution(generator); 
        if (Xi < R) sum += function(X) / R;
    }
    return (end - start) * sum / pointCount;
}