# ex:ts=8
# New ports collection makefile for:	py-fckhsum
# Date created:			Jul 5, 2001
# Whom:				ijliao
#
# $FreeBSD$
#

PORTNAME=	fchksum
PORTVERSION=	1.7.1
CATEGORIES=	security python
MASTER_SITES=	http://users.dakotacom.net/~donut/programs/fchksum/
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	python-${PORTNAME}-${PORTVERSION}

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Python module to find the checksum of files

PLIST_FILES=	%%PYTHON_SITELIBDIR%%/fchksum.so
USE_PYTHON=	yes
USE_PYDISTUTILS=	yes
PYDISTUTILS_PKGNAME=	python-fchksum

.include <bsd.port.mk>
