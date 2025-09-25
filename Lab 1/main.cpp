#include <iostream>
#include "integration.h"

using namespace std;

const int N1 = 100;
const int N2 = 1000;
const int N3 = 10000;
const int N4 = 100000;

double fx(double x) {
    return x * x;
}

int main() {
    float start, end;
    cout << "Enter intervals separated by spaces: ";
    cin >> start >> end;

    // Вычисление
    double result1 = integration::MonteCarlo(fx, start, end, N1);
    double result2 = integration::MonteCarlo(fx, start, end, N2);
    double result3 = integration::MonteCarlo(fx, start, end, N3);
    double result4 = integration::MonteCarlo(fx, start, end, N4);
    cout << "Monte Carlo Integration Result (N1): " << result1 << endl;
    cout << "Monte Carlo Integration Result (N2): " << result2 << endl;
    cout << "Monte Carlo Integration Result (N3): " << result3 << endl;
    cout << "Monte Carlo Integration Result (N4): " << result4 << endl;
    system("pause");
    return 0;
}