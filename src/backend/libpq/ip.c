/*-------------------------------------------------------------------------
 *
 * ip.c
 *	  IPv6-aware network access.
 *
<<<<<<< HEAD
 * Portions Copyright (c) 1996-2010, PostgreSQL Global Development Group
=======
 * Portions Copyright (c) 1996-2008, PostgreSQL Global Development Group
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
<<<<<<< HEAD
 *	  $PostgreSQL: pgsql/src/backend/libpq/ip.c,v 1.51 2010/02/26 02:00:43 momjian Exp $
=======
 *	  $PostgreSQL: pgsql/src/backend/libpq/ip.c,v 1.42 2008/01/01 19:45:49 momjian Exp $
>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
 *
 * This file and the IPV6 implementation were initially provided by
 * Nigel Kukard <nkukard@lbsd.net>, Linux Based Systems Design
 * http://www.lbsd.net.
 *
 *-------------------------------------------------------------------------
 */

/* This is intended to be used in both frontend and backend, so use c.h */
#include "c.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#include <arpa/inet.h>
#include <sys/file.h>

#include "libpq/ip.h"


static int range_sockaddr_AF_INET(const struct sockaddr_in * addr,
					   const struct sockaddr_in * netaddr,
					   const struct sockaddr_in * netmask);

#ifdef HAVE_IPV6
static int range_sockaddr_AF_INET6(const struct sockaddr_in6 * addr,
						const struct sockaddr_in6 * netaddr,
						const struct sockaddr_in6 * netmask);
#endif

#ifdef	HAVE_UNIX_SOCKETS
static int getaddrinfo_unix(const char *path,
				 const struct addrinfo * hintsp,
				 struct addrinfo ** result);

static int getnameinfo_unix(const struct sockaddr_un * sa, int salen,
				 char *node, int nodelen,
				 char *service, int servicelen,
				 int flags);
#endif


/*
 *	pg_getaddrinfo_all - get address info for Unix, IPv4 and IPv6 sockets
 */
