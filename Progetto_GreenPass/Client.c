#include "Header/Client.h"

void Logo () {
    printf("*****************************\n");
    printf("******** CLIENT ***********\n");
    printf("*****************************\n");
    printf("+++ GreenPass +++\n");
    printf("____________________________________\n");
}

void StartMenu(int sockfd) {
    int sm;

    Logo();
    do {
        printf("Benvenuto.\n");
        printf("Selezionare operazione:\n");
        printf("1) Invia codice tessera sanitaria\n");
        printf("2) Esci\n");
        fflush(stdin);
        sm = getchar();
    }while(sm < '1' || sm > '2');
    clear();
    getchar();
    switch (sm) {
        case '1':
            invioTesseraSanitaria(sockfd);
            break;
        case '2':
            fputs("Client chiuso\n", stdout);
            exit(0);
        default:
            break;
    }

}

void invioTesseraSanitaria(int sfd) {

    char CodiceTS[20];
    char ris;

        printf("---Comunicazione con centro vaccinale---\n");
        printf("Inserire Codice Tessera Sanitaria: \n");
        fflush(stdin);
        fgets(CodiceTS, sizeof(CodiceTS), stdin);

        write(sfd, "C",2);
        write(sfd, &CodiceTS, sizeof(CodiceTS));	    /* invio richiesta al centro vaccinale */
                                                                                        /* lettura esito */
        read(sfd, &ris, sizeof (ris));

        if (strcmp(&ris,"1") == 0)
            fprintf(stdout, "Operazione effettuata correttamente!\n");
        else {
            if (strcmp(&ris, "2") == 0)
                fputs("Invio non riuscito.\n", stderr);

        }
    //exit(0);
}


int main(int argc, char *argv[]) {
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
}