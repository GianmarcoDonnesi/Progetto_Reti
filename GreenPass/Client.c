#include "Header/Client.h"

void Logo () {
    fputs("*****************************\n", stdout);
    fputs("******** CLIENT ***********\n", stdout);
    fputs("*****************************\n", stdout);
    fputs("+++ GreenPass +++\n", stdout);
    fputs("____________________________________\n", stdout);
}

void StartMenu(int sockfd) {
    char sm;
        do {
            Logo();
            fputs("Benvenuto.\n", stdout);
            fputs("Selezionare operazione:\n", stdout);
            fputs("1) Invia codice tessera sanitaria\n", stdout);
            fputs("2) Esci\n", stdout);
            sm = getchar();
        } while (sm < '1' || sm > '2');
        clear();
        getchar();
        switch (sm) {
            case '1':
                invioTesseraSanitaria(sockfd);
                break;
            case '2':
                fputs("Client chiuso\n",stdout);
                exit(0);
            default:
                exit(0);
        }

}

void invioTesseraSanitaria(int sfd) {

    char CodiceTS[20];
    char ris;

        fputs("---Comunicazione con centro vaccinale---\n", stdout);
        fputs("Inserire Codice Tessera Sanitaria: ", stdout);
        fgets(CodiceTS, sizeof(CodiceTS), stdin);
        fflush(stdin);
        write(sfd, "CV",3);
        write(sfd, &CodiceTS, sizeof(CodiceTS));	    /* invio richiesta al centro vaccinale */
                                                                                        /* lettura esito */

        if (read(sfd, &ris, sizeof (ris))) {
            fputs("Errore di connessione con il Server.\n", stderr);
            exit(1);
        }
        if (strcmp(&ris,"1") == 0)
            fprintf(stdout, "Codice inviato correttamente!\n");
        else {
            if (strcmp(&ris, "2") == 0)
                fputs("Invio non riuscito.\n", stderr);

        }
    exit(0);
}


int main(int argc, char **argv) {
    int                sockfd;
    char               tessera_sanitaria[20] ;
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
    servaddr.sin_port = htons(8027);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit (1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr,"Connessione fallita\n");
        exit(1);
    }
    StartMenu(sockfd);                                        /* comunicazione con CentroVaccinale */
    exit(0);
}