//
// Created by Gianmarco on 20/05/2022.
//

#ifndef GREENPASS_DB_MODEL_H
#define GREENPASS_DB_MODEL_H
#include "../Header/p_richiestaT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct UTENTE {
    char numerotessera [20];
    char codice [20];
    char data_scadenza[10];
    int validita;
};



void nuovoUtenteVaccinato();
void ricercaValidita(int connfd, char numerotessera[20]);
void modificavalidita(int connfd, struct P_RICHIESTAT richiestaT);

#endif //GREENPASS_DB_MODEL_H
