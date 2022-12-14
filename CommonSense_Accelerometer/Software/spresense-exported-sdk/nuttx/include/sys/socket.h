/****************************************************************************
 * include/sys/socket.h
 *
 *   Copyright (C) 2007, 2009, 2011, 2015-2016, 2018 Gregory Nutt. All
 *     rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_SYS_SOCKET_H
#define __INCLUDE_SYS_SOCKET_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <sys/types.h>
#include <sys/uio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* The socket()domain parameter specifies a communication domain; this selects
 * the protocol family which will be used for communication.
 */

/* Supported Protocol Families */

#define PF_UNSPEC      0         /* Protocol family unspecified */
#define PF_UNIX        1         /* Local communication */
#define PF_LOCAL       1         /* Local communication */
#define PF_INET        2         /* IPv4 Internet protocols */
#define PF_INET6      10         /* IPv6 Internet protocols */
#define PF_NETLINK    16         /* Netlink IPC socket */
#define PF_ROUTE      PF_NETLINK /* 4.4BSD Compatibility*/
#define PF_PACKET     17         /* Low level packet interface */
#define PF_BLUETOOTH  31         /* Bluetooth sockets */
#define PF_IEEE802154 36         /* Low level IEEE 802.15.4 radio frame interface */
#define PF_PKTRADIO   64         /* Low level packet radio interface */

/* Supported Address Families. Opengroup.org requires only AF_UNSPEC,
 * AF_UNIX, AF_INET and AF_INET6.
 */

#define AF_UNSPEC      PF_UNSPEC
#define AF_UNIX        PF_UNIX
#define AF_LOCAL       PF_LOCAL
#define AF_INET        PF_INET
#define AF_INET6       PF_INET6
#define AF_NETLINK     PF_NETLINK
#define AF_ROUTE       PF_ROUTE
#define AF_PACKET      PF_PACKET
#define AF_BLUETOOTH   PF_BLUETOOTH
#define AF_IEEE802154  PF_IEEE802154
#define AF_PKTRADIO    PF_PKTRADIO

/* The socket created by socket() has the indicated type, which specifies
 * the communication semantics.
 */

#define SOCK_UNSPEC    0 /* Unspecified socket type */
#define SOCK_STREAM    1 /* Provides sequenced, reliable, two-way,
                          * connection-based byte streams. An out-of-band data
                          * transmission mechanism may be supported.
                          */
#define SOCK_DGRAM     2 /* Supports  datagrams (connectionless, unreliable
                          * messages of a fixed maximum length).
                          */
#define SOCK_RAW       3 /* Provides raw network protocol access. */
#define SOCK_RDM       4 /* Provides a reliable datagram layer that does not
                          * guarantee ordering.
                          */
#define SOCK_SEQPACKET 5 /* Provides a sequenced, reliable, two-way
                          * connection-based data transmission path for
                          * datagrams of fixed maximum length; a consumer is
                          * required to read an entire packet with each read
                          * system call.
                          */
#define SOCK_PACKET    10 /* Obsolete and should not be used in new programs */

/* Bits in the FLAGS argument to `send', `recv', et al. These are the bits
 * recognized by Linus, not all are supported by NuttX.
 */

#define MSG_OOB        0x0001 /* Process out-of-band data.  */
#define MSG_PEEK       0x0002 /* Peek at incoming messages.  */
#define MSG_DONTROUTE  0x0004 /* Don't use local routing.  */
#define MSG_CTRUNC     0x0008 /* Control data lost before delivery.  */
#define MSG_PROXY      0x0010 /* Supply or ask second address.  */
#define MSG_TRUNC      0x0020
#define MSG_DONTWAIT   0x0040 /* Enable nonblocking IO.  */
#define MSG_EOR        0x0080 /* End of record.  */
#define MSG_WAITALL    0x0100 /* Wait for a full request.  */
#define MSG_FIN        0x0200
#define MSG_SYN        0x0400
#define MSG_CONFIRM    0x0800 /* Confirm path validity.  */
#define MSG_RST        0x1000
#define MSG_ERRQUEUE   0x2000 /* Fetch message from error queue.  */
#define MSG_NOSIGNAL   0x4000 /* Do not generate SIGPIPE.  */
#define MSG_MORE       0x8000 /* Sender will send more.  */

/* Protocol levels supported by get/setsockopt(): */

#define SOL_SOCKET      0 /* Only socket-level options supported */

/* Socket-level options */

#define SO_ACCEPTCONN    0 /* Reports whether socket listening is enabled
                            * (get only).
                            * arg: pointer to integer containing a boolean
                            * value
                            */
