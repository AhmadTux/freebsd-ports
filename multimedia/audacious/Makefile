# New ports collection makefile for:	audacious
# Date created:			2006-01-01
# Whom:				Zastupov Stepan [RedChrom] <redchrom@mail.ru>
#
# $FreeBSD$
#

PORTNAME=	audacious
PORTVERSION=	3.1.1
CATEGORIES=	multimedia audio
MASTER_SITES=	http://distfiles.atheme.org/
MASTER_SITE_SUBDIR=oliver

MAINTAINER=	oliver@FreeBSD.org
COMMENT=	A media player based on BMP and XMMS

GNU_CONFIGURE=	yes
USE_GMAKE=	yes
USE_XORG=	x11 sm
USE_GNOME=	gtk20 libglade2
USE_LDCONFIG=	yes
CPPFLAGS+=	-I${LOCALBASE}/include
LDFLAGS+=	-L${LOCALBASE}/lib
CONFIGURE_ARGS=	--disable-gtk3

MAN1=		audacious.1 audtool.1

LIB_DEPENDS=	mcs.1:${PORTSDIR}/devel/libmcs \
		mowgli.2:${PORTSDIR}/devel/libmowgli \
		xml2.5:${PORTSDIR}/textproc/libxml2

PORTDOCS=	README AUTHORS NEWS

OPTIONS=	CHARSET	"Build with automatic charset detection"	off \
		DBUS	"Build with dbus support"			on  \
		GNOME	"Build with gconf support"			off \
		ICONV	"Build with iconv support"			on \
		NLS	"Native Language Support"			on  \
		EXECINFO "Build with libexecinfo support"		on  \
		SSE2	"Build with SSE2 instruction set"		on

.include <bsd.port.pre.mk>

.if !defined(WITHOUT_DBUS)
LIB_DEPENDS+=	dbus-glib-1.2:${PORTSDIR}/devel/dbus-glib
CONFIGURE_ARGS+=--enable-dbus
PLIST_SUB+=	WITH_DBUS=""
.else
CONFIGURE_ARGS+=--disable-dbus
PLIST_SUB+=	WITH_DBUS="@comment "
.endif

.if !defined(WITH_CHARSET)
CONFIGURE_ARGS+=--disable-chardet
.endif

.if defined(WITH_EXECINFO) || exists(${LOCALBASE}/lib/libexecinfo.so)
LIB_DEPENDS+=	execinfo.1:${PORTSDIR}/devel/libexecinfo
.endif

.if defined(WITH_GNOME)
USE_GNOME+=	gconf2
CONFIGURE_ARGS+=--enable-gconf
.endif

.if defined(WITH_ICONV) || exists(${LOCALBASE}/lib/libiconv.so)
USE_ICONV=	yes
.endif

.if !defined(WITHOUT_NLS)
USE_GETTEXT=	yes
CONFIGURE_ARGS+=--enable-nls
PLIST_SUB+=	NLS=""
.else
CONFIGURE_ARGS+=--disable-nls
PLIST_SUB+=	NLS="@comment "
.endif

.if defined(WITHOUT_SSE2)
CONFIGURE_ARGS+=--disable-sse2
.endif

post-patch:
	@${REINPLACE_CMD} 's|$${libdir}/pkgconfig|$${prefix}/libdata/pkgconfig|g' \
		${WRKSRC}/Makefile
	@${REINPLACE_CMD} 's|CPPFLAGS +=\(.*\)|CPPFLAGS =\1 -I${LOCALBASE}/include|g' \
		${WRKSRC}/src/libaudgui/Makefile \
		${WRKSRC}/src/libaudtag/Makefile \
		${WRKSRC}/src/libaudcore/Makefile
	@${REINPLACE_CMD} 's|FMT_FIXED32,|FMT_FIXED32|g; \
		s|OUTPUT_PLUGIN_INIT_FOUND_DEVICES,|OUTPUT_PLUGIN_INIT_FOUND_DEVICES|g' \
		${WRKSRC}/src/audacious/plugin.h
	@${REINPLACE_CMD} 's|VALUE_NULL,|VALUE_NULL|g' \
		${WRKSRC}/src/audacious/preferences.h
	@${REINPLACE_CMD} 's|glib-2\.0/||g' \
		${WRKSRC}/src/libaudtag/util.h \
		${WRKSRC}/src/libaudtag/wma/guid.c \
		${WRKSRC}/src/libaudtag/wma/wma.c \
		${WRKSRC}/src/libaudtag/wma/wma_fmt.h
.if defined(WITH_EXECINFO) || exists(${LOCALBASE}/lib/libexecinfo.so)
	@${REINPLACE_CMD} 's|$${REGEX_LIBS}|& -L${LOCALBASE}/lib -lexecinfo|' \
		${WRKSRC}/src/audacious/Makefile
	@${REINPLACE_CMD} 's|-laudcore|-laudcore -L${LOCALBASE}/lib -lexecinfo|' \
		${WRKSRC}/audacious.pc.in
.endif

post-install:
.if !defined(NOPORTDOCS)
	@${MKDIR} ${DOCSDIR}
.for i in ${PORTDOCS}
	${INSTALL_DATA} ${WRKSRC}/${i} ${DOCSDIR}
.endfor
.endif
	@${CAT} ${PKGMESSAGE}
.if defined(WITH_GNOME)
	@${ECHO_MSG} ""
	@${ECHO_MSG} "You have installed audacious with gconf support."
	@${ECHO_MSG} "ALL PRESETS will be stored in the gconf database."
	@${ECHO_MSG} ""
.endif

.include <bsd.port.post.mk>
