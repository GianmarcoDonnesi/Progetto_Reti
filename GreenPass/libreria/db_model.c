#include "db_model.h"

Utente *nuovoUtente(char codF[MAX], char Psw[MAX]) {
    Utente *pnt = (Utente *) malloc (sizeof(Utente));
    strcpy(pnt -> codicefiscale, codF);
    strcpy(pnt -> password, Psw);
    pnt -> greenpass = NULL;
    pnt -> next = NULL;
    return pnt;
}

Utente *inserisciUtente(Utente *top, char codF[], char psw[]) {
    if (top == NULL || strcmp(top -> codicefiscale, codF) > 0) {
        Utente *tmp = nuovoUtente(codF, psw);
        tmp -> next = top;
        top = tmp;
        return top;
    }
    else
        top -> next = inserisciUtente(top -> next, codF, psw);
}

Utente *ricercaUtente (Utente *top, char C_fiscale[]){
    if (top == NULL || !strcmp(top->codicefiscale, C_fiscale))
        return top;
    else
        return ricercaUtente(top -> next, C_fiscale);
}

GreenPass *nuovoGreenPass(char Cod[MAX], char D_scadanza[MAX], char val[MAX]) {
    GreenPass *pnt = (GreenPass *) malloc (sizeof(GreenPass));
    strcpy(pnt -> codice, Cod);
    strcpy(pnt -> data_scadenza, D_scadanza);
    strcpy(pnt -> validita, val);
    pnt -> next = NULL;
    return pnt;
}

GreenPass *inserisciGreenPass(GreenPass *top, char C[], char Ds[], char v[]) {
    if (top == NULL || strcmp(top -> codice, C) > 0) {
        GreenPass *tmp = nuovoGreenPass(C, Ds,v);
        tmp -> next = top;
        top = tmp;
        return top;
    }
    else
        top -> next = inserisciGreenPass(top -> next, C, Ds,v);
}

GreenPass *ricercaGreenPass (GreenPass *top, char C_gp[]){
    if (top == NULL || !strcmp(top->codice, C_gp))
        return top;
    else
        return ricercaGreenPass(top -> next, C_gp);
}

