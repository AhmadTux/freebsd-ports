# New ports collection makefile for:	libevent2
# Date created:				22 February 2011
# Whom:					Martin Matuska <mm@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	libevent2
DISTVERSION=	2.0.16
CATEGORIES=	devel
MASTER_SITES=	https://github.com/downloads/libevent/libevent/
DISTNAME=	${PORTNAME:S/2//}-${DISTVERSION}-stable

MAINTAINER=	mm@FreeBSD.org
COMMENT=	API for executing callback functions on events or timeouts

LICENSE=	BSD
LICENSE_FILE=	${WRKSRC}/LICENSE

FETCH_ARGS=	-pRr
USE_GNOME=	lthack
GNU_CONFIGURE=	yes
CONFIGURE_ENV=	LDFLAGS="${LDFLAGS}"
USE_LDCONFIG=	${PREFIX}/lib/event2

OPTIONS=	OPENSSL "Enable OpenSSL support" on \
		THREADS "Enable threads support" on

CONFIGURE_ARGS+=	--libdir=${PREFIX}/lib/event2

.include <bsd.port.options.mk>

.if !defined(WITH_OPENSSL)
CONFIGURE_ARGS+=	--disable-openssl
PLIST_SUB+=		OPENSSL="@comment "
.else
USE_OPENSSL=	yes
CPPFLAGS+=	-I${OPENSSLINC}
LDFLAGS+=	-L${OPENSSLLIB}
PLIST_SUB+=		OPENSSL=""
.endif

.include <bsd.port.pre.mk>

.if !defined(WITH_THREADS)
CONFIGURE_ARGS+=	--disable-thread-support
PLIST_SUB+=		THREADS="@comment "
.else
CONFIGURE_ENV+=		PTHREAD_CFLAGS="${PTHREAD_CFLAGS}" \
			PTHREAD_LIBS="${PTHREAD_LIBS}"
PLIST_SUB+=		THREADS=""
.endif

post-patch:
	@${REINPLACE_CMD} -E \
		-e "s,^pkgconfigdir =.*,pkgconfigdir = \$$(prefix)/libdata/pkgconfig,g" \
		${WRKSRC}/Makefile.in

post-install:
	@${INSTALL_SCRIPT} ${WRKSRC}/event_rpcgen.py ${PREFIX}/bin/event2_rpcgen.py

regression-test: build
	@(${SH} ${WRKSRC}/test/regress)

.include <bsd.port.post.mk>
