# New ports collection makefile for:	gxneur
# Date created:				07 December 2008
# Whom:					Alexander Logvinov <ports@logvinov.com>
#
# $FreeBSD$
#

PORTNAME=	gxneur
PORTVERSION=	0.15.0
CATEGORIES=	deskutils
MASTER_SITES=	http://dists.xneur.ru/release-${PORTVERSION}/tgz/

MAINTAINER=	avl@FreeBSD.org
COMMENT=	GTK frontend for XNeur keyboard layout switcher

BUILD_DEPENDS=	xneur:${PORTSDIR}/deskutils/xneur
RUN_DEPENDS=	xneur:${PORTSDIR}/deskutils/xneur

MAKE_JOBS_SAFE=	yes

USE_BZIP2=	yes
GNU_CONFIGURE=	yes
USE_GNOME=	desktopfileutils gconf2 gtk20 libglade2
INSTALLS_ICONS=	yes

MAN1=		${PORTNAME}.1

.include <bsd.port.pre.mk>

.if !defined(WITHOUT_NLS)
USE_GETTEXT=	yes
PLIST_SUB+=	NLS=""
.else
CONFIGURE_ARGS+=	--disable-nls
PLIST_SUB+=	NLS="@comment "
.endif

post-patch:
	@${REINPLACE_CMD} -e '/all_modifiers/d' ${WRKSRC}/src/misc.c

post-install:
	@-update-desktop-database

.include <bsd.port.post.mk>
