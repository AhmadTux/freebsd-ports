# ex:ts=8
# New ports collection makefile for:	poco-ssl
# Date created:			Jan 28, 2007
# Whom:				wes
#
# $FreeBSD$
#

PORTNAME=	poco
PORTVERSION=	1.4.1
PORTREVISION=	2
CATEGORIES=	devel net
MASTER_SITES=	SF/${PORTNAME}/sources/${PORTNAME}-${PORTVERSION}
PKGNAMESUFFIX=	-ssl
DISTNAME=	poco-${PORTVERSION}p1-all

MAINTAINER=	wes@FreeBSD.org
COMMENT=	C++ Portable Components library with NetSSL library

LIB_DEPENDS=	odbc:${PORTSDIR}/databases/unixODBC

CONFLICTS=	poco-[0-9]*

GNU_CONFIGURE=	yes
USE_GMAKE=	yes
USE_LDCONFIG=	yes

USE_MYSQL=	yes
USE_SQLITE=	yes

.include <bsd.port.mk>
