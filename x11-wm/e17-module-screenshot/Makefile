# New ports collection makefile for:	e17-module-uptime
# Date created:		23 June 2006
# Whom:			Stanislav Sedov <ssedov@mbsd.msk.ru>
#
# $FreeBSD$
#

PORTNAME=	screenshot
PORTVERSION=	20110129
CATEGORIES=	x11-wm enlightenment
MASTER_SITES=	http://files.roorback.net/e17/2011-01-29/modules/
PKGNAMEPREFIX=	e17-module-

MAINTAINER=	ports@FreeBSD.org
COMMENT=	An e17 module which allows users to take screenshots

BUILD_DEPENDS=	enlightenment:${PORTSDIR}/x11-wm/enlightenment \
		emprint:${PORTSDIR}/x11/emprint
RUN_DEPENDS=	enlightenment:${PORTSDIR}/x11-wm/enlightenment \
		emprint:${PORTSDIR}/x11/emprint

DIST_SUBDIR=	e17
GNU_CONFIGURE=	yes
USE_BZIP2=	yes
USE_GMAKE=	yes
USE_GNOME=	pkgconfig
USE_EFL=	edje libtool_hack modarch_hack

.if !defined(WITHOUT_NLS)
USE_GETTEXT=	yes
PLIST_SUB+=	NLS=""
.else
CONFIGURE_ARGS=	--disable-nls
PLIST_SUB+=	NLS="@comment "
.endif

.include <bsd.port.mk>
