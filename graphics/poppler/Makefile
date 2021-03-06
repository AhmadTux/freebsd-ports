# New ports collection makefile for:	poppler
# Date created:         03 March 2005
# Whom:                 marcus@FreeBSD.org
#
# $FreeBSD$
#   $MCom: ports/graphics/poppler/Makefile,v 1.74 2011/08/05 14:05:23 kwm Exp $
#

PORTNAME=	poppler
PORTVERSION=	0.18.4
PORTREVISION?=	0
CATEGORIES=	graphics print
MASTER_SITES=	http://poppler.freedesktop.org/

MAINTAINER?=	gnome@FreeBSD.org
COMMENT?=	A PDF rendering library

LICENSE=	GPLv2

LIB_DEPENDS=	jpeg.11:${PORTSDIR}/graphics/jpeg \
		fontconfig.1:${PORTSDIR}/x11-fonts/fontconfig \
		lcms.1:${PORTSDIR}/graphics/lcms
RUN_DEPENDS=	poppler-data>0:${PORTSDIR}/graphics/poppler-data

USE_GMAKE=	yes
USE_GNOME=	gnomehack libxml2 ltverhack
USE_AUTOTOOLS=	libtool
MAKE_JOBS_UNSAFE=	yes
USE_LDCONFIG=	yes
CONFIGURE_ARGS=	--enable-zlib \
		--enable-xpdf-headers \
		--disable-gtk-test \
		--with-html-dir=${DOCSDIR}
CPPFLAGS+=	-I${LOCALBASE}/include ${PTHREAD_CFLAGS}
LDFLAGS+=	-L${LOCALBASE}/lib ${PTHREAD_LIBS}

.if !defined(SLAVEPORT)
OPTIONS=	CAIRO "Enable cairo output backend" on \
		OPENJPEG "Enable JPEG 2000 support" on
.endif

.include <bsd.port.options.mk>

.if defined(SLAVEPORT)
LIB_DEPENDS+=	poppler.18:${PORTSDIR}/graphics/poppler

.if ${SLAVEPORT}=="glib"
BUILD_DEPENDS+=	g-ir-scanner:${PORTSDIR}/devel/gobject-introspection
LIB_DEPENDS+=	cairo.2:${PORTSDIR}/graphics/cairo
CONFIGURE_ARGS+=--enable-poppler-glib
USE_GNOME+=	gnomehier glib20
BUILD_WRKSRC=	${WRKSRC}/glib
INSTALL_WRKSRC=	${WRKSRC}/glib
.else
CONFIGURE_ARGS+=--disable-poppler-glib
.endif

.if ${SLAVEPORT}=="qt4"
CONFIGURE_ARGS+=--enable-poppler-qt4
CONFIGURE_ENV+=	ac_cv_prog_MOCQT4=${LOCALBASE}/bin/moc-qt4
USE_QT_VER+=	4
QT_COMPONENTS=	gui corelib xml qtestlib moc_build
# It requires poppler/splash to be built unless you disable splash backend
# support to allow you use BUILD_WRKSRC here. It seems that splash backend
# can read more PDF files than without, tested with demos's poppler_qt4viewer.
#BUILD_WRKSRC=	${WRKSRC}/qt4
INSTALL_WRKSRC=	${WRKSRC}/qt4
.else
CONFIGURE_ARGS+=--disable-poppler-qt4
.endif

.if ${SLAVEPORT}=="utils"
LIB_DEPENDS+=	poppler-glib.8:${PORTSDIR}/graphics/poppler-glib
CONFIGURE_ARGS+=--enable-utils
CONFLICTS=	xpdf-[0-9]*
BUILD_WRKSRC=	${WRKSRC}/utils
INSTALL_WRKSRC=	${WRKSRC}/utils
MAN1=		pdffonts.1 pdfimages.1 pdfinfo.1 pdfseparate.1 pdftocairo.1 \
		pdftohtml.1  pdftops.1 pdftotext.1 pdftoppm.1 pdfunite.1
.else
CONFIGURE_ARGS+=--disable-utils
.endif

.else
CONFIGURE_ARGS+=--disable-poppler-glib \
		--disable-poppler-qt4 \
		--disable-utils

.if defined(WITHOUT_CAIRO)
CONFIGURE_ARGS+=--disable-cairo-output
PLIST_SUB+=	CAIRO="@comment "
.else
LIB_DEPENDS+=	cairo.2:${PORTSDIR}/graphics/cairo
PLIST_SUB+=	CAIRO=""
.endif

.if defined(WITHOUT_OPENJPEG)
CONFIGURE_ARGS+=--disable-libopenjpeg
.else
CONFIGURE_ARGS+=--enable-libopenjpeg
LIB_DEPENDS+=	openjpeg.2:${PORTSDIR}/graphics/openjpeg
.endif
.endif

post-patch:
	@${REINPLACE_CMD} -e 's|-version-info 19:0:0|-version-info 18:0:0|g' \
		${WRKSRC}/poppler/Makefile.in
.if defined(SLAVEPORT) && ${SLAVEPORT}=="qt4"
	@${REINPLACE_CMD} -e 's|MOCQT4=`.*|MOCQT4="${LOCALBASE}/bin/moc-qt4"|' \
		${WRKSRC}/configure
.endif

post-install:
.if defined(SLAVEPORT)
.if ${SLAVEPORT}=="glib"
	${INSTALL_DATA} ${WRKSRC}/poppler-glib.pc ${PREFIX}/libdata/pkgconfig
.elif ${SLAVEPORT}=="qt4"
	${INSTALL_DATA} ${WRKSRC}/poppler-qt4.pc ${PREFIX}/libdata/pkgconfig
.endif
.endif

.include <bsd.port.mk>