int
pg_getaddrinfo_all(const char *hostname, const char *servname,
				   const struct addrinfo * hintp, struct addrinfo ** result)
{
	int			rc;

	/* not all versions of getaddrinfo() zero *result on failure */
	*result = NULL;

#ifdef HAVE_UNIX_SOCKETS
	if (hintp->ai_family == AF_UNIX)
		return getaddrinfo_unix(servname, hintp, result);
#endif

	/* NULL has special meaning to getaddrinfo(). */
	rc = getaddrinfo((!hostname || hostname[0] == '\0') ? NULL : hostname,
					 servname, hintp, result);

<<<<<<< HEAD


#if defined(__darwin__)
=======
#ifdef _AIX

>>>>>>> 632e7b6353a99dd139b999efce4cb78db9a1e588
	/*
	 * Attempt to work around some issues on OSX.
	 *
	 * There are times when a process can be in a state where it can no longer call for name resolution services.
	 * This can happen if the processes was forked into the background, and then the interactive session (or ssh session)
	 * was logged off.  This is because the Mach kernel garbage collects the security context for the session.
	 *
	 * The real fix is to make sure we always start the postmasters with launchctl, so they get started in the
	 * right security context.   pg_ctl has been updated to do this, which should solve the problem.
	 *
	 * But, there is always a chance I missed something in how that is supposed to work.
	 * It really makes for strange errors if "localhost" can't be resolved to an IP address, and
	 * if we can resolve "localhost" plus our own machine name, that's good enough for the
	 * GPDB single-node-edition to run.
	 *
	 *
	 * So to make life easier, let's do our own resolution if the hostname is our own machine's name,
	 * or if the hostname is "localhost", and we got an error from getaddrinfo() trying to resolve them the
	 * "normal" way.   This code really shouldn't ever get run, so in the future, when we have confidence
	 * it isn't needed any more, we can take this hack out.
	 *
	 *
	 */
	if (hostname != NULL && (rc == EAI_AGAIN || rc == EAI_NONAME))
	{
		struct addrinfo *ai;
		struct sockaddr_in *psin;
		char myhostname[255];
		myhostname[0] = '\0';

		if (gethostname(myhostname, sizeof(myhostname)) == 0)
		{
			/*
			 * If we got a valid hostname, and the name we are looking up is our own hostname,
			 * we know we are really localhost, and can give back the loopback address.
			 *
			 * "localhost" should have resolved by getaddrinfo, but if not, it too is a safe
			 * host name to convert to the loopback address.
			 */
			if (((strlen(myhostname) > 0) && (strncmp(myhostname,hostname,255) == 0)) ||
					(strcmp("localhost",hostname) == 0))
			{
				/*
				 * This is a little dangerous... If we later call freeaddrinfo(), there is no guarantee that
				 * the memory getaddrinfo allocates was done this way, so freeaddrinfo might try some other
				 * way to release the memory beside the two free() calls.
				 * But it probably does, and we are only worried about one OS here:  OSX.
				 * So testing should show up any issue.
				 */
				ai = malloc(sizeof(*ai));
				if (!ai)
					return EAI_MEMORY;

				psin = malloc(sizeof(*psin));
				if (!psin)
				{
					free(ai);
					return EAI_MEMORY;
				}

				memset(psin, 0, sizeof(struct sockaddr_in));

#ifdef HAVE_STRUCT_SOCKADDR_STORAGE_SS_LEN
				psin->sin_len = sizeof(struct sockaddr_in);
#endif
				psin->sin_family = AF_INET;

				/* This only fills in sin_port if servname was numeric.  If it was a name from /etc/services, it returns 0 */
				if (servname)
						psin->sin_port = htons((unsigned short) atoi(servname));
				psin->sin_addr.s_addr = htonl(INADDR_LOOPBACK);

				ai->ai_flags = 0;
				ai->ai_family = AF_INET;
				ai->ai_socktype = SOCK_STREAM;
				ai->ai_protocol = 0;
				ai->ai_addrlen = sizeof(*psin);
				ai->ai_addr = (struct sockaddr *) psin;
				ai->ai_canonname = NULL;
				ai->ai_next = NULL;

				*result = ai;
				rc = 0;
			}
		}

	}
#endif

	/*
	 * If we get EAI_AGAIN, the error might be temporary.   Retry it to see.
	 * Note:  Mac OSX Leopard seems to give this error for NXDOMAIN errors, which aren't
	 * really temporary, but retrying doesn't cause any real issue.
	 */
	if (rc == EAI_AGAIN)
	{
#ifndef FRONTEND
		pg_usleep(1000);
#endif
		rc = getaddrinfo((!hostname || hostname[0] == '\0') ? NULL : hostname,
							 servname, hintp, result);
	}

	return rc;
}


/*
 *	pg_freeaddrinfo_all - free addrinfo structures for IPv4, IPv6, or Unix
 *
 * Note: the ai_family field of the original hint structure must be passed
 * so that we can tell whether the addrinfo struct was built by the system's
 * getaddrinfo() routine or our own getaddrinfo_unix() routine.  Some versions
 * of getaddrinfo() might be willing to return AF_UNIX addresses, so it's
 * not safe to look at ai_family in the addrinfo itself.
 */
void
pg_freeaddrinfo_all(int hint_ai_family, struct addrinfo * ai)
{
#ifdef HAVE_UNIX_SOCKETS
	if (hint_ai_family == AF_UNIX)
	{
		/* struct was built by getaddrinfo_unix (see pg_getaddrinfo_all) */
		while (ai != NULL)
		{
			struct addrinfo *p = ai;

			ai = ai->ai_next;
			free(p->ai_addr);
			free(p);
		}
	}
	else
#endif   /* HAVE_UNIX_SOCKETS */
	{
		/* struct was built by getaddrinfo() */
		if (ai != NULL)
			freeaddrinfo(ai);
	}
}


