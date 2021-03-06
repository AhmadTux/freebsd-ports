# New ports collection makefile for:	xfce4-utils
# Date created:		7 June 2003
# Whom:			James Earl <jdearl@telus.net>
#
# $FreeBSD$
#

PORTNAME=	xfce4-utils
PORTVERSION=	4.8.3
PORTREVISION=	2
CATEGORIES=	sysutils xfce
MASTER_SITES=	${MASTER_SITE_XFCE}
MASTER_SITE_SUBDIR=	src/xfce/xfce-utils/${PORTVERSION:R}
DISTNAME=	xfce-utils-${PORTVERSION}
DIST_SUBDIR=	xfce4

MAINTAINER=	xfce@FreeBSD.org
COMMENT=	Xfce 4 essential utilities and scripts

BUILD_DEPENDS=	p5-XML-Parser>=2.40:${PORTSDIR}/textproc/p5-XML-Parser

DOCSDIR=	${PREFIX}/share/doc/${PORTNAME:S/4//}

CONFIGURE_ARGS+=--with-xsession-prefix=${PREFIX} \
	--with-vendor-info=FreeBSD
CONFIGURE_ENV+=	ac_cv_prog_AWK="${AWK}"

USE_AUTOTOOLS=	libtool
GNU_CONFIGURE=	yes
INSTALLS_ICONS=	yes
USE_BZIP2=	yes
USE_GMAKE=	yes
USE_PERL5=	yes
USE_GNOME=	gnomehack gtk20 intltool intlhack desktopfileutils pkgconfig
USE_XFCE=	configenv libmenu libutil xfconf
USE_XORG=	x11

OPTIONS=	DBUS	"Enable D-BUS support"			on \
		NLS "Enable Native Language Support" on \
		XLOCK "Use xlock for 'lock screen'" on \
		XSCREENSAVER "Use xscreensaver for 'lock screen'" off

.include <bsd.port.options.mk>

.if !defined(WITHOUT_DBUS)
LIB_DEPENDS+=	dbus-glib-1.2:${PORTSDIR}/devel/dbus-glib
CONFIGURE_ARGS+=--enable-dbus
PLIST_SUB+=	WITH_DBUS=""
.else
CONFIGURE_ARGS+=--disable-dbus
PLIST_SUB+=	WITH_DBUS="@comment "
.endif

.if !defined(WITHOUT_NLS)
USE_GETTEXT=	yes
CONFIGURE_ARGS+=--enable-nls
PLIST_SUB+=	NLS=""
.else
CONFIGURE_ARGS+=--disable-nls
PLIST_SUB+=	NLS="@comment "
.endif

.if !defined(WITHOUT_XLOCK)
RUN_DEPENDS+=	xlock:${PORTSDIR}/x11/xlockmore
.endif

.if !defined(WITHOUT_XSCREENSAVER)
RUN_DEPENDS+=	xscreensaver:${PORTSDIR}/x11/xscreensaver
.endif

.if defined(WITH_XLOCK) && defined(WITH_XSCREENSAVER)
IGNORE=	should only have one locking option defined
.endif

post-install:
	@-update-desktop-database

.include <bsd.port.mk>
