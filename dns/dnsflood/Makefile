# New ports collection makefile for:	dnsflood
# Date created:				17.10.2003
# Whom:					krion
#
# $FreeBSD$
#

PORTNAME=	dnsflood
PORTVERSION=	1.12
PORTREVISION=	1
CATEGORIES=	dns
MASTER_SITES=	http://www.adotout.com/
EXTRACT_SUFX=	.tgz

MAINTAINER=	ports@FreeBSD.org
COMMENT=	DNS flood detector

WRKSRC=		${WRKDIR}/dns_flood_detector

PLIST_FILES=	sbin/dns_flood_detector
PORTDOCS=	README

pre-patch:
	${CP} ${WRKSRC}/makefiles/Makefile-FreeBSD ${WRKSRC}/Makefile

post-patch:
	@${REINPLACE_CMD} -e 's|-pthread|${PTHREAD_LIBS}|g' \
		${WRKSRC}/Makefile

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/dns_flood_detector ${PREFIX}/sbin

post-install:
.if !defined(NOPORTDOCS)
	${MKDIR} ${DOCSDIR}
.for i in README
	${INSTALL_DATA} ${WRKSRC}/${i} ${DOCSDIR}
.endfor
.endif

.include <bsd.port.mk>
