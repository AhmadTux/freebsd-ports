# New ports collection makefile for:	goonies
# Date created:		07 Sep 2006
# Whom:			Dmitry Marakasov <amdmi3@amdmi3.ru>
#
# $FreeBSD$
#

PORTNAME=	goonies
PORTVERSION=	1.0.1
PORTREVISION=	7
CATEGORIES=	games
MASTER_SITES=	http://goonies.jorito.net/download/ \
		http://mirror.amdmi3.ru/distfiles/
DISTNAME=	${PORTNAME}_r${PORTVERSION:C/\./-/g}
EXTRACT_SUFX=	.tgz

MAINTAINER=	amdmi3@FreeBSD.org
COMMENT=	Remake of the 8 bit classic game 'The Goonies'

USE_GMAKE=	yes
USE_SDL=	sdl mixer image
USE_GL=		yes

BUILD_WRKSRC=	${WRKSRC}/src
MAKEFILE=	../build/linux/Makefile
MAKE_ENV=	DATADIR="${DATADIR}"
MAKE_JOBS_SAFE=	yes

DATADIRS=	sound graphics maps
PORTDOCS=	*

.include <bsd.port.pre.mk>

.if ${ARCH} == "sparc64"
BROKEN=		Does not compile on sparc64
.endif

post-patch:
.if ${OSVERSION} < 800501 || (${OSVERSION} >= 900000 && ${OSVERSION} < 900006)
	@${REINPLACE_CMD} -e 's|__APPLE__|__FreeBSD__|' \
		${WRKSRC}/src/SFXManager.cpp
.endif
	@${REINPLACE_CMD} -e 's|sound/|${DATADIR}/sound/|g' \
		${WRKSRC}/src/SFXManager.cpp
	@${REINPLACE_CMD} -e 's|graphics/|${DATADIR}/graphics/|g' \
		${WRKSRC}/src/GLTManager.cpp ${WRKSRC}/src/TheGooniesApp.cpp
	@${REINPLACE_CMD} -e 's|maps/|${DATADIR}/maps/|g' \
		${WRKSRC}/src/TheGoonies.cpp

do-install:
	${INSTALL_PROGRAM} ${BUILD_WRKSRC}/goonies ${PREFIX}/bin
	cd ${WRKSRC} && \
		${FIND} ${DATADIRS} -type d -exec ${MKDIR} ${DATADIR}/{} \; ;\
		${FIND} ${DATADIRS} -type f -exec ${INSTALL_DATA} {} ${DATADIR}/{} \;

.if !defined(NOPORTDOCS)
	${MKDIR} ${DOCSDIR}
	${INSTALL_DATA} ${WRKSRC}/docs/* ${DOCSDIR}/
.endif

.include <bsd.port.post.mk>
