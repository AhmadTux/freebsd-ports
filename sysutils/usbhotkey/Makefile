# New ports collection makefile for:   usbhotkey
# Date created:        16 July 2007
# Whom:                ntarmos@ceid.upatras.gr
#
# $FreeBSD$
#

PORTNAME=		usbhotkey
PORTVERSION=		0.3.1
PORTREVISION=		5
CATEGORIES=		sysutils
MASTER_SITES=		SF/${PORTNAME}/${PORTNAME}/${PORTNAME}-${PORTVERSION}

MAINTAINER=		ntarmos@cs.uoi.gr
COMMENT=		A utility to remap USB keyboards under X11

LIB_DEPENDS=		hid.0:${PORTSDIR}/devel/libhid \
			Xtst.6:${PORTSDIR}/x11/libXtst

OPTIONS=		GUI "Enable GUI bits (Imlib dependency)" on

USE_RUBY=		yes
USE_GMAKE=		yes
HAS_CONFIGURE=		yes

.include <bsd.port.pre.mk>

.if !defined(WITHOUT_PTHREADS)
CPPFLAGS+=	${PTHREAD_CFLAGS} -I${LOCALBASE}/include \
		-I${LOCALBASE}/include/ruby-${RUBY_VER} \
		-I${LOCALBASE}/include/ruby-${RUBY_VER}/${RUBY_ARCH}
LDFLAGS+=	${PTHREAD_LIBS} -L${LOCALBASE}/lib
.else
CPPFLAGS+=	-I${LOCALBASE}/include \
		-I${LOCALBASE}/include/ruby-${RUBY_VER} \
		-I${LOCALBASE}/include/ruby-${RUBY_VER}/${RUBY_ARCH}
LDFLAGS+=	-L${LOCALBASE}/lib
.endif

.if !defined(WITHOUT_GUI)
LIB_DEPENDS+=		Imlib.5:${PORTSDIR}/graphics/imlib
.else
CONFIGURE_ARGS+=	--without-Imlib
.endif

.if ${RUBY_VER} == 1.9
EXTRA_PATCHES+=		${PATCHDIR}/extra-patch-main.c
.endif

.include <bsd.port.post.mk>
