#include "Header/ServerG.h"

void elaboraRichiesta(int masterfd, int sockfd, p_queryG *rich){
    char buf[BUFFSIZE];
    GreenPass g_buf;
    switch (rich -> richiesta){
        case '1':
            Write(masterfd, rich, sizeof(*rich));
            do{
                Read(masterfd, &g_buf, sizeof(g_buf));
                //write clientS
                Write(sockfd, &g_buf, sizeof(g_buf));
            }while (g_buf.codice[0] != '\0');
            break;

        case '2':
            Write(masterfd, rich, sizeof(*rich));
                Read(masterfd, buf, sizeof(buf));
                //write clientT
                Write(sockfd, buf, sizeof(buf));
                break;
    }

}
void stampautilizzo(char *prog) {
    fprintf(stderr, "uso: %s -i <indirizzo> [opzione...]\n", prog);
    fputs("\t-i <indirizzo>\t\tIndirizzo serverV\n", stderr);
    fputs("\t-m <porta>\t\tNumero di porta serverV [default: 8000]\n", stderr);
    fputs("\t-p <porta>\t\tNumero di porta serverG[default: 8002]\n", stderr);
    exit(1);
}

int main(int argc, char **argv) {
    int portV = 8006, portG = 8007;
    char address[200];
    address[0] = '\0';

    /* VARIABILI SERVER verso clientS */
    int listenfd, connfd, sockfd;
    struct sockaddr_in s_servaddr, s_cliaddr;
    int len = sizeof(s_cliaddr);
    int option = 1, i, maxi = -1, maxd, ready, client[FD_SETSIZE];
    fd_set active_fd_set, read_fd_set;
    char iplog[FD_SETSIZE][16];
    int portlog[FD_SETSIZE];

    /* VARIABILI CLIENT verso serverV */
    struct sockaddr_in c_servaddr;
    int masterfd;
    char buf[BUFFSIZE];
    p_queryG richiesta;

    /* CLIENT verso serverV */
    /* creazione socket */

    masterfd = Socket(AF_INET, SOCK_STREAM, 0);
    c_servaddr.sin_family = AF_INET;
    c_servaddr.sin_port = htons(portV);

    /* lettura indirizzo IP */
    if (inet_pton(AF_INET, argv[1], &c_servaddr.sin_addr) < 0) {
        fprintf(stderr, "inet_pton error for %s\n", argv[1]);
        exit(1);
    }
    /* connessione al server */
    if (connect(masterfd, (struct sockaddr *) &c_servaddr, sizeof(c_servaddr)) < 0) {
        fprintf(stderr, "Connessione fallita\n");
        exit(1);
    }
    fputs("Connessione al serverV...\n", stdout);
    Write(masterfd, "G", 1);    /* identificazione al server */
    Read(masterfd, buf, 1);    /* leggi esito connessione */
    if (buf[0] == '0')
        printf("Connesso al serverV\n");
    else {
        fputs("Connessione fallita.\n", stderr);
        exit(1);
    }
    /* SERVER verso client */
    /* creazione socket */
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    /* impostazioni socket */
    SetSockOpt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    /* indirizzo del server */
    s_servaddr.sin_family = AF_INET;
    s_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_servaddr.sin_port = htons(portG);

    fprintf(stdout, "Indirizzo IP: %s\n", inet_ntoa(s_servaddr.sin_addr));
    fprintf(stdout, "Porta: %d\n", (int) ntohs(s_servaddr.sin_port));

    /* assegnazione indirizzo */
    Bind(listenfd, &s_servaddr);
    /* messa in ascolto */
    Listen(listenfd, 1024);
    maxd = listenfd;

    for (i = 0; i < FD_SETSIZE; i++)
        client[i] = -1; /* inizializzazione vettore di client */
    FD_ZERO(&active_fd_set); /* inizializzazione a zero l'insieme dei descrittori */
    FD_SET(listenfd, &active_fd_set); /* aggiunta del descrittore di ascolto */

    /* esecuzione server */
    while (1) {
        read_fd_set = active_fd_set; /* imposta il set di descrittori per la lettura */
        ready = Select(maxd + 1, &read_fd_set, NULL, NULL, NULL);
        /* Se è arrivata una richiesta di connessione, il socket di ascolto
		è leggibile: viene invocata accept() e creato un socket di connessione */
        if (FD_ISSET(listenfd, &read_fd_set)) {
            /* accettazione richiesta di connessione */
            connfd = Accept(listenfd, &s_cliaddr, &len);
            Read(connfd, buf, 1);
            if (buf[0] == 's' || buf[0] == 't') {
                /* identificazione client */
                Write(connfd, "0", 1);
                for (i = 0; i < FD_SETSIZE; i++)
                    if (client[i] < 0) {
                        client[i] = connfd;
                        break;
                    }
                /* errore se non ci sono posti liberi nel vettore client */
                if (i == FD_SETSIZE) {
                    fputs("accept: client array size full\n", stderr);
                    exit(1);
                }
                inet_ntop(AF_INET, &s_cliaddr.sin_addr, iplog[i], sizeof(iplog[i]));
                portlog[i] = ntohs(s_cliaddr.sin_port);
                printf(stdout, "(%s:%d) Client connesso.\n", iplog[i], portlog[i]);
                FD_SET(connfd, &active_fd_set);
                if (connfd > maxd)
                    maxd = connfd;
                if (i > maxi)
                    maxi = i;
            }else {
                Write(connfd, "1", 1);
                close(connfd);
                fputs("Connessione client rifiutata.\n", stderr);
            }
            --ready; /* richiesta accettata/rifiutata, un socket in meno */


        }
        /* controlla i socket attivi per controllare se sono leggibili */
        for (i = 0; i <= maxi && ready > 0; i++) {
            if ((sockfd = client[i]) >= 0) { /* se il descrittore non è stato selezionato, viene saltato */
                if (FD_ISSET(sockfd, &read_fd_set)) {
                    /* leggi da sockfd */
                    if (!Read(sockfd, &richiesta, sizeof(richiesta)))
                        elaboraRichiesta(masterfd, sockfd, &richiesta);
                    else {
                        /* chiusura connessione da parte del client */
                        fprintf(stdout, "(%s:%d) Connessione chiusa da client.\n", iplog[i], portlog[i]);
                        FD_CLR(sockfd, &active_fd_set); /* rimuove sockfd dalla lista dei socket da controllare */
                        client[i] = -1; /* cancella sockfd da client */
                    }
                    --ready; /* richiesta elaborata o connessione chiusa, un socket in meno */
                }
            }
        }
    }
    exit(0);
}
