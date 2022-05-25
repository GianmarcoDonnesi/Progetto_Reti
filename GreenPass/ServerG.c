#include "Header/ServerG.h"


void invioRichiestaS(int sfd, int sfd1) {

    char CodiceTS[20];
    char ris;

        read(sfd,&CodiceTS,sizeof(CodiceTS));
        write(sfd1,&CodiceTS, sizeof(CodiceTS));

        if (read(sfd1, &ris, 1)) {
            fputs("Errore di connessione con il ServerV.\n", stderr);
            exit(1);
        }
        if (ris == '1') {
            write(sfd, 1, sizeof(short));
        }else {
                if (ris == '2')
                    write(sfd, 2, sizeof(short));
            }
    exit(0);
        }



void invioRichiestaT(int sfd, int sfd1) {

    struct P_RICHIESTAT richiestaT;
    char ris;

        read(sfd, &richiestaT, sizeof(struct P_RICHIESTAT));
        write(sfd1, &richiestaT, sizeof(struct P_RICHIESTAT));

        if (read(sfd1, &ris, 1)) {
            fputs("Errore di connessione con il ServerV.\n", stderr);
            exit(1);
        }
        if (ris == '1') {
            write(sfd, 1, sizeof(short));
        }else {
            if (ris == '2')
                write(sfd, 2, sizeof(short));
        }

    exit(0);
}


/*void invioRisposta(int sfd, int ris) {
     invio risposta a un ClientS o ad un ClientT
    *write(sfd, &ris, sizeof(ris));

    *exit(0);
* }
*/


int main(int argc, char **argv) {
    int portV = 1025, portG = 1026;


    /* VARIABILI SERVER verso ClientS o ClientT */
    int listenfd, connfd, i;
    char identificativo_client[4];
    /* 	listenfd: descrittore di ascolto
	 *	connfd: descrittore di connessione
	 *	sockfd: descrittore di servizio
	 */
    struct sockaddr_in s_servaddr, s_cliaddr;
    int len;
    pid_t pid;

    /* 	servaddr: informazioni indirizzo server
	 *	cliaddr: informazioni indirizzo client connesso
	 */

    /* VARIABILI CLIENT verso ServerV */
    struct sockaddr_in c_servaddr;
    int masterfd;

    /* CLIENT verso serverV */
    /* creazione socket */
    masterfd = socket(AF_INET, SOCK_STREAM, 0);
    c_servaddr.sin_family = AF_INET;
    c_servaddr.sin_port = htons(portV);

    /* lettura indirizzo IP */
    if (inet_pton(AF_INET, argv[1], &c_servaddr.sin_addr) < 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    /* connessione al serverV */
    if (connect(masterfd, (struct sockaddr *) &c_servaddr, sizeof(c_servaddr)) < 0) {
        fprintf(stderr, "Connessione fallita\n");
        exit(1);
    }
    fputs("Connessione al serverV\n", stdout);
    if(strcmp(identificativo_client,"S") == 0){
        write(masterfd, "S", 4);
        invioRichiestaS(listenfd,masterfd);
    }else if (strcmp(identificativo_client,"T-1") == 0){
        write(masterfd, "T-1", 4);
        invioRichiestaT(listenfd,masterfd);
    }else if(strcmp(identificativo_client,"T-2") == 0){
        write(masterfd, "T-2", 4);
        invioRichiestaT(listenfd,masterfd);
    }

    /* SERVER verso clientS o clientT */
    /* creazione socket */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(1);
    }

    /* indirizzo del server */
    s_servaddr.sin_family = AF_INET;
    s_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_servaddr.sin_port = htons(portG);

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
            read(connfd,&identificativo_client, sizeof(identificativo_client));
            close(connfd);
            exit(0);
        }
        else{
            close(connfd);
        }
    }

    exit(0);
}
