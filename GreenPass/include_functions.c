#include "Header/include_functions.h"


int Socket(int family, int type, int protocol) {
    int fd;
    if ((fd = socket(family, type, protocol)) < 0) {
        perror("socket");
        exit(1);
    }
    return fd;
}

void Connect(int sockfd, struct sockaddr_in *servaddr) {
    if (connect(sockfd,(struct sockaddr *) servaddr, sizeof(*servaddr)) < 0) {
        perror("connect");
        exit(1);
    }
}


void Bind(int sockfd, struct sockaddr_in *servaddr) {
    if (bind(sockfd, (struct sockaddr *) servaddr, sizeof(*servaddr)) < 0) {
        perror("bind");
        exit(1);
    }
}

void Listen(int sockfd, int queue_length) {
    if (listen(sockfd, queue_length) < 0) {
        perror("listen");
        exit(1);
    }
}


int Accept(int sockfd, struct sockaddr_in *clientaddr, socklen_t *addr_dim) {
    int connfd;
    if ((connfd = accept(sockfd, (struct sockaddr *) clientaddr, addr_dim)) < 0){
        perror("accept");
        exit(1);
    }
    return connfd;
}

void Inet_pton(int af, const char *src, void *dst) {
    if (inet_pton(af, src, dst) <= 0) {
        fprintf(stderr,"inet_pton: %s non è valido\n", src);
        exit(1);
    }
}


struct hostent *GetHostByName(const char *name) {
    struct hostent *host = gethostbyname(name);
    if (host == NULL){
        herror("gethostbyname");
        exit(1);
    }
    return host;
}


struct hostent *GetHostByAddr (const char *addr, int type) {
    struct in_addr addr_net;
    struct hostent *host = gethostbyaddr((const char *) &addr_net, sizeof(addr_net), type);

    inet_aton(addr, &addr_net);

    if (host == NULL) {
        herror("gethostbyaddr");
        exit(1);
    }
    return host;
}

void GetSockOpt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) {
    if (getsockopt(sockfd, level, optname, optval, optlen) < 0) {
        perror("getsockopt");
        exit(1);
    }
}


void SetSockOpt(int sockfd, int level, int optname, void *optval, socklen_t optlen) {
    if (setsockopt(sockfd, level, optname, optval, optlen) < 0) {
        perror("setsockopt");
        exit(1);
    }
}

int Select(int maxfdp, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout) {
    int n;
    if ((n = select(maxfdp, readfds, writefds, exceptfds, timeout)) < 0) {
        perror("select");
        exit(1);
    }
    return n;
}

pid_t Fork(void) {
    int pid;
    if ((pid = fork()) < 0) {
        perror ("fork");
        exit(1);
    }
    return pid;
}


ssize_t Read(int fd, void *buf, size_t count) {
    size_t nleft;	/* numero di byte rimanenti da leggere */
    ssize_t nread;	/* numero di byte letti (utilizzato per gestire gli spiazzamenti per *buf) */
    nleft = count;
    memset(buf, 0, count);
    while (nleft > 0) {
        if ((nread = read(fd, buf, nleft)) < 0) {
            if (errno == EINTR)
                continue;			/* continua se read() è interrotta da chiamata di sistema */
            else {
                perror("read");
                exit(nread);
            }
        }
        else
        if (nread == 0) /* EOF */
            break;
        nleft -= nread; /* imposta il numero di byte rimanenti a quelli da leggere */
        buf += nread;	/* spiazzamento puntatore a buffer */
    }
    buf = 0;
    return nleft;
}


ssize_t Write(int fd, const void *buf, size_t count) {
    size_t nleft;		/* numero di byte rimanenti da scrivere */
    ssize_t nwritten;	/* numero di byte scritti (utilizzato per gestire gli spiazzamenti per *buf) */
    nleft = count;
    while (nleft > 0) {
        if ((nwritten = write(fd, buf, nleft)) < 0) {
            if (errno == EINTR)
                continue;			/* continua se write() è interrotta da chiamata di sistema */
            else {
                perror("write");
                exit(nwritten);
            }
        }
        nleft -= nwritten;	/* imposta il numero di byte rimanenti a quelli da scrivere */
        buf +=nwritten;		/* spiazzamento puntatore a buffer */
    }
    return nleft;
}

FILE *Fopen(const char *pathname, const char *mode) {
    FILE *file;
    if ((file = fopen(pathname, mode)) == NULL) {
        perror("fopen");
        exit(1);
    }
    return file;
}

