# Ports collection makefile for:  horde4-passwd
# Date created:			  Oct 28, 2011
# Whom:				  Martin Matuska (mm@FreeBSD.org)
#
# $FreeBSD$
#

PORTNAME=	passwd
PORTVERSION=	4.0.0
CATEGORIES=	www pear
PKGNAMEPREFIX=	horde-

MAINTAINER=	horde@FreeBSD.org
COMMENT=	Horde password changing application

LICENSE=	GPLv2
LICENSE_FILE=	${WRKSRC}/COPYING

LATEST_LINK=	horde4-${PORTNAME}

USE_PHP=	gettext
USE_HORDE_BUILD=	Horde_Role
USE_HORDE_RUN=	Horde_Role \
		horde \
		Horde_Auth \
		Horde_Core \
		Horde_Exception \
		Horde_Injector \
		Horde_Util \
		Horde_View

.include <bsd.port.pre.mk>
.include "${PORTSDIR}/devel/pear-channel-horde/bsd.horde.mk"
.include <bsd.port.post.mk>
