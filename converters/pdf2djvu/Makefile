# New ports collection makefile for:	pdf2djvu
# Date created:			7 Mar 2008
# Whom:					Denise H. G. <darcsis@gmail.com>
#
# $FreeBSD$
#

PORTNAME=	pdf2djvu
PORTVERSION=	0.5.11
PORTREVISION=	8
CATEGORIES=	converters
MASTER_SITES=	${MASTER_SITE_GOOGLE_CODE}
DISTNAME=	${PORTNAME}_${PORTVERSION}${EXTRACT_SUFFIX}

MAINTAINER=	darcsis@gmail.com
COMMENT=	Pdf2djvu creates DjVu files from PDF files

BUILD_DEPENDS=	${LOCALBASE}/include/pstreams/pstream.h:${PORTSDIR}/devel/pstreams
LIB_DEPENDS=	poppler.18:${PORTSDIR}/graphics/poppler
LIB_DEPENDS+=	djvulibre.24:${PORTSDIR}/graphics/djvulibre-nox11

GNU_CONFIGURE=	yes
CONFIGURE_ENV=	LIBS=${PTHREAD_LIBS}
CPPFLAGS+=	-I${LOCALBASE}/include
USE_GMAKE=	yes

PLIST_FILES=	bin/pdf2djvu
MAN1=		pdf2djvu.1
MANCOMPRESSED=	no

WRKSRC=	${WRKDIR}/${PORTNAME}-${PORTVERSION}

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/${PORTNAME} ${PREFIX}/bin
	${INSTALL_MAN}  ${WRKSRC}/doc/${PORTNAME}.1 ${MANPREFIX}/man/man1

.include <bsd.port.mk>
