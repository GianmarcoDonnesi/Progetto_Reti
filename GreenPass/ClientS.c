#include "Header/ClientS.h"

void Logo () {
    fputs("*****************************\n", stdout);
    fputs("******** CLIENT S ***********\n", stdout);
    fputs("*****************************\n", stdout);
    fputs("+++ Controllo validità GreenPass +++\n", stdout);
    fputs("____________________________________\n", stdout);
}

void verificavalidita(int sockfd){
    p_richiestaS richiestaS;
    char buf[BUFFSIZE];

    fputs("Inserire il codice della tessera sanitaria da controllare: ", stdout);
    fgets(buf, sizeof(buf), stdin);
    if(buf[0] != '\n'){
        buf[strcspn(buf, "\r\n")] = '\0';
        strcpy(richiestaS.numero_tessera, buf);
        fprintf(stdout, "Richiesta controllo validità per %s.\n", richiestaS.numero_tessera);
        Write(sockfd, &richiestaS, sizeof(richiestaS));
        Read(sockfd, &buf, 1);
        if(buf[0] == '0'){
            //inserire validità tessera
            fputs("Operazione completata con successo.\n", stdout);
        }else{
            fputs("Operazione fallita.\n", stderr);
        }
    }
}
