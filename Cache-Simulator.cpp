#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

bool powerOfTwo(int x){
	return x * (!(x & (x - 1)));
}

int setarNumeroDeBlocos() {
    int tamanhoDaCache = 0;

    while(!powerOfTwo(tamanhoDaCache)) {
        cout << "Digite o número de blocos da cache: ";
        cin >> tamanhoDaCache;
    }
    
    return tamanhoDaCache;
}

int setarNumeroDeSets() {
    int tamanhoDaCache = 0;

    while(!powerOfTwo(tamanhoDaCache)) {
        cout << "Digite o número de sets do bloco: ";
        cin >> tamanhoDaCache;
    }
    
    return tamanhoDaCache;
}

int setarTamanhoDaMemoria() {
    int tamanhoDaMemoria = 0;

    while(!powerOfTwo(tamanhoDaMemoria)) {
        cout << "Digite o tamanho da memória: ";
        cin >> tamanhoDaMemoria;
    }
    
    return tamanhoDaMemoria;
}

int* freshMem(int n){
	int* array = new int[n];
	for(int i = 0; i < n; i++) array[i] = 0;
	return array;
}

void lerArquivo(int* memoria, int tamanhoDaMemoria) {
    FILE* arquivo = fopen("t4.txt", "r");

    if (arquivo == NULL) {
        cout << "Erro ao abrir o arquivo" << endl;
        return;
    }

    for (int i = 0; i < tamanhoDaMemoria; i++) {
        fscanf(arquivo, "%d", &memoria[i]);
    }

    fclose(arquivo);
}


void simularCache(int numeroDeBlocos, int tamanhoDaMemoria, int numeroDeSets) {
    
    
}

int main() {
    srand(time(NULL));

    int numeroDeBlocos = setarNumeroDeBlocos();
    int tamanhoDaMemoria = setarTamanhoDaMemoria();
    int numeroDeSets = setarNumeroDeSets();

    simularCache(numeroDeBlocos, tamanhoDaMemoria, numeroDeSets);
                                                                                                                                                                                                                                                                                                                                                                                           
    return 0;
}