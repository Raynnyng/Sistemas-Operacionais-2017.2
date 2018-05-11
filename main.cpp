#include <bits/stdc++.h>	// std::cout
#include <thread>		// std::thread
#include <mutex>        // std::mutex, std::unique_lock
#include <condition_variable>
using namespace std;
mutex mtx;
condition_variable cond_variable;
volatile bool flag = false;
volatile int actual = 0;
volatile int count_thread = 0;

//Variáveis Compartilhadas
volatile int solicitcao = 0;
volatile int resposta = 0;

//Controle de Tempo
volatile clock_t tempoi_escalonador;
int main(int argc, char** argv) {
	cout << "Hello World";
	return 0;
}
