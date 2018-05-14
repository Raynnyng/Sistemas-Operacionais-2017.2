#include <bits/stdc++.h>
#define QTD_THREADS 100
#define VALOR 10000

using namespace std;
mutex mutx;
condition_variable var_cond;

volatile int solicitacao = 0;
volatile int resposta = 0;
volatile bool pronto = false;

volatile int atual = -1;
volatile bool sinal = false;
volatile int contador_thread = 0;

volatile clock_t tempoEscalonador;

int somatorio(int k){
    int soma = k;
    for(int i = 0; i < k; i++)
        soma += i;
    return soma;
}

void servidor(int id){
    while(true){
        if(sinal){
		resposta = somatorio(solicitacao);
		sinal = false;
		contador_thread++;
        }
        if(contador_thread == QTD_THREADS)
            	break;
    }
}

void cliente(int id){
    clock_t inicio, fim;
    double duracao, espera;
    inicio = clock();
    unique_lock<mutex> trava(mutx);
    while(id != atual or !pronto)
        var_cond.wait(trava);
    solicitacao = rand() % VALOR + 1;
    cout << "Thread: ";
    cout << id << endl;
    cout << "Valor de Entrada: " << solicitacao << endl;
    sinal = true;
    while(sinal);
    cout << "Valor de Saída: " << resposta << endl;
    fim = clock();
    duracao = 1000 * (double) (fim - inicio) / (double) (CLOCKS_PER_SEC);
    espera = 1000 * (double) (fim - tempoEscalonador) / (double) (CLOCKS_PER_SEC);
    printf("Tempo de resposta: %.2fms\n", duracao);
    printf("Tempo de espera: %.2fms\n\n", espera);
}

void firstComeFirstServed(thread threads[]){
    clock_t fim;
    double duracao, vazao;
    for(int i = 0; i < QTD_THREADS; i++){
        pronto = true;
        atual++;
        var_cond.notify_all();
        threads[i].join();
        pronto = false;
    }
    fim = clock();
    duracao = 1000 * (double) (fim - tempoEscalonador) / (double) (CLOCKS_PER_SEC);
    vazao = QTD_THREADS / duracao;
    printf("Vazão: %.2f threads/ms\n", vazao);
}

void loteria(thread threads[]){
    clock_t fim;
    double duracao, vazao;
    int sorteados[QTD_THREADS];
    for(int i = 0; i < QTD_THREADS; i++)
        sorteados[i] = -1;
    int bilhete = -1;
    for(int i = 0; i < QTD_THREADS; i++){
        while(true){
            bilhete = rand() % QTD_THREADS;
            int *p = find(sorteados, sorteados + QTD_THREADS, bilhete);
            if(p == sorteados + QTD_THREADS)
                break;
        }
        sorteados[i] = bilhete;
        cout << "Bilhete sorteado: " << bilhete << endl;
        pronto = true;
        atual = bilhete;
        var_cond.notify_all();
        threads[bilhete].join();
        pronto = false;
    }
    fim = clock();
    duracao = 1000 * (double) (fim - tempoEscalonador) / (double) (CLOCKS_PER_SEC);
    vazao = QTD_THREADS / duracao;
    printf("Vazão: %.2f threads/ms\n", vazao);
}
int main(){
    srand(time(NULL));
    cout << "Algoritmo Loteria\n";
    tempoEscalonador = clock();
    thread server = thread(servidor, -1);
    thread clientes[QTD_THREADS];
    for(int i = 0; i < QTD_THREADS; i++)
        clientes[i] = thread(cliente, i);
    firstComeFirstServed(clientes);
    loteria(clientes);
    return 0;
}
