# New ports collection makefile for:	spnetkit
# Date created:				18 Feb 2008
# Whom:					vanilla@
#
# $FreeBSD$
#

PORTNAME=	spnetkit
PORTVERSION=	0.3.0
CATEGORIES=	net
MASTER_SITES=	${MASTER_SITE_GOOGLE_CODE}
DISTNAME=	${PORTNAME}-${PORTVERSION}.src

MAINTAINER=	vanilla@FreeBSD.org
COMMENT=	A C++ wrapper for BSD-style sockets

BROKEN=		does not build

WRKSRC=		${WRKDIR}/${PORTNAME}-${PORTVERSION}/spnetkit
USE_LDCONFIG=	yes
USE_GMAKE=	yes
ALL_TARGET=	all ssl

do-install:
	@${MKDIR} ${PREFIX}/include/spnetkit
	@${INSTALL} ${WRKSRC}/*.hpp ${PREFIX}/include/spnetkit
	@${INSTALL} ${WRKSRC}/libspnetkit.so ${PREFIX}/lib/libspnetkit.so.0
	@${LN} -sf ${PREFIX}/lib/libspnetkit.so.0 ${PREFIX}/lib/libspnetkit.so
	@${INSTALL} ${WRKSRC}/libspnetkit_s.so ${PREFIX}/lib/libspnetkit_s.so.0
	@${LN} -sf ${PREFIX}/lib/libspnetkit_s.so.0 ${PREFIX}/lib/libspnetkit_s.so

.include <bsd.port.mk>
