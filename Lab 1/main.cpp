#include "integration.h"
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <functional>
#include <stdexcept>
#include <string>

using namespace std;

const double A = 2.0;
const double B = 5.0;
const vector<int> N_samples = {100, 1000, 10000, 100000}; 

double function_f(double x) {
    return x * x;
}

double calculate_analytical_integral(double a, double b) {
    return (pow(b, 3) / 3.0) - (pow(a, 3) / 3.0);
}

void print_results(const string& method_name, double integral_value, double true_integral, int N) {
    double absolute_error = abs(integral_value - true_integral);
    double error_estimate = true_integral / sqrt(N); 
    cout << left << setw(10) << N
              << setw(20) << integral_value
              << setw(20) << absolute_error
              << setw(20) << error_estimate << " | "
              << method_name << endl;
}

int main() {
    cout << fixed << setprecision(10);
   
    integration integrator; 
    double true_integral = calculate_analytical_integral(A, B);
    cout << "===================================================================================================================" << endl;
    cout << "Integral (I_true) of f(x)=x^2 over [" << A << ", " << B << "]: " << true_integral << endl;
    cout << "===================================================================================================================" << endl;
    cout << left << setw(10) << "N" 
              << setw(20) << "I_MC" 
              << setw(20) << "|I-I_true|" 
              << setw(20) << "Error estimates" 
              << "| Method" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;

    for (int N : N_samples) {
        try {
            double result = integrator.MonteCarlo(function_f, A, B, N);
            print_results(" MonteCarlo", result, true_integral, N);
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;

    int strat_count_1 = static_cast<int>(round((B - A) / 1.0)); 
    int strat_count_05 = static_cast<int>(round((B - A) / 0.5));

    for (int N : N_samples) {
        try {
            double res1 = integrator.StratificationMonteCarlo(function_f, A, B, N, strat_count_1);
            print_results("Stratified MC (step=1.0, m=3)", res1, true_integral, N);
            double res05 = integrator.StratificationMonteCarlo(function_f, A, B, N, strat_count_05);
            print_results("Stratified MC (step=0.5, m=6)", res05, true_integral, N);
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }

    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
     auto pdf1_x = [](double x) { return x / 10.5; }; 
    auto pdf2_x2 = [](double x) { return x * x / 39.0; }; 
    auto pdf3_x3 = [](double x) { return x * x * x / 152.25; }; 
    for (int N : N_samples) {
        try {
            double res1 = integrator.ImportanceSamplingMonteCarlo(function_f, A, B, N, pdf1_x);
            print_results("IS (p(x)=x)", res1, true_integral, N);
            double res2 = integrator.ImportanceSamplingMonteCarlo(function_f, A, B, N, pdf2_x2);
            print_results("IS (p(x)=x^2)", res2, true_integral, N);
            double res3 = integrator.ImportanceSamplingMonteCarlo(function_f, A, B, N, pdf3_x3);
            print_results("IS (p(x)=x^3)", res3, true_integral, N);
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    std::vector<std::function<double(double)>> mis_pdfs = {pdf1_x, pdf3_x3};

    for (int N : N_samples) {
        try {
            double res_opt1 = integrator.MultipleImportanceSamplingMonteCarlo(function_f, A, B, N, mis_pdfs, 1);
            print_results("MIS (p(x), p(x)^3) - Weight Opt 1", res_opt1, true_integral, N);
            double res_opt2 = integrator.MultipleImportanceSamplingMonteCarlo(function_f, A, B, N, mis_pdfs, 2);
            print_results("MIS (p(x), p(x)^3) - Weight Opt 2", res_opt2, true_integral, N);
        } catch (const exception& e) {
            cerr << e.what() << endl;
        }
    }
    cout << "-------------------------------------------------------------------------------------------------------------------" << endl;
    const vector<double> R_values = {0.5, 0.75, 0.95}; 
    for (int N : N_samples) {
        for (double R : R_values) {
            try {
                double result = integrator.RussianRouletteMonteCarlo(function_f, A, B, N, R);
                string method_name = "RR (R=" + to_string(R) + ")";
                print_results(method_name, result, true_integral, N);
            } catch (const exception& e) {
                cerr << e.what() << endl;
            }
        }
    }
    cout << "===================================================================================================================" << endl;

    return 0;
}