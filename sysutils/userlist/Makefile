# New ports collection makefile for:	userlist
# Date created:			8 May 2001
# Whom:				Jamie Heckford <jamie@jamiesdomain.co.uk>
#
# $FreeBSD$
#

PORTNAME=	userlist
PORTVERSION=	0.1
CATEGORIES=	sysutils
MASTER_SITES=	# does no longer exist
DISTFILES=	# n/a

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Prints a list of all usernames in /etc/passwd

NO_BUILD=	yes
PLIST_FILES=	bin/userlist

do-install:
	${INSTALL_SCRIPT} ${FILESDIR}/userlist ${PREFIX}/bin

.include <bsd.port.mk>
