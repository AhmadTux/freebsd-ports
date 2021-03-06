# New ports collection makefile for: uae
# Date created: 22 May 2000
# Whom: Simon Dick <simond@irrelevant.org>
#
# $FreeBSD$
#

PORTNAME=	uae
PORTVERSION=	0.8.29
PORTREVISION=	1
CATEGORIES=	emulators
MASTER_SITES=	http://www.amigaemulator.org/files/sources/develop/

MAINTAINER=	simond@irrelevant.org
COMMENT=	Amiga emulator

CONFLICTS=	e-uae-[0-9]*

USE_BZIP2=	yes
USE_GMAKE=	yes
GNU_CONFIGURE=	yes
USE_AUTOTOOLS=	autoconf
CONFIGURE_ARGS=	--enable-sound
USE_GNOME=	gtk20

CFLAGS+=	${PTHREAD_CFLAGS}

pre-configure:
	@${REINPLACE_CMD} -e 's|-lpthread|${PTHREAD_LIBS}|g' \
		${WRKSRC}/configure.in

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/uae ${PREFIX}/bin
	${INSTALL_PROGRAM} ${WRKSRC}/readdisk ${PREFIX}/bin
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/COMPATIBILITY ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/CREDITS ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/FAQ ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/NEWS ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/README ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/README.PROGRAMMERS ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/unix/README ${DOCSDIR}/README.unix
.endif
	@${MKDIR} ${DATADIR}/amiga
	${INSTALL_DATA} ${WRKSRC}/amiga/mousehack ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/timehack ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/transdisk ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/transrom ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/uae-control ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/uae-control.info ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/uae_rcli ${DATADIR}/amiga/
	${INSTALL_DATA} ${WRKSRC}/amiga/uaectrl ${DATADIR}/amiga/
	@${MKDIR} ${DATADIR}/amiga/devs
	@${MKDIR} ${DATADIR}/amiga/devs/AHI
	@${MKDIR} ${DATADIR}/amiga/devs/AudioModes
	${INSTALL_DATA} ${WRKSRC}/amiga/devs/AHI/uae.audio ${DATADIR}/amiga/devs/AHI/
	${INSTALL_DATA} ${WRKSRC}/amiga/devs/AudioModes/UAE ${DATADIR}/amiga/devs/AudioModes/

.include <bsd.port.pre.mk>

.if ${ARCH} == "powerpc"
BROKEN=		Does not compile on powerpc
.endif

.include <bsd.port.post.mk>
