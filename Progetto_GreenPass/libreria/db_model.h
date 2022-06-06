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
    char numerotessera [21];
    int giorno;
    int mese;
    int anno;
    int validita;
};



#endif //GREENPASS_DB_MODEL_H
