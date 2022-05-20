//
// Created by Gianmarco on 20/05/2022.
//

#ifndef GREENPASS_DB_MODEL_H
#define GREENPASS_DB_MODEL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50


typedef struct greenP {
    char codice [MAX];
    char data_scadenza[MAX];
    char validita[MAX];
    struct greenP *next;
}GreenPass;

typedef struct idUtente {
    char codicefiscale [MAX];
    char password [MAX];
    GreenPass *greenpass;
    struct idUtente *next;
}Utente;



Utente *nuovoUtente(char codF[MAX], char Psw[MAX]);
Utente *inserisciUtente(Utente *top, char codF[], char psw[]);
Utente *ricercaUtente (Utente *top, char C_fiscale[]);
GreenPass *nuovoGreenPass(char Cod[MAX], char D_scadanza[MAX], char val[MAX]);
GreenPass *inserisciGreenPass(GreenPass *top, char C[], char Ds[], char v[]);
GreenPass *ricercaGreenPass (GreenPass *top, char C_gp[]);

#endif //GREENPASS_DB_MODEL_H