/*
 *	pg_getnameinfo_all - get name info for Unix, IPv4 and IPv6 sockets
 *
 * The API of this routine differs from the standard getnameinfo() definition
 * in two ways: first, the addr parameter is declared as sockaddr_storage
 * rather than struct sockaddr, and second, the node and service fields are
 * guaranteed to be filled with something even on failure return.
 */
int
pg_getnameinfo_all(const struct sockaddr_storage * addr, int salen,
				   char *node, int nodelen,
				   char *service, int servicelen,
				   int flags)
{
	int			rc;

#ifdef HAVE_UNIX_SOCKETS
	if (addr && addr->ss_family == AF_UNIX)
		rc = getnameinfo_unix((const struct sockaddr_un *) addr, salen,
							  node, nodelen,
							  service, servicelen,
							  flags);
	else
#endif
		rc = getnameinfo((const struct sockaddr *) addr, salen,
						 node, nodelen,
						 service, servicelen,
						 flags);

	if (rc != 0)
	{
		if (node)
			strlcpy(node, "???", nodelen);
		if (service)
			strlcpy(service, "???", servicelen);
	}

	return rc;
}


#if defined(HAVE_UNIX_SOCKETS)

/* -------
 *	getaddrinfo_unix - get unix socket info using IPv6-compatible API
 *
 *	Bugs: only one addrinfo is set even though hintsp is NULL or
 *		  ai_socktype is 0
 *		  AI_CANONNAME is not supported.
 * -------
 */
static int
getaddrinfo_unix(const char *path, const struct addrinfo * hintsp,
				 struct addrinfo ** result)
{
	struct addrinfo hints;
	struct addrinfo *aip;
	struct sockaddr_un *unp;

	*result = NULL;

	MemSet(&hints, 0, sizeof(hints));

#if defined(_AIX) && (_SS_MAXSIZE == 1025 || _SS_MAX_SIZE == 1280)
	if (strlen(path) >= 108)
#else
	if (strlen(path) >= sizeof(unp->sun_path))
#endif
		return EAI_FAIL;

	if (hintsp == NULL)
	{
		hints.ai_family = AF_UNIX;
		hints.ai_socktype = SOCK_STREAM;
	}
	else
		memcpy(&hints, hintsp, sizeof(hints));

	if (hints.ai_socktype == 0)
		hints.ai_socktype = SOCK_STREAM;

	if (hints.ai_family != AF_UNIX)
	{
		/* shouldn't have been called */
		return EAI_FAIL;
	}

	aip = calloc(1, sizeof(struct addrinfo));
	if (aip == NULL)
		return EAI_MEMORY;

	unp = calloc(1, sizeof(struct sockaddr_un));
	if (unp == NULL)
	{
		free(aip);
		return EAI_MEMORY;
	}

	aip->ai_family = AF_UNIX;
	aip->ai_socktype = hints.ai_socktype;
	aip->ai_protocol = hints.ai_protocol;
	aip->ai_next = NULL;
	aip->ai_canonname = NULL;
	*result = aip;

	unp->sun_family = AF_UNIX;
	aip->ai_addr = (struct sockaddr *) unp;
	aip->ai_addrlen = sizeof(struct sockaddr_un);

	strcpy(unp->sun_path, path);

#ifdef HAVE_STRUCT_SOCKADDR_STORAGE_SS_LEN
#if defined(_AIX) && (_SS_MAXSIZE == 1025 || _SS_MAXSIZE == 1280)
	unp->sun_len = 112;
#else
	unp->sun_len = sizeof(struct sockaddr_un);
#endif
#endif

	return 0;
}

/*
 * Convert an address to a hostname.
 */
