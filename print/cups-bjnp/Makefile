# New ports collection makefile for:	cups-bjnp
# Date created:				15 May 2009
# Whom:					shurd@sasktel.net
#
# $FreeBSD$
#

PORTNAME=	cups-bjnp
PORTVERSION=	0.5.3
PORTREVISION=	3
CATEGORIES=	print
MASTER_SITES=	SF

MAINTAINER=	shurd@sasktel.net
COMMENT=	CUPS backend for Canon USB over IP protocol

LIB_DEPENDS=	cups.2:${PORTSDIR}/print/cups-client
BUILD_DEPENDS=	${LOCALBASE}/libexec/cups/backend:${PORTSDIR}/print/cups-base

GNU_CONFIGURE=	yes
CONFIGURE_ENV=	LIBS="-lcompat"
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib

.include <bsd.port.mk>
