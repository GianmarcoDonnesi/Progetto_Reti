
#include "Header/Server.h"
#include "Header/include_functions.h"


//sfd descrittore come server
//sfd1 descrittore come client

void invioRichiestaClient(int sfd, int sfd1) {

    char CodiceTS[20];
    char identificativo[2];
    char ris;

    read(sfd,&identificativo, sizeof (identificativo));
    write(sfd1,&identificativo, sizeof (identificativo));
    read(sfd,&CodiceTS,sizeof(CodiceTS));
    write(sfd1,&CodiceTS,sizeof (CodiceTS));


    if (read(sfd1, &ris, sizeof (ris)) < 0) {
        fputs("Errore di connessione con il ServerV.\n", stderr);
        exit(1);
    }
    if (strcmp(&ris,"1") == 0) {
        write(sfd, "1", 2);
    }else if (strcmp(&ris,"2") == 0){
        write(sfd, "2", 2);
    }
   // exit(0);
}



int main(int argc, char *argv[]) {
    int portV = 8025, portC = 8027;


    /* VARIABILI SERVER verso ClientS o ClientT */
    int listenfd, connfd;
    char identificativo[2];
    /* 	listenfd: descrittore di ascolto
	 *	connfd: descrittore di connessione
	 *	sockfd: descrittore di servizio
	 */
    struct sockaddr_in s_servaddr, s_client;
    socklen_t len;
    pid_t pid;

    /* 	s_servaddr: informazioni indirizzo server
	 *	s_client: informazioni indirizzo client
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
    s_servaddr.sin_port = htons(portC);

    /* assegnazione indirizzo */
    if (bind(listenfd, (struct sockaddr *) &s_servaddr, sizeof(s_servaddr)) < 0) {
        perror("bind");
        exit(1);
    }
    /* messa in ascolto */
    if (listen(listenfd, 8027) < 0) {
        perror("listen");
        exit(1);
    }

    while(1){
        len = sizeof(s_client);
        connfd = accept(listenfd, (struct sockaddr*) &s_client, &len);
        if(connfd < 0){
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
            invioRichiestaClient(connfd,Vfd);
            close(Vfd);
        }
        else{
            close(connfd);
            close(Vfd);
        }
    }

    exit(0);
}
