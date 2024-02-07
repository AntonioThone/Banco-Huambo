#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct { 
    int conta_facul_engenharia;
    int bloqueada;
    pthread_mutex_t mutex;
} 

BancoHuambo;

BancoHuambo banco_huambo = {25000, 0, PTHREAD_MUTEX_INITIALIZER};

void depositar_propina(int valor) {
    pthread_mutex_lock(&banco_huambo.mutex);

    if (!banco_huambo.bloqueada) {
        printf("Deposito de %d USD recebido.\n", valor);
        banco_huambo.conta_facul_engenharia += valor;
        printf("Saldo atual: %d USD\n", banco_huambo.conta_facul_engenharia);
    } else {
        printf("Conta bloqueada. Deposito nao permitido.\n");
    }

    pthread_mutex_unlock(&banco_huambo.mutex);
}

void realizar_pagamento(int valor, const char *descricao) {
    pthread_mutex_lock(&banco_huambo.mutex);

    if (!banco_huambo.bloqueada) {
        if (banco_huambo.conta_facul_engenharia >= valor) {
            printf("Pagamento de %d USD para %s realizado.\n", valor, descricao);
            banco_huambo.conta_facul_engenharia -= valor;
            printf("Saldo atual: %d USD\n", banco_huambo.conta_facul_engenharia);
        } else {
            printf("Saldo insuficiente. Pagamento não realizado.\n");
        }
    } else {
        printf("Conta bloqueada. Pagamento nao permitido.\n");
    }

    pthread_mutex_unlock(&banco_huambo.mutex);
}

void bloquear_conta() {
    pthread_mutex_lock(&banco_huambo.mutex);
    printf("Conta bloqueada.\n");
    banco_huambo.bloqueada = 1;
    pthread_mutex_unlock(&banco_huambo.mutex);
}

void desbloquear_conta() {
    pthread_mutex_lock(&banco_huambo.mutex);
    printf("Conta desbloqueada.\n");
    banco_huambo.bloqueada = 0;
    pthread_mutex_unlock(&banco_huambo.mutex);
}

void *depositar_propina_em_loop(void *arg) {
    int i;
    for (i = 0; i < *((int *)arg); i++) {
        int valor = rand() % 2001 + 1000; // Valores aleatórios entre 1000 e 3000
        depositar_propina(valor);
        usleep(500000); // Aguardar meio segundo
    }

    return NULL;
}

void *realizar_pagamentos_em_loop(void *arg) {
    while (1) {
        int valor = rand() % 10001 + 5000; // Valores aleatórios entre 5000 e 15000
        realizar_pagamento(valor, "Pagamento de salários e despesas");
        sleep(1);
    }

    return NULL;
}

int main() {
    pthread_t thread_depositos, thread_pagamentos;
    int estudantes = 20;

    // Inicializar as threads para simular os depósitos diários de propina e os pagamentos diários
    pthread_create(&thread_depositos, NULL, depositar_propina_em_loop, &estudantes);
    pthread_create(&thread_pagamentos, NULL, realizar_pagamentos_em_loop, NULL);

    // Aguarde até que a interrupção ocorra (simulação do pagamento de 300.000 USD de um professor)
    //sleep(5);
    bloquear_conta();

    // Aguarde até que o pagamento da parcela de juros seja feito (simulação do final do mês)
    //sleep(4);
    desbloquear_conta();

    // Aguarde até que as threads sejam encerradas
    pthread_join(thread_depositos, NULL);
    pthread_join(thread_pagamentos, NULL);
	
	estudantes ++;
	
    return 0;
}

