#ifndef GREENPASS_INCLUDE_FUNCTIONS_H
#define GREENPASS_INCLUDE_FUNCTIONS_H

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>

#define BUFFSIZE 2048
#define clear() printf("\033[H\033[J")



int Socket(int family, int type, int protocol);
void Connect(int sockfd, struct sockaddr_in *servaddr);
void Bind(int sockfd, struct sockaddr_in *servaddr);
void Listen(int sockfd, int queue_length);
int Accept(int sockfd, struct sockaddr_in *clientaddr, socklen_t *addr_dim);
void inet_pton(int af, const char *src, void *dst);
struct hostent *GetHostByName(const char *name);
struct hostent *GetHostByAddr (const char *addr, int type);
void GetSockOpt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
void SetSockOpt(int sockfd, int level, int optname, void *optval, socklen_t optlen);
int Select(int maxfdp, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
pid_t Fork(void);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
FILE *Fopen(const char *pathname, const char *mode);


#endif