static int
getnameinfo_unix(const struct sockaddr_un * sa, int salen,
				 char *node, int nodelen,
				 char *service, int servicelen,
				 int flags)
{
	int			ret = -1;

	/* Invalid arguments. */
	if (sa == NULL || sa->sun_family != AF_UNIX ||
		(node == NULL && service == NULL))
		return EAI_FAIL;

	/* We don't support those. */
	if ((node && !(flags & NI_NUMERICHOST))
		|| (service && !(flags & NI_NUMERICSERV)))
		return EAI_FAIL;

	if (node)
	{
		ret = snprintf(node, nodelen, "%s", "[local]");
		if (ret == -1 || ret > nodelen)
			return EAI_MEMORY;
	}

	if (service)
	{
		ret = snprintf(service, servicelen, "%s", sa->sun_path);
		if (ret == -1 || ret > servicelen)
			return EAI_MEMORY;
	}

	return 0;
}
#endif   /* HAVE_UNIX_SOCKETS */


/*
 * pg_range_sockaddr - is addr within the subnet specified by netaddr/netmask ?
 *
 * Note: caller must already have verified that all three addresses are
 * in the same address family; and AF_UNIX addresses are not supported.
 */
int
pg_range_sockaddr(const struct sockaddr_storage * addr,
				  const struct sockaddr_storage * netaddr,
				  const struct sockaddr_storage * netmask)
{
	if (addr->ss_family == AF_INET)
		return range_sockaddr_AF_INET((struct sockaddr_in *) addr,
									  (struct sockaddr_in *) netaddr,
									  (struct sockaddr_in *) netmask);
#ifdef HAVE_IPV6
	else if (addr->ss_family == AF_INET6)
		return range_sockaddr_AF_INET6((struct sockaddr_in6 *) addr,
									   (struct sockaddr_in6 *) netaddr,
									   (struct sockaddr_in6 *) netmask);
#endif
	else
		return 0;
}

static int
range_sockaddr_AF_INET(const struct sockaddr_in * addr,
					   const struct sockaddr_in * netaddr,
					   const struct sockaddr_in * netmask)
{
	if (((addr->sin_addr.s_addr ^ netaddr->sin_addr.s_addr) &
		 netmask->sin_addr.s_addr) == 0)
		return 1;
	else
		return 0;
}


#ifdef HAVE_IPV6

static int
range_sockaddr_AF_INET6(const struct sockaddr_in6 * addr,
						const struct sockaddr_in6 * netaddr,
						const struct sockaddr_in6 * netmask)
{
	int			i;

	for (i = 0; i < 16; i++)
	{
		if (((addr->sin6_addr.s6_addr[i] ^ netaddr->sin6_addr.s6_addr[i]) &
			 netmask->sin6_addr.s6_addr[i]) != 0)
			return 0;
	}

	return 1;
}
#endif   /* HAVE_IPV6 */

/*
 *	pg_sockaddr_cidr_mask - make a network mask of the appropriate family
 *	  and required number of significant bits
 *
 * numbits can be null, in which case the mask is fully set.
 *
 * The resulting mask is placed in *mask, which had better be big enough.
 *
 * Return value is 0 if okay, -1 if not.
 */
