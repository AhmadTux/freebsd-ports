# New ports collection makefile for:	gnash
# Date created:				12 Jan 2006
# Whom:					Dmitry Marakasov <amdmi3@amdmi3.ru>
#
# $FreeBSD$
#

PORTNAME=	gnash
PORTVERSION=	0.8.9
PORTREVISION=	3
CATEGORIES=	graphics
MASTER_SITES=	GNU/${PORTNAME}/${PORTVERSION}/ \
		http://mirror.amdmi3.ru/distfiles/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	GNU Flash movie player

LIB_DEPENDS=	boost_thread.4:${PORTSDIR}/devel/boost-libs \
		jpeg.11:${PORTSDIR}/graphics/jpeg \
		curl.6:${PORTSDIR}/ftp/curl \
		gif.5:${PORTSDIR}/graphics/giflib \
		speex.1:${PORTSDIR}/audio/speex

CONFLICTS=	gnash-devel-[0-9]*

USE_BZIP2=	yes
USE_GMAKE=	yes
GNU_CONFIGURE=	yes
WANT_SDL=	yes
WANT_GNOME=	yes
USE_GCC=	4.2+	# triggers compiler error on gcc 3.4
WANT_GSTREAMER=	yes
USE_GNOME=	pkgconfig libxml2
USE_XORG=	x11 xau xdmcp xext xi xinerama ice sm xv
USE_AUTOTOOLS=	libltdl
MAKE_JOBS_SAFE=	yes
INSTALLS_ICONS=	yes

# jemalloc leads to infinite recursion on FreeBSD
# --enable-avm2 doesn't build (but consider for future)
CONFIGURE_ARGS=	--with-boost-incl="${LOCALBASE}/include" \
		--with-boost-lib="${LOCALBASE}/lib" \
		--disable-testsuite --disable-avm2 \
		--with-plugins-install=prefix \
		--sysconfdir="${WRKDIR}/etc" \
		--disable-jemalloc \
		--disable-silent-rules

GNASHVER=	${PORTVERSION}
CONFIG_FILES=	gnashpluginrc gnashrc

WRKSRC=		${WRKDIR}/${PORTNAME}-${GNASHVER}
PLIST_SUB+=	GNASHVER="${GNASHVER}"

USE_LDCONFIG=	${PREFIX}/lib/gnash

MAN1=		cygnal.1 findmicrophones.1 findwebcams.1 \
		flvdumper.1 gnash.1 gprocessor.1 \
		rtmpget.1 soldumper.1

OPTIONS=	PLUGIN		"Enable browser plugin" on \
		CYGNAL		"Enable Cygnal media server" off \
		GTK		"GUI: GTK (required for Mozilla/Firefox plugin)" on \
		KDE		"GUI: KDE 3.x (required for Konqueror 3.x plugin)" off \
		KDE4		"GUI: KDE 4.x (required for Konqueror 4.x plugin)" off \
		AGG		"Renderer: AGG" on \
		OPENGL		"Renderer: OpenGL (broken)" off \
		CAIRO		"Renderer: Cairo (experimental)" off \
		VAAPI		"Enable VAAPI support (requires FFMPEG)" off \
		FFMPEG		"Media handler: ffmpeg (+SDL sound output)" on \
		GSTREAMER	"Media handler: GStreamer" off

.include <bsd.port.options.mk>

# Cygnal option processing
.if defined(WITH_CYGNAL)
PLIST_SUB+=	CYGNAL=""
CONFIGURE_ARGS+=	--enable-cygnal
CONFIG_FILES+=	cygnalrc
.else
PLIST_SUB+=	CYGNAL="@comment "
CONFIGURE_ARGS+=	--disable-cygnal
.endif

# NLS option processing (not sure if --disable-nls actually has any effect)
.if !defined(WITHOUT_NLS)
USE_GETTEXT=	yes
PLIST_SUB+=	NLS=""
.else
CONFIGURE_ARGS+=	--disable-nls
PLIST_SUB+=	NLS="@comment "
.endif

