#include <iostream>
#include <omp.h>
//compilar: g++ omp.gpp -o omp -fopenmp
using namespace std;

int main(){

#pragma omp parallel num_threads(20)
    {
    cout << "Hello World!" << endl;
    }
    return 0;

}
