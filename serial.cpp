#include <iostream>
#include <omp.h>
//compilar: g++ omp.gpp -o omp -fopenmp
#define SIZE 10000

using namespace std;

int main()
{
    long *a = new long[SIZE];
    long *b = new long[SIZE];
    long *c = new long[SIZE];

    /*fill_n(a, SIZE, 111);
    fill_n(b, SIZE, 222);*/
#pragma omp parallel for
    for (long j = 0; j<SIZE; ++j){
        a[j] = 111;
        b[j] = 222;
    }
#pragma omp parallel for
    for(long i=0; i<SIZE; i++){
        c[i] = a[i] * b[i] + a[i] * b[i];
    }
    delete [] a;
    delete [] b;
    delete [] c;

    return 0;

}
