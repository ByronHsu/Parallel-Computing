#include <bits/stdc++.h>
using namespace std;

double a = 0, b = 1, n = 50;

double f(double x){
    return (4 / (1 + x * x));
}

double xi(double i){
    return (a + (b - a) / n * i);
}

int main(){
    double sum = 0;

    for(int i = 1; i <= n / 2; i++){
        sum = sum + (4 * f(xi(2 * i - 1)) + 2 * f(xi(2 * i)));
    }

    double integral = (1 / (3 * n)) * (f(0) - f(1) + sum);
    printf("ans = %13.11f\n", integral);
}