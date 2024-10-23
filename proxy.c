#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <errno.h>

#include "utils.h"

extern int     sendrequest(int sd);
extern char *  readresponse(int sd);
extern void    forwardresponse(int sd, char *msg);
extern int     startserver();

int main(int argc, char *argv[])
{
  int    servsock;    /* server socket descriptor */

  fd_set livesdset, servsdset, currset;   /* set of live client sockets and set of live http server sockets */
  int maxfd;         /* largest file descriptor number used for select */
  struct pair * table = malloc(sizeof(struct pair)); /* table to keep client<->server pairs */

  char *msg;

  /* check usage */
  if (argc != 1) {
    fprintf(stderr, "usage : %s\n", argv[0]);
    exit(1);
  }

  /* get ready to receive requests */
  servsock = startserver();
  if (servsock == -1) {
    exit(1);
  }

  table->next = NULL;

  /* initialize fd_sets and largest fd number */
  FD_ZERO(&livesdset);
  FD_ZERO(&servsdset);
  FD_ZERO(&currset);
  FD_SET(servsock, &livesdset);
  maxfd = servsock;

  while (1) {
    currset = livesdset;

    /* combine livesdset and servsdset */
    for (int i = 0; i <= maxfd; i++) {
      if (FD_ISSET(i, &servsdset)) {
        FD_SET(i, &currset);
      }
    }

    if (select(maxfd + 1, &currset, NULL, NULL, NULL) < 0) {
        fprintf(stderr, "Can't select.\n");
        continue;
    }

    for (int frsock = 0; frsock <= maxfd; frsock++) {
        if (!FD_ISSET(frsock, &currset)) continue;
        if (frsock == servsock) continue;

        if (FD_ISSET(frsock, &livesdset)) {
            /* input from existing client */
            int newsd = sendrequest(frsock);
            if (!newsd) {
                printf("admin: disconnect from client\n");
                FD_CLR(frsock, &livesdset);
            } else {
                insertpair(table, newsd, frsock);
                FD_SET(newsd, &servsdset);
                if (newsd > maxfd) maxfd = newsd;
            }
        }

        if (FD_ISSET(frsock, &servsdset)) {
            /* input from the http server */
            struct pair *entry = NULL;
            struct pair *delentry;
            msg = readresponse(frsock);
            if (!msg) {
                fprintf(stderr, "error: server died\n");
                exit(1);
            }

            /* forward response to client */
            entry = searchpair(table, frsock);
            if (!entry) {
                fprintf(stderr, "error: could not find matching client sd\n");
                exit(1);
            }

            forwardresponse(entry->clientsd, msg);
            delentry = deletepair(table, entry->serversd);

            FD_CLR(entry->clientsd, &livesdset);
            FD_CLR(entry->serversd, &servsdset);
        }
    }

    /* input from new client */
    if (FD_ISSET(servsock, &currset)) {
        struct sockaddr_in caddr;
        socklen_t clen = sizeof(caddr);
        int csd = accept(servsock, (struct sockaddr*)&caddr, &clen);

        if (csd != -1) {
            FD_SET(csd, &livesdset);
            if (csd > maxfd) maxfd = csd;
        } else {
            perror("accept");
            exit(0);
        }
    }
  }
}
