#include "Header/Server.h"
#include "Header/p_richiestaT.h"
#include "libreria/db_model.h"
#include "Header/include_functions.h"
#include "time.h"


void ricercaValidita(int connfd, char numerotessera[20]){
    FILE *fp = fopen("db.dat", "rb");
    struct UTENTE Utente;

    while (fread(&Utente, sizeof(struct UTENTE), 1, fp) > 0)
    {
        if (strcmp(Utente.numerotessera,numerotessera) == 0)
        {
            if(Utente.validita == 1){
                write(connfd, "1", 2);
            }else if(Utente.validita == 0){
                write(connfd, "2", 2);
            }

        }
    }
    fclose(fp);
}

void modificavalidita(int connfd, struct P_RICHIESTAT richiestaT){
    FILE *fp = fopen("db.dat", "rb+");
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
        write(connfd,"1",2);
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
}
void rispostaS(int sfd){

    char numerotesserasanitaria[20];

    read(sfd,&numerotesserasanitaria, sizeof (numerotesserasanitaria));
    ricercaValidita(sfd,numerotesserasanitaria);
}

void rispostaCV(int sfd){

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    struct UTENTE inserimento_utente;
    char CodiceTs[20];

    read(sfd, &CodiceTs, sizeof (CodiceTs));
    strcpy(inserimento_utente.numerotessera,CodiceTs);
    inserimento_utente.validita = 1;
    inserimento_utente.giorno = tm.tm_mday;
    inserimento_utente.mese = tm.tm_mon +1;
    inserimento_utente.anno = tm.tm_year +1900;

    FILE *fp = fopen("db.dat", "ab");
    fwrite(&inserimento_utente, sizeof(struct UTENTE), 1, fp);
    fclose(fp);

    write(sfd,"1",2);

}


int main(int argc,char *argv[]){

    int portV = 8025;
    int listenfd, connfd, i;
    char identificativo[4];
    struct sockaddr_in s_servaddr, s_cliaddr;
    socklen_t len;
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
        connfd = accept(listenfd, (struct sockaddr*)&s_cliaddr, &len);
        if(connfd < 0){
            perror("accept");
            exit(1);
        }
        if((pid = fork())<0)
        {
            perror (" fork error ");
            exit ( -1);
        }
        if(pid==0) {
            close(listenfd);
            read(connfd, &identificativo, sizeof(identificativo));
            if((strcmp(identificativo,"T-1") == 0) || (strcmp(identificativo,"T-2") == 0)){
            rispostaT(connfd,identificativo);
           }else if(strcmp(identificativo,"S")==0){
               rispostaS(connfd);
            }else if (strcmp(identificativo,"C") == 0) {
                rispostaCV(connfd);
            }
            close(connfd);
            exit(0);
        }else{
            close(connfd);
        }
    }

    exit(0);










}
