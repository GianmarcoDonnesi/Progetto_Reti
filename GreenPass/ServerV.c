#include "Header/ServerV.h"

void *serverG(void *args){

}


void printUsage(char *prog) {
    fprintf(stderr, "uso: %s [-p <porta>]\n", prog);
    fputs("\t-p <porta>\t\tNumero di porta [default: 8000]\n", stderr);
    exit(1);
}