#
# Plugin configuration
#
.if !defined(WITHOUT_GTK) && !defined(WITHOUT_PLUGIN)
USE_WEBPLUGINS=		native
WEBPLUGINS_FILES=	libgnashplugin.so
.include "${PORTSDIR}/www/firefox/Makefile.webplugins"
PLIST_SUB+=	NPAPI=""
CONFIGURE_ARGS+=--with-npapi-plugindir="${WEBPLUGINS_DIR}"
BUILD_DEPENDS+=	${LOCALBASE}/lib/libxul/xpidl:${PORTSDIR}/www/libxul
RUN_DEPENDS+=	${LOCALBASE}/lib/libxul/xpidl:${PORTSDIR}/www/libxul
.else
PLIST_SUB+=	NPAPI="@comment "
CONFIGURE_ARGS+=	--disable-npapi
.endif

.if defined(WITH_KDE) && !defined(WITHOUT_PLUGIN)
PLIST_SUB+=	KPARTS=""
USE_LDCONFIG+=	${PREFIX}/lib/kde3
.else
PLIST_SUB+=	KPARTS="@comment "
CONFIGURE_ARGS+=	--disable-kparts3
.endif

.if defined(WITH_KDE4) && !defined(WITHOUT_PLUGIN)
PLIST_SUB+=	KPARTS4=""
USE_LDCONFIG+=	${KDE4_PREFIX}/lib/kde4
CONFIGURE_ARGS+=	--with-kde4_prefix=${PREFIX}/kde4
.else
PLIST_SUB+=	KPARTS4="@comment "
CONFIGURE_ARGS+=	--disable-kparts4
.endif

#
# GUI options processing
#
GNASH_GUIS=

.if !defined(WITHOUT_GTK)
USE_GNOME=	gtk20
GNASH_GUIS+=	gtk
PLIST_SUB+=	GTK=""
MAN1+=		gnash-gtk-launcher.1 gtk-gnash.1
.else
PLIST_SUB+=	GTK="@comment "
.endif

.if defined(WITH_KDE)
GNASH_GUIS+=	kde3
PLIST_SUB+=	KDE=""
USE_KDELIBS_VER=3
.else
PLIST_SUB+=	KDE="@comment "
.endif

.if defined(WITH_KDE4)
GNASH_GUIS+=	kde4
PLIST_SUB+=	KDE4=""
USE_QT_VER=	4
QT_COMPONENTS=	moc_build
QT_NONSTANDARD=	yes
USE_KDE4=	kdehier kdelibs
CONFIGURE_ENV+=	KDE4_CONFIG="${KDE4_PREFIX}/bin/kde4-config"
.	if exists(${LOCALBASE}/lib/libkdeui.so)
IGNORE=		KDE4 GUI can't be built when KDE3 is installed. Please rerun 'make config' and disable KDE4 GUI or deinstall kdelibs-3
.	endif
MAN1+=		gnash-qt-launcher.1 kde4-gnash.1
.else
PLIST_SUB+=	KDE4="@comment "
.endif

.if defined(WITH_KDE) && defined defined(WITH_KDE4)
IGNORE=		KDE and KDE4 GUIs are mutually exclusive. Please rerun 'make config' and disable one of these
.endif

.if ${GNASH_GUIS} == ""
IGNORE=		needs at least one GUI enabled. Please rerun 'make config' and enable GTK, KDE or KDE4
.endif

CONFIGURE_ARGS+=--enable-gui=`${ECHO} ${GNASH_GUIS} | ${TR} ' ' ,`

#
# Renderer options processing
#
GNASH_RENDERERS=

.if defined(WITH_AGG)
LIB_DEPENDS+=		agg.2:${PORTSDIR}/graphics/agg
GNASH_RENDERERS+=	agg
.endif

.if defined(WITH_OPENGL)
USE_GL=			yes
GNASH_RENDERERS+=	ogl
.	if !defined(WITHOUT_GTK)
LIB_DEPENDS+=		gtkglext-x11-1.0.0:${PORTSDIR}/x11-toolkits/gtkglext
.	endif
.endif

.if defined(WITH_CAIRO)
.	if defined(WITH_KDE) || defined(WITH_KDE4)
IGNORE=			doesn't support Cairo renderer with KDE/KDE4 gui. Please rerun 'make config' and select different renderer or disable KDE/KDE4 GUIs
.	endif
LIB_DEPENDS+=		cairo.2:${PORTSDIR}/graphics/cairo
GNASH_RENDERERS+=	cairo
.endif

.if ${GNASH_RENDERERS} == ""
IGNORE=			needs at least one renderer enabled. Please rerun 'make config' and enable AGG, OPENGL or CAIRO
.endif

