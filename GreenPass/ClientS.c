#include "Header/Client.h"

void Logo () {
    fputs("*****************************\n", stdout);
    fputs("******** CLIENT S ***********\n", stdout);
    fputs("*****************************\n", stdout);
    fputs("+++ Controllo validità GreenPass +++\n", stdout);
    fputs("____________________________________\n", stdout);
}

void verificavalidita(int sockfd){
    p_richiestaS richiestaS;
    char buf[BUFFSIZE], ris;

    fputs("Inserire il codice della tessera sanitaria da controllare: ", stdout);
    fgets(buf, sizeof(buf), stdin);
    if(buf[0] != '\n'){
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(richiestaS.numero_tessera, buf);
        fprintf(stdout, "Richiesta controllo validità per %s.\n", richiestaS.numero_tessera);
        Write(sockfd, &richiestaS, sizeof(richiestaS));
        Read(sockfd, &ris, 1);
        if(ris == '0'){
            //inserire validità tessera
            fputs("Operazione completata con successo.\n", stdout);
        }else{
            fputs("Operazione fallita.\n", stderr);
        }
    }
}
int main(int argc, char **argv) {
    int                sockfd, portS = 8001;
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
    servaddr.sin_port = htons(portS);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit (1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr,"Connessione fallita\n");
        exit(1);
    }
    Write(sockfd, "s", 1);           /* identificazione al server */
    Read(sockfd, buf, 1);	              /* lettura esito connessione */

    if (buf[0] == '0')
        fputs("Connesso con serverN.\n", stdout);
    else {
        fputs("Connessione fallita: server non valido.\n", stderr);
        exit(1);
    }
    Logo();
    verificavalidita(sockfd);
    exit(0);
}
