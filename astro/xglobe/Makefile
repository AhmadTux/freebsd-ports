# New ports collection makefile for:   xglobe
# Date created:        20 January 1998
# Whom:                Kris Kennaway <kris@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	xglobe
PORTVERSION=	0.5
PORTREVISION=	10
CATEGORIES=	astro
MASTER_SITES=	${MASTER_SITE_LOCAL}
MASTER_SITE_SUBDIR=kris

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Displays a view of the Earth (like xearth) with a rendered photo map

USE_QT_VER=	4
QT_COMPONENTS=	corelib gui qt3support moc_build
USE_GMAKE=	yes
MAKE_ENV=	PTHREAD_LIBS=${PTHREAD_LIBS} QT_PREFIX=${QT_PREFIX}

do-install:
	@${MKDIR} ${DATADIR}
	@${INSTALL_PROGRAM} ${WRKSRC}/xglobe ${PREFIX}/bin/
	@${INSTALL_DATA} ${WRKSRC}/map.bmp ${DATADIR}
	@${INSTALL_DATA} ${WRKSRC}/xglobe-markers ${DATADIR}

.include <bsd.port.mk>