int
pg_sockaddr_cidr_mask(struct sockaddr_storage * mask, char *numbits, int family)
{
	long		bits;
	char	   *endptr;

	if (numbits == NULL)
	{
		bits = (family == AF_INET) ? 32 : 128;
	}
	else
	{
		bits = strtol(numbits, &endptr, 10);
		if (*numbits == '\0' || *endptr != '\0')
			return -1;
	}

	switch (family)
	{
		case AF_INET:
			{
				struct sockaddr_in mask4;
				long		maskl;

				if (bits < 0 || bits > 32)
					return -1;
				memset(&mask4, 0, sizeof(mask4));
				/* avoid "x << 32", which is not portable */
				if (bits > 0)
					maskl = (0xffffffffUL << (32 - (int) bits))
						& 0xffffffffUL;
				else
					maskl = 0;
				mask4.sin_addr.s_addr = htonl(maskl);
				memcpy(mask, &mask4, sizeof(mask4));
				break;
			}

#ifdef HAVE_IPV6
		case AF_INET6:
			{
				struct sockaddr_in6 mask6;
				int			i;

				if (bits < 0 || bits > 128)
					return -1;
				memset(&mask6, 0, sizeof(mask6));
				for (i = 0; i < 16; i++)
				{
					if (bits <= 0)
						mask6.sin6_addr.s6_addr[i] = 0;
					else if (bits >= 8)
						mask6.sin6_addr.s6_addr[i] = 0xff;
					else
					{
						mask6.sin6_addr.s6_addr[i] =
							(0xff << (8 - (int) bits)) & 0xff;
					}
					bits -= 8;
				}
				memcpy(mask, &mask6, sizeof(mask6));
				break;
			}
#endif
		default:
			return -1;
	}

	mask->ss_family = family;
	return 0;
}


#ifdef HAVE_IPV6

/*
 * pg_promote_v4_to_v6_addr --- convert an AF_INET addr to AF_INET6, using
 *		the standard convention for IPv4 addresses mapped into IPv6 world
 *
 * The passed addr is modified in place; be sure it is large enough to
 * hold the result!  Note that we only worry about setting the fields
 * that pg_range_sockaddr will look at.
 */
void
pg_promote_v4_to_v6_addr(struct sockaddr_storage * addr)
{
	struct sockaddr_in addr4;
	struct sockaddr_in6 addr6;
	uint32		ip4addr;

	memcpy(&addr4, addr, sizeof(addr4));
	ip4addr = ntohl(addr4.sin_addr.s_addr);

	memset(&addr6, 0, sizeof(addr6));

	addr6.sin6_family = AF_INET6;

	addr6.sin6_addr.s6_addr[10] = 0xff;
	addr6.sin6_addr.s6_addr[11] = 0xff;
	addr6.sin6_addr.s6_addr[12] = (ip4addr >> 24) & 0xFF;
	addr6.sin6_addr.s6_addr[13] = (ip4addr >> 16) & 0xFF;
	addr6.sin6_addr.s6_addr[14] = (ip4addr >> 8) & 0xFF;
	addr6.sin6_addr.s6_addr[15] = (ip4addr) & 0xFF;

	memcpy(addr, &addr6, sizeof(addr6));
}

/*
 * pg_promote_v4_to_v6_mask --- convert an AF_INET netmask to AF_INET6, using
 *		the standard convention for IPv4 addresses mapped into IPv6 world
 *
 * This must be different from pg_promote_v4_to_v6_addr because we want to
 * set the high-order bits to 1's not 0's.
 *
 * The passed addr is modified in place; be sure it is large enough to
 * hold the result!  Note that we only worry about setting the fields
 * that pg_range_sockaddr will look at.
 */
void
pg_promote_v4_to_v6_mask(struct sockaddr_storage * addr)
{
	struct sockaddr_in addr4;
	struct sockaddr_in6 addr6;
	uint32		ip4addr;
	int			i;

	memcpy(&addr4, addr, sizeof(addr4));
	ip4addr = ntohl(addr4.sin_addr.s_addr);

	memset(&addr6, 0, sizeof(addr6));

	addr6.sin6_family = AF_INET6;

	for (i = 0; i < 12; i++)
		addr6.sin6_addr.s6_addr[i] = 0xff;

	addr6.sin6_addr.s6_addr[12] = (ip4addr >> 24) & 0xFF;
	addr6.sin6_addr.s6_addr[13] = (ip4addr >> 16) & 0xFF;
	addr6.sin6_addr.s6_addr[14] = (ip4addr >> 8) & 0xFF;
	addr6.sin6_addr.s6_addr[15] = (ip4addr) & 0xFF;

	memcpy(addr, &addr6, sizeof(addr6));
}
#endif   /* HAVE_IPV6 */


