#include "Header/Server.h"

//sfd descrittore come server
//sfd1 descrittore come client
void invioRichiestaS(int sfd, int sfd1) {

    char CodiceTS[20];
    char ris;

        read(sfd,&CodiceTS,sizeof(CodiceTS));
        write(sfd1,&CodiceTS, sizeof(CodiceTS));

    if (read(sfd1, &ris, sizeof (ris)) < 0) {
            fputs("Errore di connessione con il ServerV.\n", stderr);
            exit(1);
        }
        if (strcmp(&ris,"1") == 0) {
            write(sfd, "1", 2);
        }else if (strcmp(&ris,"2") == 0){
                    write(sfd, "2", 2);
            }
    exit(0);
        }



void invioRichiestaT(int sfd, int sfd1) {

    struct P_RICHIESTAT richiestaT;
    char ris;

        read(sfd, &richiestaT, sizeof(struct P_RICHIESTAT));
        write(sfd1, &richiestaT, sizeof(struct P_RICHIESTAT));

    if (read(sfd1, &ris, sizeof (ris)) < 0) {
            fputs("Errore di connessione con il ServerV.\n", stderr);
            exit(1);
        }
    if (strcmp(&ris,"1") == 0) {
        write(sfd, "1", 2);
    }else  if (strcmp(&ris,"2") == 0){
        write(sfd, "2", 2);
    }

    exit(0);
}



int main(int argc, char **argv) {
    int portV = 8025, portG = 8026;


    /* VARIABILI SERVER verso ClientS o ClientT */
    int listenfd, connfd, i;
    char identificativo_client[4];
    /* 	listenfd: descrittore di ascolto
	 *	connfd: descrittore di connessione
	 *	sockfd: descrittore di servizio
	 */
    struct sockaddr_in s_servaddr,client;
    socklen_t len;
    pid_t pid;

    /* 	servaddr: informazioni indirizzo server
	 *	cliaddr: informazioni indirizzo client connesso
	 */

    /* VARIABILI CLIENT verso ServerV */
    struct sockaddr_in c_servaddr;
    int Vfd;


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
        len = sizeof(client);
        if ( ( connfd = accept(listenfd, (struct sockaddr *)&client, &len) ) < 0 ) {
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

            /* CLIENT verso serverV */
            /* creazione socket */
            if ( (Vfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
                fprintf(stderr,"socket error\n");
                exit (1);
            }
            c_servaddr.sin_family = AF_INET;
            c_servaddr.sin_port = htons(portV);

            /* lettura indirizzo IP */
            if (inet_pton(AF_INET, argv[1], &c_servaddr.sin_addr) < 0) {
                fprintf(stderr, "inet_pton error for %s\n", argv[1]);
                exit(1);
            }

            /* connessione al serverV */
            if (connect(Vfd, (struct sockaddr *) &c_servaddr, sizeof(c_servaddr)) < 0) {
                fprintf(stderr, "Connessione fallita\n");
                exit(1);
            }

            fputs("Connesso al ServerV\n", stdout);
            if(strcmp(identificativo_client,"S") == 0){
                write(Vfd, "S", 2);
                invioRichiestaS(connfd,Vfd);
            }else if (strcmp(identificativo_client,"T-1") == 0){
                write(Vfd, "T-1", 4);
                invioRichiestaT(connfd,Vfd);
            }else if(strcmp(identificativo_client,"T-2") == 0){
                write(Vfd, "T-2", 4);
                invioRichiestaT(connfd,Vfd);
            }
            close(Vfd);
        }
        else{
            close(connfd);
            close(Vfd);
        }
    }

    exit(0);
}