#define SO_BROADCAST     1 /* Permits sending of broadcast messages (get/set).
                            * arg: pointer to integer containing a boolean
                            * value
                            */
#define SO_DEBUG         2 /* Enables recording of debugging information
                            * (get/set).
                            * arg: pointer to integer containing a boolean
                            * value
                            */
#define SO_DONTROUTE     3 /* Requests that outgoing messages bypass standard
                            * routing (get/set)
                            * arg: pointer to integer containing a boolean
                            * value
                            */
#define SO_ERROR         4 /* Reports and clears error status (get only).
                            * arg: returns an integer value
                            */
#define SO_KEEPALIVE     5 /* Keeps connections active by enabling the periodic
                            * transmission of messages (get/set).
                            * arg:  pointer to integer containing a boolean int
                            * value
                            */
#define SO_LINGER        6 /* Lingers on a close() if data is present (get/set)
                            * arg: struct linger
                            */
#define SO_OOBINLINE     7 /* Leaves received out-of-band data (data marked
                            * urgent) inline
                            * (get/set) arg: pointer to integer containing a
                            * boolean value
                            */
#define SO_RCVBUF        8 /* Sets receive buffer size.
                            * arg: integer value (get/set).
                            */
#define SO_RCVLOWAT      9 /* Sets the minimum number of bytes to process for
                            * socket input (get/set).
                            * arg: integer value
                            */
#define SO_RCVTIMEO     10 /* Sets the timeout value that specifies the maximum
                            * amount of time an input function waits until it
                            * completes (get/set).
                            * arg: struct timeval
                            */
#define SO_REUSEADDR    11 /* Allow reuse of local addresses (get/set)
                            * arg: pointer to integer containing a boolean
                            * value
                            */
#define SO_SNDBUF       12 /* Sets send buffer size (get/set).
                            * arg: integer value
                            */
#define SO_SNDLOWAT     13 /* Sets the minimum number of bytes to process for
                            * socket output (get/set).
                            * arg: integer value
                            */
#define SO_SNDTIMEO     14 /* Sets the timeout value specifying the amount of
                            * time that an output function blocks because flow
                            * control prevents data from being sent(get/set).
                            * arg: struct timeval
                            */
#define SO_TYPE         15 /* Reports the socket type (get only).
                            * return: int
                            */

/* Protocol-level socket operations. */

#define SOL_IP          1 /* See options in include/netinet/ip.h */
#define SOL_IPV6        2 /* See options in include/netinet/ip6.h */
#define SOL_TCP         3 /* See options in include/netinet/tcp.h */
#define SOL_UDP         4 /* See options in include/netinit/udp.h */
#define SOL_HCI         5 /* See options in include/netpacket/bluetooth.h */
#define SOL_L2CAP       6 /* See options in include/netpacket/bluetooth.h */
#define SOL_SCO         7 /* See options in include/netpacket/bluetooth.h */
#define SOL_RFCOMM      8 /* See options in include/netpacket/bluetooth.h */

/* Protocol-level socket options may begin with this value */

#define __SO_PROTOCOL  16

/* Values for the 'how' argument of shutdown() */

#define SHUT_RD         1 /* Bit 0: Disables further receive operations */
#define SHUT_WR         2 /* Bit 1: Disables further send operations */
#define SHUT_RDWR       3 /* Bits 0+1: Disables further send and receive
                           * operations
                           */

/* The maximum backlog queue length */

#ifdef CONFIG_NET_TCPBACKLOG_CONNS
#  define SOMAXCONN CONFIG_NET_TCPBACKLOG_CONNS
#else
#  define SOMAXCONN 0
#endif

/* Definitions associated with sendmsg/recvmsg */

#define CMSG_NXTHDR(mhdr, cmsg) cmsg_nxthdr((mhdr), (cmsg))

#define CMSG_ALIGN(len) \
  (((len)+sizeof(long)-1) & ~(sizeof(long)-1))
#define CMSG_DATA(cmsg) \
  ((void *)((char *)(cmsg) + CMSG_ALIGN(sizeof(struct cmsghdr))))
#define CMSG_SPACE(len) \
  (CMSG_ALIGN(sizeof(struct cmsghdr)) + CMSG_ALIGN(len))
#define CMSG_LEN(len)   \
  (CMSG_ALIGN(sizeof(struct cmsghdr)) + (len))

#define __CMSG_FIRSTHDR(ctl, len) \
  ((len) >= sizeof(struct cmsghdr) ? (FAR struct cmsghdr *)(ctl) : \
   (FAR struct cmsghdr *)NULL)
