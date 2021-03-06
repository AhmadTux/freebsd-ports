# New ports collection makefile for:	online-desktop
# Date created:		2008-05-28
# Whom:			Ashish Shukla <wahjava@gmail.com>
#
# $FreeBSD$
#

PORTNAME=	online-desktop
PORTVERSION=	0.3.2
PORTREVISION=	6
CATEGORIES=	net gnome
MASTER_SITES=	GNOME

MAINTAINER=	ashish@FreeBSD.org
COMMENT=	GNOME Online Desktop

BUILD_DEPENDS=	${LOCALBASE}/libdata/pkgconfig/dbus-1.pc:${PORTSDIR}/devel/dbus \
		${LOCALBASE}/libdata/pkgconfig/dbus-glib-1.pc:${PORTSDIR}/devel/dbus-glib \
		${LOCALBASE}/libdata/pkgconfig/nspr.pc:${PORTSDIR}/devel/nspr \
		${LOCALBASE}/libdata/pkgconfig/nss.pc:${PORTSDIR}/security/nss \
		${LOCALBASE}/libdata/pkgconfig/purple.pc:${PORTSDIR}/net-im/libpurple \
		${LOCALBASE}/libdata/pkgconfig/pidgin.pc:${PORTSDIR}/net-im/pidgin
LIB_DEPENDS=	dbus-1.3:${PORTSDIR}/devel/dbus \
		dbus-glib-1.2:${PORTSDIR}/devel/dbus-glib \
		nspr4.1:${PORTSDIR}/devel/nspr \
		nss3.1:${PORTSDIR}/security/nss \
		purple.10:${PORTSDIR}/net-im/libpurple
RUN_DEPENDS=	${LOCALBASE}/libexec/desktop-data-engine:${PORTSDIR}/net/desktop-data-model \
		${LOCALBASE}/bin/bigboard:${PORTSDIR}/misc/bigboard \
		${LOCALBASE}/bin/purple-remote:${PORTSDIR}/net-im/libpurple \
		${LOCALBASE}/bin/pidgin:${PORTSDIR}/net-im/pidgin

USE_BZIP2=	yes
USE_GNOME=	glib20 gconf2 pygnomedesktop
USE_GMAKE=	yes
USE_GETTEXT=	yes
GNU_CONFIGURE=	yes
USE_PYTHON=	2.5+
GCONF_SCHEMAS=	online-desktop.schemas
INSTALLS_ICONS=	yes
USE_LDCONFIG=	yes

post-patch:
	@${FIND} ${WRKSRC} -type f -exec fgrep -l '%%PREFIX%%' {} \; |${XARGS} ${REINPLACE_CMD} -e 's,%%PREFIX%%,${PREFIX},g' -i .bak
	@${FIND} ${WRKSRC} -type f -exec fgrep -l '%%LOCALBASE%%' {} \; |${XARGS} ${REINPLACE_CMD} -e 's,%%LOCALBASE%%,${LOCALBASE},g' -i .bak

.include <bsd.port.mk>
