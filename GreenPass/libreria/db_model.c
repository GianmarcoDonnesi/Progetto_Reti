#include "db_model.h"
#include "../Header/include_functions.h"
#include "../Header/p_richiestaT.h"

void nuovoUtenteVaccinato(){
    FILE *fp = fopen("db.dat", "ab");
    struct UTENTE Utente;

    printf("Inserire il numero della tessera sanitaria: \n");
    fgets(Utente.numerotessera, 20, stdin);
    strcpy(Utente.numerotessera + strlen(Utente.numerotessera) - 1, "\0");
    fflush(stdin);

    printf("Inserire codice del GreenPass: \n");
    fgets(Utente.codice, 20, stdin);
    strcpy(Utente.codice + strlen(Utente.codice) - 1, "\0");
    fflush(stdin);

    printf("Inserire data validità GreenPass (GG-MM-AAAA): \n");
    fgets(Utente.data_scadenza, 20, stdin);
    strcpy(Utente.data_scadenza + strlen(Utente.data_scadenza) - 1, "\0");
    fflush(stdin);

    printf("Inserire validità: \n");
    scanf("%d", &Utente.validita);
    fflush(stdin);

    fwrite(&Utente, sizeof(struct UTENTE), 1, fp);
    fclose(fp);
}

void ricercaValidita(int connfd, char numerotessera[20]){
    FILE *fp = fopen("db.dat", "ab");
    struct UTENTE Utente;

        while (fread(&Utente, sizeof(struct UTENTE), 1, fp) > 0)
        {
            if (strcmp(Utente.numerotessera,numerotessera) == 0)
            {
                if(Utente.validita == 1){
                    Write(connfd, 1, sizeof(short));
                }else if(Utente.validita == 0){
                    Write(connfd, 0, sizeof (short));
                }

            }
    }
    fclose(fp);
}

void modificavalidita(int connfd, struct P_RICHIESTAT richiestaT){
    FILE *fp = fopen("db.dat", "ab");
    struct UTENTE Utente;

        while (fread(&Utente, sizeof(struct UTENTE), 1, fp) > 0)
        {
            if (strcmp(Utente.numerotessera,richiestaT.numero_tessera) == 0)
            {
                if(Utente.validita == 0 && strcmp(richiestaT.controllo_contagio,"G") == 0){
                    Utente.validita = 1;
                fseek(fp, -sizeof(struct UTENTE), SEEK_CUR);
                fwrite(&Utente, sizeof(struct UTENTE), 1, fp);
                break;

            } else if (Utente.validita == 1 && strcmp(richiestaT.controllo_contagio,"C") == 0){
                    Utente.validita = 0;
                    fseek(fp, -sizeof(struct UTENTE), SEEK_CUR);
                    fwrite(&Utente, sizeof(struct UTENTE), 1, fp);
                    break;
                }
        }

    }
    fclose(fp);
}