CONFIGURE_ARGS+=	--enable-renderer=`${ECHO} ${GNASH_RENDERERS} | ${TR} ' ' ,`

#
# Hardware acceleration options processing
#
.if defined(WITH_VAAPI)
.	if !defined(WITH_FFMPEG)
IGNORE=			VAAPI currently works only with FFMPEG. Please rerun 'make config' and enable FFMPEG.
.	endif
LIB_DEPENDS+=		va.1:${PORTSDIR}/multimedia/libva
PLIST_SUB+=		VAAPI=""
.else
CONFIGURE_ARGS+=	--enable-hwaccel=none # XVideo
PLIST_SUB+=		VAAPI="@comment "
.endif

#
# Media handler options processing
#
.if defined(WITH_FFMPEG) && !defined(WITH_GSTREAMER)
LIB_DEPENDS+=		avcodec.1:${PORTSDIR}/multimedia/ffmpeg
CONFIGURE_ARGS+=	--enable-media=ffmpeg
USE_SDL+=		sdl
.elif !defined(WITH_FFMPEG) && defined(WITH_GSTREAMER)
USE_GSTREAMER=		yes
CONFIGURE_ARGS+=	--enable-media=gst
USE_SDL+=		sdl
.elif !defined(WITH_FFMPEG) && !defined(WITH_GSTREAMER)
CONFIGURE_ARGS+=	--enable-media=none
.else
IGNORE=			can't be built with multiple media handlers enabled. Please rerun 'make config' and leave one or none of them (ffmpeg|gstreamer)
.endif

.if defined(WITH_GSTREAMER)
PLIST_SUB+=		GSTREAMER=""
.else
PLIST_SUB+=		GSTREAMER="@comment "
.endif

.include <bsd.port.pre.mk>

pre-everything::
.if defined(WITH_OPENGL)
	@${ECHO_CMD} "OpenGL support is currently broken (Gnash hangs using 100% CPU). Use at your own risk"
	@sleep 3
.endif

post-patch:
	@${REINPLACE_CMD} -e '/^PTHREAD_[LC][IF]*=/ d; s|-lpthread|${PTHREAD_LIBS}|' \
		${WRKSRC}/configure
	@${REINPLACE_CMD} -e 's|/usr/local|${LOCALBASE}|g; \
		s|/lib64|/lib|g; \
		s|/usr/X11R6|${LOCALBASE}|g' ${WRKSRC}/configure \
		${WRKSRC}/macros/libslist ${WRKSRC}/macros/incllist
	@${REINPLACE_CMD} -e '/KDE4_APPSDATADIR=/ s|share/kde4|share|' \
		${WRKSRC}/configure
	@${REINPLACE_CMD} -e '/^pkgconfigdir =/ s|libdir)|exec_prefix)/libdata|' \
		${WRKSRC}/Makefile.in
	@${REINPLACE_CMD} -e '/^man1dir =/ s|=.*|= ${MAN1PREFIX}/man/man1|' \
		${WRKSRC}/doc/C/Makefile.in

post-install:
.for conf in ${CONFIG_FILES}
	@${INSTALL_DATA} ${WRKDIR}/etc/${conf} ${PREFIX}/etc/${conf}.dist
	if [ ! -f ${PREFIX}/etc/${conf} ]; then \
		${INSTALL_DATA} ${PREFIX}/etc/${conf}.dist \
			${PREFIX}/etc/${conf}; \
	fi
.endfor

.if !defined(WITHOUT_PLUGIN)
.if !defined(WITHOUT_GTK)
	${MKDIR} ${WEBPLUGINS_DIR}
	@cd ${INSTALL_WRKSRC}/plugin && \
		${SETENV} ${MAKE_ENV} ${GMAKE} ${MAKE_ARGS} install-plugin
.endif
.if defined(WITH_KDE)
	@cd ${INSTALL_WRKSRC}/plugin/klash && \
		${SETENV} ${MAKE_ENV} ${GMAKE} ${MAKE_ARGS} install-plugin
.endif
.if defined(WITH_KDE4)
	@cd ${INSTALL_WRKSRC}/plugin/klash4 && \
		${SETENV} ${MAKE_ENV} ${GMAKE} ${MAKE_ARGS} install-plugin
.endif
.endif

.include <bsd.port.post.mk>
