# Makefile for Barriers server
PROG=	barriers
SRCS=	main.c barriers.c

DPADD+=	${LIBSYS}
LDADD+=	-lsys

MAN=

BINDIR?= /usr/sbin
FILES=barriers.conf
FILESNAME=barriers
FILESDIR= /etc/system.conf.d

.include <minix.service.mk>
