#include <iostream>
#include <omp.h>
#include <fstream>
using namespace std;
//ryzen7 5800h 2.5GHz | 8 núcleos | 16 threads | 16GB RAM | Linux ubuntu 26.04

bool ehPrimo(long long numero) {
    if (numero < 2) return false;

    for (long long divisor = 2; divisor <= numero / divisor; divisor++) {
        if (numero % divisor == 0) {
            return false;
        }
    }

    return true;
}

long long contarSequencial(long long limite) {
    long long quantidade = 0;

    for (long long numero = 2; numero <= limite; numero++) {
        if (ehPrimo(numero)) {
            quantidade++;
        }
    }

    return quantidade;
}

long long contarParalelo(long long limite, int threads) {
    long long quantidade = 0;

    #pragma omp parallel for num_threads(threads) reduction(+:quantidade)
    for (long long numero = 2; numero <= limite; numero++) {
        if (ehPrimo(numero)) {  
            quantidade++;
        }
    }

    return quantidade;
}

int main() {
    long long limite;

    cout << "Digite o limite: ";
    cin >> limite;

    double inicio = omp_get_wtime();
    long long primosSequencial = contarSequencial(limite);
    double tempoSequencial = omp_get_wtime() - inicio;

    cout << "\nVersao sequencial:\n";
    cout << "Threads: 1\n";
    cout << "Primos encontrados: " << primosSequencial << '\n';
    cout << "Tempo: " << tempoSequencial << " segundos\n";

    ofstream arquivo("resultados.csv");
    arquivo << "threads,tempo_s,primos\n";
    arquivo << "1," << tempoSequencial << "," << primosSequencial << "\n";
    
    int quantidadeThreads[] = {2, 4, 6, 8, 16};

    cout << "\nVersoes paralelas:\n";

    for (int threads : quantidadeThreads) {
        inicio = omp_get_wtime();
        long long primosParalelo = contarParalelo(limite, threads);
        double tempoParalelo = omp_get_wtime() - inicio;

        double speedup = tempoSequencial / tempoParalelo;

        cout << "\nThreads: " << threads << '\n';
        cout << "Primos encontrados: " << primosParalelo << '\n';
        cout << "Tempo: " << tempoParalelo << " segundos\n";
        cout << "Speed-up: " << speedup << '\n';

        arquivo << threads << ","  << tempoParalelo << ","<< primosParalelo << "\n";

        if (primosParalelo != primosSequencial) {
            cout << "Erro: os resultados foram diferentes!\n";
        }
    }


    
    return 0;
}