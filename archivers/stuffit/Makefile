# New ports collection makefile for:    stuffit
# Date created:		12 Dec 2001
# Whom:			Nathan Ahlstrom <nra@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	stuffit
PORTVERSION=	5.2.0.611
PORTREVISION=	1
CATEGORIES=	archivers linux
MASTER_SITES=	http://my.smithmicro.com/downloads/files/
DISTNAME=	stuffit520.611linux-i386

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Stuffit Archive Creator and Expander

RESTRICTED=	Only unmodified original package can be distributed

ONLY_FOR_ARCHS=	i386 amd64

USE_LINUX=	yes
NO_WRKSUBDIR=	yes
STUFFIT_FILES=	bin/unstuff bin/stuff
STUFFIT_MAN=	man/man1/stuff.1 man/man1/unstuff.1
PLIST_FILES=	${STUFFIT_FILES}

MAN1=		stuff.1 unstuff.1

do-build:
.for sFile in ${STUFFIT_FILES}
	${BRANDELF} -t Linux ${WRKSRC}/${sFile}
.endfor

do-install:
.for sFile in ${STUFFIT_FILES}
	${INSTALL_PROGRAM} ${WRKSRC}/${sFile} ${PREFIX}/bin
.endfor
.for mFile in ${STUFFIT_MAN}
	${INSTALL_MAN} ${WRKSRC}/${mFile} ${MANPREFIX}/man/man1
.endfor

.include <bsd.port.mk>
