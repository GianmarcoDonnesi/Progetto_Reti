
#include "p_richiestaS.h"
#include "p_richiestaT.h"
#include "p_auth.h"
#include "include_functions.h"

void Logo();
void Login(char *usr_login, int sfd);
void Signup(int sfd);
void infogreenpass(char *tesserasanitaria,int sfd);
void StartMenu(int sockfd, char *usr_login);
void verificavalidita(int sockfd);
void modificavalidita(int sockfd);
