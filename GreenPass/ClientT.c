#include "Header/Client.h"

void Logo () {
    fputs("*****************************\n", stdout);
    fputs("******** CLIENT T ***********\n", stdout);
    fputs("*****************************\n", stdout);
    fputs("+++ Modifica validità GreenPass +++\n", stdout);
    fputs("____________________________________\n", stdout);
}


void modificavalidita(int sockfd){
    p_richiestaT richiestaT;
    char buf[BUFFSIZE], ris;
    char CoG;
do {
    fputs("Inserire il codice della tessera sanitaria da controllare: ", stdout);
    fgets(buf, sizeof(buf), stdin);
    fputs("Inserire il codice per invalidare o ripristinare il GreenPass: ", stdout);
    fputs("1) C = Contagio (invalida GreenPass) ", stdout);
    fputs("2) G = Guarigione (ripristina GreenPass) ", stdout);
    CoG = getchar();
}while (CoG != 'C' && CoG != 'G' && CoG != 'c' && CoG != 'g');
clear();
getchar();
    if(buf[0] != '\n') {
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(richiestaT.numero_tessera, buf);
        richiestaT.controllo_contagio = CoG;
        Write(sockfd, &richiestaT, sizeof(richiestaT));
        Read(sockfd, &ris, 1);
        if(ris == '0'){
            fputs("Operazione completata con successo.\n", stdout);
        }else{
            fputs("Operazione fallita.\n", stderr);
        }


    }
    }

    int main(int argc, char **argv) {
        int                sockfd, portT = 8002;
        char               buf[BUFFSIZE] ;
        struct sockaddr_in servaddr;

        if (argc != 2) {
            fprintf(stderr,"Specificare indirizzo IP del server: %s <IPaddress>\n",argv[0]);
            exit (1);
        }

        /* creazione socket */
        if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            fprintf(stderr,"socket error\n");
            exit (1);
        }
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(portT);

        if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
            fprintf(stderr,"inet_pton error for %s\n", argv[1]);
            exit (1);
        }

        if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            fprintf(stderr,"Connessione fallita\n");
            exit(1);
        }
        Write(sockfd, "t", 1);           /* identificazione al server */
        Read(sockfd, buf, 1);	              /* lettura esito connessione */

        if (buf[0] == '0')
            fputs("Connesso con serverN.\n", stdout);
        else {
            fputs("Connessione fallita: server non valido.\n", stderr);
            exit(1);
        }
        Logo();
        modificavalidita(sockfd);
        exit(0);
        }