#define CMSG_FIRSTHDR(msg) \
  __CMSG_FIRSTHDR((msg)->msg_control, (msg)->msg_controllen)

/****************************************************************************
 * Type Definitions
 ****************************************************************************/

 /* sockaddr_storage structure. This structure must be (1) large enough to
  * accommodate all supported protocol-specific address structures, and (2)
  * aligned at an appropriate boundary so that pointers to it can be cast
  * as pointers to protocol-specific address structures and used to access
  * the fields of those structures without alignment problems.
  *
  * REVISIT: sizeof(struct sockaddr_storge) should be 128 bytes.
  */

#ifdef CONFIG_NET_IPv6
struct sockaddr_storage
{
  sa_family_t ss_family;       /* Address family */
  char        ss_data[26];     /* 26-bytes of address data */
};
#else
struct sockaddr_storage
{
  sa_family_t ss_family;       /* Address family */
  char        ss_data[14];     /* 14-bytes of address data */
};
#endif

/* The sockaddr structure is used to define a socket address which is used
 * in the bind(), connect(), getpeername(), getsockname(), recvfrom(), and
 * sendto() functions.
 */

struct sockaddr
{
  sa_family_t sa_family;       /* Address family: See AF_* definitions */
  char        sa_data[14];     /* 14-bytes data (actually variable length) */
};

/* Used with the SO_LINGER socket option */

struct linger
{
  int  l_onoff;   /* Indicates whether linger option is enabled. */
  int  l_linger;  /* Linger time, in seconds. */
};

struct msghdr
{
  void *msg_name;               /* Socket name */
  int msg_namelen;              /* Length of name */
  struct iovec *msg_iov;        /* Data blocks */
  unsigned long msg_iovlen;     /* Number of blocks */
  void *msg_control;            /* Per protocol magic (eg BSD file descriptor passing) */
  unsigned long msg_controllen; /* Length of cmsg list */
  unsigned int msg_flags;
};

struct cmsghdr
{
  unsigned long cmsg_len;       /* Data byte count, including hdr */
  int cmsg_level;               /* Originating protocol */
  int cmsg_type;                /* Protocol-specific type */
};

/****************************************************************************
 * Inline Functions
 ****************************************************************************/

static inline struct cmsghdr *__cmsg_nxthdr(FAR void *__ctl,
                                            unsigned int __size,
                                            FAR struct cmsghdr *__cmsg)
{
  FAR struct cmsghdr *__ptr;

  __ptr = (struct cmsghdr *)(((unsigned char *)__cmsg) + CMSG_ALIGN(__cmsg->cmsg_len));
  if ((unsigned long)((char *)(__ptr + 1) - (char *)__ctl) > __size)
    {
      return (struct cmsghdr *)0;
    }

  return __ptr;
}

static inline struct cmsghdr *cmsg_nxthdr(FAR struct msghdr *__msg,
                                          FAR struct cmsghdr *__cmsg)
{
  return __cmsg_nxthdr(__msg->msg_control, __msg->msg_controllen, __cmsg);
}

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

int socket(int domain, int type, int protocol);
int bind(int sockfd, FAR const struct sockaddr *addr, socklen_t addrlen);
int connect(int sockfd, FAR const struct sockaddr *addr, socklen_t addrlen);

int listen(int sockfd, int backlog);
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);

ssize_t send(int sockfd, FAR const void *buf, size_t len, int flags);
ssize_t sendto(int sockfd, FAR const void *buf, size_t len, int flags,
               FAR const struct sockaddr *to, socklen_t tolen);

ssize_t recv(int sockfd, FAR void *buf, size_t len, int flags);
ssize_t recvfrom(int sockfd, FAR void *buf, size_t len, int flags,
                 FAR struct sockaddr *from, FAR socklen_t *fromlen);

int shutdown(int sockfd, int how);

int setsockopt(int sockfd, int level, int option,
               FAR const void *value, socklen_t value_len);
int getsockopt(int sockfd, int level, int option,
               FAR void *value, FAR socklen_t *value_len);

int getsockname(int sockfd, FAR struct sockaddr *addr,
                FAR socklen_t *addrlen);
int getpeername(int sockfd, FAR struct sockaddr *addr,
                FAR socklen_t *addrlen);

ssize_t recvmsg(int sockfd, FAR struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, FAR struct msghdr *msg, int flags);

#undef EXTERN
#if defined(__cplusplus)
}
#endif

#endif /* __INCLUDE_SYS_SOCKET_H */
