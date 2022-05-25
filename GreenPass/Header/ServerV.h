//
// Created by Gianmarco on 22/05/2022.
//

#ifndef GREENPASS_SERVERV_H
#define GREENPASS_SERVERV_H

#include "../libreria/db_model.h"
#include "include_functions.h"
#include "p_richiestaT.h"
#include <pthread.h>

typedef struct{
    int connfd;
    FILE *dbf;
    Utente *listaUtenti;
}pthread_args;

pthread_mutex_t mutex;

void *serverG(void *connfd);
void *CentroVaccinale(void *connfd);
void printUsage(char *prog);


#endif //GREENPASS_SERVERV_H
