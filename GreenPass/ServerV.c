#include "Header/Server.h"
#include "Header/p_richiestaT.h"
#include "libreria/db_model.h"

void nuovoUtenteVaccinato(struct UTENTE Utente){
    FILE *fp = fopen("db.dat", "ab");
    fwrite(&Utente, sizeof(struct UTENTE), 1, fp);
    fclose(fp);
}

void ricercaValidita(int connfd, char numerotessera[20]){
    FILE *fp = fopen("db.dat", "rb");
    struct UTENTE Utente;

    while (fread(&Utente, sizeof(struct UTENTE), 1, fp) > 0)
    {
        if (strcmp(Utente.numerotessera,numerotessera) == 0)
        {
            if(strcmp(Utente.validita,"1") == 0){
                write(connfd, "1", 2);
            }else if(strcmp(Utente.validita,"0") == 0){
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
            if(strcmp(Utente.validita,"0") == 0 && strcmp(richiestaT.controllo_contagio,"G") == 0){
                Utente.validita[1] = '1';
                fseek(fp, -sizeof(struct UTENTE), SEEK_CUR);
                fwrite(&Utente, sizeof(struct UTENTE), 1, fp);
                break;

            } else if (strcmp(Utente.validita,"1") == 0 && strcmp(richiestaT.controllo_contagio,"C") == 0){
                Utente.validita[1] = '0';
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
    exit(0);
}
void rispostaS(int sfd){

    char numerotesserasanitaria[20];

    read(sfd,&numerotesserasanitaria, sizeof (numerotesserasanitaria));
    ricercaValidita(sfd,numerotesserasanitaria);
    exit(0);
}

void rispostaCV(int sfd){

    struct UTENTE risposta;

    read(sfd,&risposta,sizeof (struct UTENTE));
    write(sfd,"1",2);

        nuovoUtenteVaccinato(risposta);
    exit(0);

}


int main(int argc,char **argv){

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
            }else if(strcmp(identificativo,"CV")==0){
                rispostaCV(connfd);
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
