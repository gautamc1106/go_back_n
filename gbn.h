#ifndef _gbn_h
#define _gbn_h

#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ioctl.h>
#include<signal.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<time.h>
#include<stdbool.h>

/*----- Error variables -----*/
extern int h_errno;
extern int errno;

/*----- Protocol parameters -----*/
#define LOSS_PROB 		 1e-2    /* loss probability                            */
#define CORR_PROB 		 1e-3    /* corruption probability                      */
#define DATALEN  		 1024    /* length of the payload                       */
#define N         		 1024    /* Max number of packets a single call to gbn_send can process */
#define TIMEOUT     	 1    /* timeout to resend packets (1 second)        */
#define MAX_ATTEMPTS	 4	  /* MAX ATTEMPTS BEFORE DISCONNECTING  */
#define DATALEN_BYTES    2 // The number of bytes used in DATA packet to represent the DATALEN
#define MAX_WINDOW_SIZE	 2

/*----- Packet types -----*/
#define SYN      0        /* Opens a connection                          */
#define SYNACK   1        /* Acknowledgement of the SYN packet           */
#define DATA     2        /* Data packets                                */
#define DATAACK  3        /* Acknowledgement of the DATA packet          */
#define FIN      4        /* Ends a connection                           */
#define FINACK   5        /* Acknowledgement of the FIN packet           */
#define RST      6        /* Reset packet used to reject new connections */

/*----- Go-Back-n packet format -----*/
typedef struct {
	uint8_t  type;            /* packet type (e.g. SYN, DATA, ACK, FIN)     */
	uint8_t  seqnum;          /* sequence number of the packet              */
    uint16_t checksum;        /* header and payload checksum                */
    uint8_t data[DATALEN];    /* pointer to the payload                     */
} __attribute__((packed)) gbnhdr;

typedef struct state_t{

	/* TODO: Your state information could be encoded here. */
	int state;
    uint8_t seqnum;
    struct sockaddr address;
    socklen_t sck_len;
    uint8_t window_size;
    bool fin; 
    bool fin_ack; 

} state_t;

enum {
	CLOSED=0,
	SYN_SENT,
	SYN_RCVD,
	ESTABLISHED,
	FIN_SENT,
	FIN_RCVD,
	FIN_WAIT
};

extern state_t s;

void gbn_init();
int gbn_connect(int sockfd, const struct sockaddr *server, socklen_t socklen);
int gbn_listen(int sockfd, int backlog);
int gbn_bind(int sockfd, const struct sockaddr *server, socklen_t socklen);
int gbn_socket(int domain, int type, int protocol);
int gbn_accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int gbn_close(int sockfd);
ssize_t gbn_send(int sockfd, const void *buf, size_t len, int flags);
ssize_t gbn_recv(int sockfd, void *buf, size_t len, int flags);

ssize_t  maybe_sendto(int  s, const void *buf, size_t len, int flags, \
                      const struct sockaddr *to, socklen_t tolen);

uint16_t checksum(uint16_t *buf, int nwords);
uint16_t p_checksum(gbnhdr *packet);

void timeout(int s);

size_t min(size_t a, size_t b);


#endif