/*
 * Run the callback function for the addr/mask, after making sure the
 * mask is sane for the addr.
 */
static void
run_ifaddr_callback(PgIfAddrCallback callback, void *cb_data,
					struct sockaddr * addr, struct sockaddr * mask)
{
	struct sockaddr_storage fullmask;

	if (!addr)
		return;

	/* Check that the mask is valid */
	if (mask)
	{
		if (mask->sa_family != addr->sa_family)
		{
			mask = NULL;
		}
		else if (mask->sa_family == AF_INET)
		{
			if (((struct sockaddr_in *) mask)->sin_addr.s_addr == INADDR_ANY)
				mask = NULL;
		}
#ifdef HAVE_IPV6
		else if (mask->sa_family == AF_INET6)
		{
			if (IN6_IS_ADDR_UNSPECIFIED(&((struct sockaddr_in6 *) mask)->sin6_addr))
				mask = NULL;
		}
#endif
	}

	/* If mask is invalid, generate our own fully-set mask */
	if (!mask)
	{
		pg_sockaddr_cidr_mask(&fullmask, NULL, addr->sa_family);
		mask = (struct sockaddr *) & fullmask;
	}

	(*callback) (addr, mask, cb_data);
}

#ifdef WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

/*
 * Enumerate the system's network interface addresses and call the callback
 * for each one.  Returns 0 if successful, -1 if trouble.
 *
 * This version is for Win32.  Uses the Winsock 2 functions (ie: ws2_32.dll)
 */
int
pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data)
{
	INTERFACE_INFO *ptr,
			   *ii = NULL;
	unsigned long length,
				i;
	unsigned long n_ii = 0;
	SOCKET		sock;
	int			error;

	sock = WSASocket(AF_INET, SOCK_DGRAM, 0, 0, 0, 0);
	if (sock == SOCKET_ERROR)
		return -1;

	while (n_ii < 1024)
	{
		n_ii += 64;
		ptr = realloc(ii, sizeof(INTERFACE_INFO) * n_ii);
		if (!ptr)
		{
			free(ii);
			closesocket(sock);
			errno = ENOMEM;
			return -1;
		}

		ii = ptr;
		if (WSAIoctl(sock, SIO_GET_INTERFACE_LIST, 0, 0,
					 ii, n_ii * sizeof(INTERFACE_INFO),
					 &length, 0, 0) == SOCKET_ERROR)
		{
			error = WSAGetLastError();
			if (error == WSAEFAULT || error == WSAENOBUFS)
				continue;		/* need to make the buffer bigger */
			closesocket(sock);
			free(ii);
			return -1;
		}

		break;
	}

	for (i = 0; i < length / sizeof(INTERFACE_INFO); ++i)
		run_ifaddr_callback(callback, cb_data,
							(struct sockaddr *) & ii[i].iiAddress,
							(struct sockaddr *) & ii[i].iiNetmask);

	closesocket(sock);
	free(ii);
	return 0;
}
#elif HAVE_GETIFADDRS			/* && !WIN32 */

#ifdef HAVE_IFADDRS_H
#include <ifaddrs.h>
#endif

/*
 * Enumerate the system's network interface addresses and call the callback
 * for each one.  Returns 0 if successful, -1 if trouble.
 *
 * This version uses the getifaddrs() interface, which is available on
 * BSDs, AIX, and modern Linux.
 */
int
pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data)
{
	struct ifaddrs *ifa,
			   *l;

	if (getifaddrs(&ifa) < 0)
		return -1;

	for (l = ifa; l; l = l->ifa_next)
		run_ifaddr_callback(callback, cb_data,
							l->ifa_addr, l->ifa_netmask);

	freeifaddrs(ifa);
	return 0;
}
#else							/* !HAVE_GETIFADDRS && !WIN32 */

#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#ifdef HAVE_NET_IF_H
#include <net/if.h>
#endif

