# New ports collection makefile for:	kazehakase
# Date created:		2004-04-06
# Whom:			michael johnson <ahze@ahze.net>
#
# $FreeBSD$
#   $MCom: ports/www/kazehakase/Makefile,v 1.26 2009/10/22 22:10:49 kwm Exp $
#

PORTNAME=	kazehakase
PORTVERSION=	0.5.8
PORTREVISION=	6
CATEGORIES=	www
MASTER_SITES=	${MASTER_SITE_SOURCEFORGE_JP}
MASTER_SITE_SUBDIR=	${PORTNAME}/43802

MAINTAINER=	kwm@FreeBSD.org
COMMENT=	Kazehakase is a browser with gecko engine like Epiphany or Galeon

LIB_DEPENDS=	gnutls.47:${PORTSDIR}/security/gnutls \
		estraier.8:${PORTSDIR}/textproc/hyperestraier

BROKEN=		does not link

USE_GNOME=	gnomehack intlhack gnomeprefix gtk20
USE_GMAKE=	yes
USE_GETTEXT=	yes
USE_GECKO=	libxul
USE_LDCONFIG=	yes
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-gecko_engine=${GECKO}
CONFIGURE_ENV=	LIBS="-L${LOCALBASE}/lib ${PTHREAD_LIBS}"
CPPFLAGS+=	-I${LOCALBASE}/include ${PTHREAD_CFLAGS} \
		-I${LOCALBASE}/include/nspr
MAN1=		kazehakase.1
OPTIONS=	WEBKIT "Webkit Backend (along with Gecko)" On

.if defined(WITH_MIGEMO)
USE_RUBY=	yes
BUILD_DEPENDS+=	${RUBY_SITEARCHLIBDIR}/uconv.so:${PORTSDIR}/japanese/ruby-uconv
RUN_DEPENDS+=	${RUBY_SITEARCHLIBDIR}/uconv.so:${PORTSDIR}/japanese/ruby-uconv
.endif

.include <bsd.port.pre.mk>

.if ${OSVERSION} < 700000
BROKEN=		does not build on 6.x
.endif

.if defined(WITH_WEBKIT)
LIB_DEPENDS+=	webkitgtk-1.0.0:${PORTSDIR}/www/webkit-gtk2
PLIST_SUB+=	WEBKIT=""
.else
PLIST_SUB+=	WEBKIT="@comment "
.endif

.if defined(WITH_THUMBNAIL) || exists(${LOCALBASE}/bin/gs)
USE_GHOSTSCRIPT=yes
.endif

.if defined(WITH_MIGEMO)
.if ${ARCH} == "i386" || ${ARCH} == "sparc64"
RUN_DEPENDS+=	migemo:${PORTSDIR}/japanese/migemo

CONFIGURE_ARGS+=	--enable-migemo
.endif
.endif

pre-everything::
	@${ECHO_MSG} ""
	@${ECHO_MSG} "Enable Migemo (Japanese incremental search) by defining WITH_MIGEMO"
	@${ECHO_MSG} ""
	@${ECHO_MSG} "Enable Thumbnail support by defining WITH_THUMBNAIL"
	@${ECHO_MSG} ""

post-patch:
	@${REINPLACE_CMD} -e 's|/usr/share/migemo|${LOCALBASE}/share/migemo|; \
		s|=beginner|=medium|' ${WRKSRC}/etc/kzrc
.if defined(WITH_MIGEMO)
	@${REINPLACE_CMD} -e 's|migemo=fals|migemo=tru|' ${WRKSRC}/etc/kzrc
.endif
.if defined(WITH_THUMBNAIL)
	@${ECHO_MSG} "create_thumbnail=true" >> ${WRKSRC}/etc/kzrc
.endif
	@${FIND} ${WRKSRC} -name Makefile.in | ${XARGS} ${REINPLACE_CMD} -e \
		's|-D.*_DISABLE_DEPRECATED=1||g'
	@${REINPLACE_CMD} -e 's|-D.*_DISABLE_DEPRECATED||g' \
		${WRKSRC}/configure
	@${REINPLACE_CMD} -e 's|-embedding||g' \
		${WRKSRC}/configure
	@${REINPLACE_CMD} -e 's|-unstable||g' \
		${WRKSRC}/configure

pre-install:
	@${REINPLACE_CMD} -e 's|localen|locale|' \
		${WRKSRC}/po/Makefile

.include <bsd.port.post.mk>
