# ex:ts=8
# Ports collection makefile for:	py-twitter
# Date created:			Apr 13, 2007
# Whom:				ijliao
#
# $FreeBSD$
#

PORTNAME=	twitter
PORTVERSION=	0.8.2
CATEGORIES=	net python
MASTER_SITES=	GOOGLE_CODE
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}
DISTNAME=	python-${PORTNAME}-${PORTVERSION}

MAINTAINER=	python@FreeBSD.org
COMMENT=	A python wrapper around the Twitter API

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}simplejson>0:${PORTSDIR}/devel/py-simplejson \
		${PYTHON_PKGNAMEPREFIX}oauth2>0:${PORTSDIR}/net/py-oauth2
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}simplejson>0:${PORTSDIR}/devel/py-simplejson \
		${PYTHON_PKGNAMEPREFIX}oauth2>0:${PORTSDIR}/net/py-oauth2

PROJECTHOST=	python-twitter
USE_PYTHON=	yes
USE_PYDISTUTILS=	easy_install
PYDISTUTILS_PKGNAME=	python_twitter

.include <bsd.port.mk>