#ifdef HAVE_SYS_SOCKIO_H
#include <sys/sockio.h>
#endif

/*
 * SIOCGIFCONF does not return IPv6 addresses on Solaris
 * and HP/UX. So we prefer SIOCGLIFCONF if it's available.
 */

#if defined(SIOCGLIFCONF) && !defined(__hpux)

/*
 * Enumerate the system's network interface addresses and call the callback
 * for each one.  Returns 0 if successful, -1 if trouble.
 *
 * This version uses ioctl(SIOCGLIFCONF).
 */
int
pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data)
{
	struct lifconf lifc;
	struct lifreq *lifr,
				lmask;
	struct sockaddr *addr,
			   *mask;
	char	   *ptr,
			   *buffer = NULL;
	size_t		n_buffer = 1024;
	pgsocket	sock,
				fd;

#ifdef HAVE_IPV6
	pgsocket	sock6;
#endif
	int			i,
				total;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		return -1;

	while (n_buffer < 1024 * 100)
	{
		n_buffer += 1024;
		ptr = realloc(buffer, n_buffer);
		if (!ptr)
		{
			free(buffer);
			close(sock);
			errno = ENOMEM;
			return -1;
		}

		memset(&lifc, 0, sizeof(lifc));
		lifc.lifc_family = AF_UNSPEC;
		lifc.lifc_buf = buffer = ptr;
		lifc.lifc_len = n_buffer;

		if (ioctl(sock, SIOCGLIFCONF, &lifc) < 0)
		{
			if (errno == EINVAL)
				continue;
			free(buffer);
			close(sock);
			return -1;
		}

		/*
		 * Some Unixes try to return as much data as possible, with no
		 * indication of whether enough space allocated. Don't believe we have
		 * it all unless there's lots of slop.
		 */
		if (lifc.lifc_len < n_buffer - 1024)
			break;
	}

#ifdef HAVE_IPV6
	/* We'll need an IPv6 socket too for the SIOCGLIFNETMASK ioctls */
	sock6 = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sock6 == -1)
	{
		free(buffer);
		close(sock);
		return -1;
	}
#endif

	total = lifc.lifc_len / sizeof(struct lifreq);
	lifr = lifc.lifc_req;
	for (i = 0; i < total; ++i)
	{
		addr = (struct sockaddr *) & lifr[i].lifr_addr;
		memcpy(&lmask, &lifr[i], sizeof(struct lifreq));
#ifdef HAVE_IPV6
		fd = (addr->sa_family == AF_INET6) ? sock6 : sock;
#else
		fd = sock;
#endif
		if (ioctl(fd, SIOCGLIFNETMASK, &lmask) < 0)
			mask = NULL;
		else
			mask = (struct sockaddr *) & lmask.lifr_addr;
		run_ifaddr_callback(callback, cb_data, addr, mask);
	}

	free(buffer);
	close(sock);
#ifdef HAVE_IPV6
	close(sock6);
#endif
	return 0;
}
#elif defined(SIOCGIFCONF)

/*
 * Remaining Unixes use SIOCGIFCONF. Some only return IPv4 information
 * here, so this is the least preferred method. Note that there is no
 * standard way to iterate the struct ifreq returned in the array.
 * On some OSs the structures are padded large enough for any address,
 * on others you have to calculate the size of the struct ifreq.
 */

/* Some OSs have _SIZEOF_ADDR_IFREQ, so just use that */
#ifndef _SIZEOF_ADDR_IFREQ

/* Calculate based on sockaddr.sa_len */
#ifdef HAVE_STRUCT_SOCKADDR_SA_LEN
#define _SIZEOF_ADDR_IFREQ(ifr) \
		((ifr).ifr_addr.sa_len > sizeof(struct sockaddr) ? \
		 (sizeof(struct ifreq) - sizeof(struct sockaddr) + \
		  (ifr).ifr_addr.sa_len) : sizeof(struct ifreq))

