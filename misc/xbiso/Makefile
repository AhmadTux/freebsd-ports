# ex:ts=8
# Ports collection makefile for:	xbiso
# Date created:			Jan 28, 2004
# Whom:				ijliao	ij
#
# $FreeBSD$
#

PORTNAME=	xbiso
PORTVERSION=	0.6.1
CATEGORIES=	misc
MASTER_SITES=	SF/${PORTNAME}/${PORTNAME}/${PORTNAME}-${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	XBox xdvdfs iso extraction utility

LIB_DEPENDS=	ftp.3:${PORTSDIR}/ftp/ftplib

GNU_CONFIGURE=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

PLIST_FILES=	bin/xbiso

post-patch:
	@${REINPLACE_CMD} -e 's|$$(CPPFLAGS)|@CPPFLAGS@|g ; \
		 s|= @LIBS@|= @LDFLAGS@ @LIBS@|g' ${WRKSRC}/Makefile.in

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/xbiso ${PREFIX}/bin

.include <bsd.port.mk>
