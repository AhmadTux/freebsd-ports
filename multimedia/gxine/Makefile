# New ports collection makefile for:	gxine
# Date created:	Sa 19 Apr 2003 01:34:06 CEST
# Whom: 	Grigori Goronzy <greg@chown.ath.cx>
#
# $FreeBSD$
#

PORTNAME=	gxine
PORTVERSION=	0.5.905
PORTREVISION=	6
CATEGORIES=	multimedia
MASTER_SITES=	SF/xine/${PORTNAME}/${PORTVERSION}

MAINTAINER=	makc@FreeBSD.org
COMMENT=	An alternative GUI for the Xine media player

LIB_DEPENDS=	xine.2:${PORTSDIR}/multimedia/libxine

USE_BZIP2=	yes
USE_AUTOTOOLS=	libtool
USE_GNOME=	gtk20 gnomehier
USE_GMAKE=	yes
USE_GCC=	4.2+
MAKE_ARGS+=	LDFLAGS=-shared-libgcc
CNU_CONFIGURE=	yes
CONFIGURE_ARGS+=--without-hal --with-libintl-prefix=${LOCALBASE}
INSTALLS_ICONS=	yes

MAKE_JOBS_SAFE=	yes

MANLANG=	"" de es
MAN1=		gxine.1 gxine_client.1
MANCOMPRESSED=	no

OPTIONS=	MOZILLA_PLUGIN "Install Mozilla plugin" off \
		GNOME "Enable Gnome integration" off \
		LIRC "Enable lirc support" off

.include <bsd.port.options.mk>

.if defined(WITH_GNOME)
USE_GNOME+=	gnomevfs2 libgnomeui
CONFIGURE_ARGS+=--with-dbus
.else
CONFIGURE_ARGS+=--disable-integration-wizard --without-dbus
.endif

.if defined(WITHOUT_NLS)
CONFIGURE_ARGS+=--disable-nls
PLIST_SUB+=	NLS="@comment "
.else
USE_GETTEXT=	yes
PLIST_SUB+=	NLS=""
.endif

.if defined(WITH_LIRC)
LIB_DEPENDS+=	lirc_client.2:${PORTSDIR}/comms/lirc
CONFIGURE_ARGS+=	--enable-lirc
.else
CONFIGURE_ARGS+=	--disable-lirc
.endif

.if defined(WITHOUT_MOZILLA_PLUGIN)
LIB_DEPENDS+=	js.1:${PORTSDIR}/lang/spidermonkey
CONFIGURE_ARGS+=--without-browser-plugin --with-spidermonkey
PLIST_SUB+=	MOZILLA_PLUGIN="@comment "
.else
USE_XORG+=	xaw
USE_GECKO=	libxul
USE_WEBPLUGINS=	native
WEBPLUGINS_DIR=	${PREFIX}/lib/gxine
WEBPLUGINS_FILES=gxineplugin.so
.include "${PORTSDIR}/www/firefox/Makefile.webplugins"
LIB_DEPENDS+=	nspr4.1:${PORTSDIR}/devel/nspr
CONFIGURE_ENV+=	NSPR_CFLAGS="-I${LOCALBASE}/include/nspr" \
		NSPR_LIBS="-L${LOCALBASE}/lib"
PLIST_SUB+=	MOZILLA_PLUGIN=""
.endif

pre-configure:
	@${REINPLACE_CMD} -e "/^mkdir_p/s:@mkdir_p@:${MKDIR}:" \
		${WRKSRC}/po/Makefile.in.in
	@${REINPLACE_CMD} -e "/^mkdir_p/s:@mkdir_p@:${MKDIR}:" \
		${WRKSRC}/misc/po/Makefile.in.in
	@${REINPLACE_CMD} -e "s:@MAKE_PNG_TRUE@:#:g" \
		${WRKSRC}/pixmaps/Makefile.in

.include <bsd.port.pre.mk>

.if (${OSVERSION}<800000) && defined(WITH_MOZILLA_PLUGIN)
BROKEN=	does not compile with MOZILLA_PLUGIN
.endif

.include <bsd.port.post.mk>
