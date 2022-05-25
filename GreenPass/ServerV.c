#include "Header/ServerG.h"
#include "Header/p_richiestaT.h"
#include "/libreria/db_model.h"

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
                Write(connfd, 2, sizeof (short));
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
        write(connfd,1,sizeof (short));
    }
    fclose(fp);
}

void rispostaT(int sfd,char identificativo[4]){

    struct P_RICHIESTAT rispostaT;

    read(sfd,&rispostaT, sizeof (struct P_RICHIESTAT));


    if(strcmp(identificativo,"T-1") == 0){
        ricercaValidita(sfd,rispostaT.numero_tessera);
    }else if(strcmp(identificativo,"T-2") == 0){
        modificavalidita(sfd,rispostaT);
    }
    exit(0);
}
void rispostaS(int sfd){

    char numerotesserasanitaria[20];

    read(sfd,&numerotesserasanitaria, sizeof (numerotesserasanitaria));
    ricercaValidita(sfd,numerotesserasanitaria);
    exit(0);
}



int main(char argc,char **argv){
    int portV = 1025;
    int listenfd, connfd, i;
    char identificativo[4];
    struct sockaddr_in s_servaddr, s_cliaddr;
    int len;
    pid_t pid;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }
    /* indirizzo del server */
    s_servaddr.sin_family = AF_INET;
    s_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_servaddr.sin_port = htons(portV);

    /* assegnazione indirizzo */
    if (bind(listenfd, (struct sockaddr *) &s_servaddr, sizeof(s_servaddr)) < 0) {
        perror("bind");
        exit(1);
    }

    /* messa in ascolto */
    if (listen(listenfd, 1024) < 0) {
        perror("listen");
        exit(1);
    }
    while(1){
        len = sizeof(s_cliaddr);
        if ( ( connfd = accept(listenfd, (struct sockaddr *) NULL, NULL) ) < 0 ) {
            perror("accept");
            exit(1);
        }
        if((pid = fork())<0)
        {
            perror (" fork error ");
            exit ( -1);
        }
        if(pid==0){
            close(listenfd);
            read(connfd, &identificativo, sizeof (identificativo));
            if((strcmp(identificativo,"T-1") == 0) || (strcmp(identificativo,"T-2") == 0)){
            rispostaT(connfd,identificativo);
}else if(strcmp(identificativo,"S")==0){
                rispostaS(connfd);
            }
            close(connfd);
            exit(0);
        }
        else{
            close(connfd);
        }
    }

    exit(0);










}