/* Padded ifreq structure, simple */
#else
#define _SIZEOF_ADDR_IFREQ(ifr) \
	sizeof (struct ifreq)
#endif
#endif   /* !_SIZEOF_ADDR_IFREQ */

/*
 * Enumerate the system's network interface addresses and call the callback
 * for each one.  Returns 0 if successful, -1 if trouble.
 *
 * This version uses ioctl(SIOCGIFCONF).
 */
int
pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data)
{
	struct ifconf ifc;
	struct ifreq *ifr,
			   *end,
				addr,
				mask;
	char	   *ptr,
			   *buffer = NULL;
	size_t		n_buffer = 1024;
	int			sock;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		return -1;

	while (n_buffer < 1024 * 100)
	{
		n_buffer += 1024;
		ptr = realloc(buffer, n_buffer);
		if (!ptr)
		{
			free(buffer);
			close(sock);
			errno = ENOMEM;
			return -1;
		}

		memset(&ifc, 0, sizeof(ifc));
		ifc.ifc_buf = buffer = ptr;
		ifc.ifc_len = n_buffer;

		if (ioctl(sock, SIOCGIFCONF, &ifc) < 0)
		{
			if (errno == EINVAL)
				continue;
			free(buffer);
			close(sock);
			return -1;
		}

		/*
		 * Some Unixes try to return as much data as possible, with no
		 * indication of whether enough space allocated. Don't believe we have
		 * it all unless there's lots of slop.
		 */
		if (ifc.ifc_len < n_buffer - 1024)
			break;
	}

	end = (struct ifreq *) (buffer + ifc.ifc_len);
	for (ifr = ifc.ifc_req; ifr < end;)
	{
		memcpy(&addr, ifr, sizeof(addr));
		memcpy(&mask, ifr, sizeof(mask));
		if (ioctl(sock, SIOCGIFADDR, &addr, sizeof(addr)) == 0 &&
			ioctl(sock, SIOCGIFNETMASK, &mask, sizeof(mask)) == 0)
			run_ifaddr_callback(callback, cb_data,
								&addr.ifr_addr, &mask.ifr_addr);
		ifr = (struct ifreq *) ((char *) ifr + _SIZEOF_ADDR_IFREQ(*ifr));
	}

	free(buffer);
	close(sock);
	return 0;
}
#else							/* !defined(SIOCGIFCONF) */

/*
 * Enumerate the system's network interface addresses and call the callback
 * for each one.  Returns 0 if successful, -1 if trouble.
 *
 * This version is our fallback if there's no known way to get the
 * interface addresses.  Just return the standard loopback addresses.
 */
int
pg_foreach_ifaddr(PgIfAddrCallback callback, void *cb_data)
{
	struct sockaddr_in addr;
	struct sockaddr_storage mask;

#ifdef HAVE_IPV6
	struct sockaddr_in6 addr6;
#endif

	/* addr 127.0.0.1/8 */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ntohl(0x7f000001);
	memset(&mask, 0, sizeof(mask));
	pg_sockaddr_cidr_mask(&mask, "8", AF_INET);
	run_ifaddr_callback(callback, cb_data,
						(struct sockaddr *) & addr,
						(struct sockaddr *) & mask);

#ifdef HAVE_IPV6
	/* addr ::1/128 */
	memset(&addr6, 0, sizeof(addr6));
	addr6.sin6_family = AF_INET6;
	addr6.sin6_addr.s6_addr[15] = 1;
	memset(&mask, 0, sizeof(mask));
	pg_sockaddr_cidr_mask(&mask, "128", AF_INET6);
	run_ifaddr_callback(callback, cb_data,
						(struct sockaddr *) & addr6,
						(struct sockaddr *) & mask);
#endif

	return 0;
}
#endif   /* !defined(SIOCGIFCONF) */

#endif   /* !HAVE_GETIFADDRS */
