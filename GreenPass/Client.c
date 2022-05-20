#include "Header/Client.h"

void Logo () {
    fputs("*****************************\n", stdout);
    fputs("******** CLIENT ***********\n", stdout);
    fputs("*****************************\n", stdout);
    fputs("+++  GreenPass +++\n", stdout);
    fputs("____________________________________\n", stdout);
}
void StartMenu(int sockfd, char *C_fiscale) {
    char sc;
    C_fiscale[0] = '\0';
    do {
        clear();
        do {
            Logo();
            fputs("Benvenuto.\n", stdout);
            fputs("Selezionare operazione:\n", stdout);
            fputs("1) Accedi e controlla validità GreenPass\n", stdout);
            fputs("2) Registrati\n", stdout);
            fputs("0) Esci\n", stdout);
            sc = getchar();
        } while (sc < '0' || sc > '2');
        clear();
        getchar();
        switch (sc) {
            case '1':
                Login(C_fiscale, sockfd);
                break;
            case '2':
                Signup(sockfd);
                break;
            default:
                exit(0);
        }
    } while (C_fiscale[0] == '\0');
}

void Signup(int sfd) {
    p_auth registrazione;
    registrazione.richiesta = '2';
    char buf[BUFFSIZE], ris;

    do {
        clear();
        Logo();
        fputs("REGISTRAZIONE\n", stdout);
        fputs("Inserisci Codice fiscale: ", stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(registrazione.Cod_Fiscale, buf);
        fputs("Password: ", stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(registrazione.passw, buf);
        Write(sfd, &registrazione, sizeof(registrazione));	/* invio pacchetto di richiesta */
        Read(sfd, &buf, 1);		/* lettura esito */
        fputs("Attendere, prego...\n", stdout);
        if (Read(sfd, &ris, 1)) {
            fputs("Errore di connessione con il server.\n", stderr);
            exit(1);
        }
        if (ris == '0'){
            fprintf(stdout, "Registrazione effettuata con successo, benvenuto!\n");
            fputs("Per effettuare l'accesso recarsi alla schermata di login.\n", stdout);
        }
        else
            fputs("CodiceFiscale già esistente.\n", stderr);

        fputs("Premere un tasto per continuare...", stdout);
        getchar();
    } while (ris != '0');
}
void Login(char *C_fiscale, int sfd) {
    char t_sanitaria[20];
    p_auth login;
    char buf[BUFFSIZE], ris;
    login.richiesta = '1';

    do {
        clear();
        Logo();
        fputs("LOGIN\n", stdout);
        fputs("Inserire Codice Fiscale: ", stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(login.Cod_Fiscale, buf);
        fputs("Password: ", stdout);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(login.passw, buf);
        Write(sfd, &login, sizeof(login));	/* invio pacchetto di richiesta */
        Read(sfd, &buf, 1);		/* lettura esito */
        Write(sfd, &login, sizeof(login));		/* invio pacchetto di login */
        fputs("Accesso in corso...\n", stdout);
        if (Read(sfd, &ris, 1)) {
            fputs("Errore di connessione con il Server.\n", stderr);
            exit(1);
        }
        if (ris == '0')
            fprintf(stdout, "Accesso effettuato!\n");
        else {
            if (ris == '1')
                fputs("Utente non valido. Riprovare.\n", stderr);
            else
                fputs("Password non valida. Riprovare.\n", stderr);
            fputs("Premere un tasto per continuare...", stdout);
            getchar();
        }
    } while (ris != '0');
    fprintf(stdout, "Inserisci il numero della tessera sanitaria:\n");
    fgets(t_sanitaria, sizeof(t_sanitaria), stdin);
    infogreenpass(t_sanitaria, sfd);
    strcpy(C_fiscale, login.Cod_Fiscale);
}

void infogreenpass(char *tesserasanitaria, int sfd) {
    char buf[BUFFSIZE], ris;
    Write(sfd, &tesserasanitaria, sizeof(tesserasanitaria));
    Read(sfd, &buf, 1);
    if (Read(sfd, &ris, 1)) {
        fputs("Errore di connessione con il Server.\n", stderr);
        exit(1);
    }
    if (ris == '0') {

        //stampa info Green Pass

    }else if (ris == '1') {
        fputs("Numero tessera sanitaria errato\n", stderr);

    }
}

int main(int argc, char **argv) {
    int                sockfd, port = 8003;
    char               buf[BUFFSIZE],cod_fiscale[16] ;
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
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit (1);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        fprintf(stderr,"Connessione fallita\n");
        exit(1);
    }
    Write(sockfd, "c", 1);           /* identificazione al server */
    Read(sockfd, buf, 1);	              /* lettura esito connessione */

    if (buf[0] == '0')
        fputs("Connesso con serverN.\n", stdout);
    else {
        fputs("Connessione fallita: server non valido.\n", stderr);
        exit(1);
    }
    StartMenu(sockfd, cod_fiscale);
    exit(0);
}