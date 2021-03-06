# New ports collection makefile for:	nemiver
# Date created:		2008-02-22
# Whom:			Romain Tartiere <romain@blogreen.org>
#
# $FreeBSD$
#

PORTNAME=	nemiver
PORTVERSION=	0.8.2
PORTREVISION=	2
CATEGORIES=	devel
MASTER_SITES=	${MASTER_SITE_GNOME}
MASTER_SITE_SUBDIR=	sources/${PORTNAME}/${PORTVERSION:C/^([0-9]+\.[0-9]+).*/\1/}
DIST_SUBDIR=	gnome2

MAINTAINER=	romain@FreeBSD.org
COMMENT=	A C/C++ Debugger for GNOME

LIB_DEPENDS=	boost_unit_test_framework:${PORTSDIR}/devel/boost-libs \
		gtop-2.0:${PORTSDIR}/devel/libgtop \
		sqlite3:${PORTSDIR}/databases/sqlite3 \
		glademm-2.4:${PORTSDIR}/devel/libglademm24 \
		gtksourceviewmm-2:${PORTSDIR}/x11-toolkits/libgtksourceviewmm
BUILD_DEPENDS=	gdb74:${PORTSDIR}/devel/gdb
RUN_DEPENDS=	gdb74:${PORTSDIR}/devel/gdb

OPTIONS=	GHEX "Enable memory view" off

GNU_CONFIGURE=	yes
CONFIGURE_TARGET=--build=${MACHINE_ARCH}-portbld-freebsd${OSREL}
USE_GMAKE=	yes
USE_GNOME=	gnomeprefix gnomehack intltool gnomedocutils gconf2 vte
USE_GCC=	4.4+
USE_LDCONFIG=	yes
USE_GETTEXT=	yes
INSTALLS_OMF=	yes
INSTALLS_ICONS=	yes
MAN1=		nemiver.1

.include <bsd.port.pre.mk>

.if !exists(${LOCALBASE}/libdata/pkgconfig/gsettings-desktop-schemas.pc)
CONFIGURE_ARGS+=--enable-gsettings=no
PLIST_SUB+=	GSETTINGS="@comment " GCONF=""
GCONF_SCHEMAS=	nemiver-workbench.schemas nemiver-dbgperspective.schemas
.else
# Technically, the port is already installed in this case so fill-in
# BUILD_DEPENDS for the sake of consistency only.
BUILD_DEPENDS+=	${LOCALBASE}/libdata/pkgconfig/gsettings-desktop-schemas.pc:${PORTSDIR}/devel/gsettings-desktop-schemas
RUN_DEPENDS+=	${LOCALBASE}/libdata/pkgconfig/gsettings-desktop-schemas.pc:${PORTSDIR}/devel/gsettings-desktop-schemas
CONFIGURE_ARGS+=--enable-gsettings=yes
PLIST_SUB+=	GSETTINGS="" GCONF="@comment "
GLIB_SCHEMAS=	org.nemiver.gschema.xml
.endif

.if defined(WITH_GHEX) && !defined(WITHOUT_GHEX)
CONFIGURE_ARGS+=--enable-memoryview
PLIST_SUB+=	GHEX=""
LIB_DEPENDS+=	gtkhex.0:${PORTSDIR}/editors/ghex
.else
CONFIGURE_ARGS+=--disable-memoryview
PLIST_SUB+=	GHEX="@comment "
.endif

.include <bsd.port.post.mk>
