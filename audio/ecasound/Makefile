# New ports collection makefile for:	ecasound
# Date created:		22 dec 2000
# Whom:			The Anarcat <anarcat@anarcat.dyndns.org>
#
# $FreeBSD$
#

PORTNAME=	ecasound
PORTVERSION=	2.8.1
CATEGORIES=	audio
MASTER_SITES=	http://ecasound.seul.org/download/

MAINTAINER=	ports@FreeBSD.org
COMMENT=	Multitrack audio processing software

BUILD_DEPENDS=	${LOCALBASE}/include/ladspa.h:${PORTSDIR}/audio/ladspa

OPTIONS=	ARTS		"Enable aRts support"		off \
		AUDIOFILE	"Enable libaudiofile support"	off \
		JACK		"Enable JACK support"		off \
		LO		"Enable liblo support"		off \
		OIL		"Enable liboil support"		off \
		SAMPLERATE	"Enable libsamplerate support"	off

LICENSE=	GPLv2 GPLv3
LICENSE_COMB=	dual

USE_GMAKE=	yes
USE_AUTOTOOLS=	libtool
CONFIGURE_ARGS=	--disable-pyecasound \
		--disable-rubyecasound \
		--enable-sys-readline
USE_LDCONFIG=	yes

MAN1=		ecaconvert.1 ecafixdc.1 ecalength.1 ecamonitor.1 \
		ecanormalize.1 ecaplay.1 ecasignalview.1 ecasound.1 \
		ecasound-iam.1 ecatools.1
MAN5=		ecasoundrc.5

CPPFLAGS+=	-I${LOCALBASE}/include ${PTHREAD_CFLAGS}
LDFLAGS+=	-L${LOCALBASE}/lib ${PTHREAD_LIBS}

.include <bsd.port.pre.mk>

.if exists(${LOCALBASE}/lib/libreadline.so.6)
LIB_DEPENDS+=	readline.6:${PORTSDIR}/devel/readline
.endif

.if defined(WITH_ARTS)
LIB_DEPENDS+=	artsc.0:${PORTSDIR}/audio/arts
.else
CONFIGURE_ARGS+=	--disable-arts
.endif

.if defined(WITH_AUDIOFILE)
LIB_DEPENDS+=	audiofile.0:${PORTSDIR}/audio/libaudiofile
.else
CONFIGURE_ARGS+=	--disable-audiofile
.endif

.if defined(WITH_JACK)
LIB_DEPENDS+=	jack.0:${PORTSDIR}/audio/jack
.else
CONFIGURE_ARGS+=	--disable-jack
.endif

.if defined(WITH_LO)
LIB_DEPENDS+=	lo.7:${PORTSDIR}/audio/liblo
.else
CONFIGURE_ARGS+=	--disable-liblo
.endif

.if defined(WITH_OIL)
LIB_DEPENDS+=	oil-0.3.0:${PORTSDIR}/devel/liboil
.else
CONFIGURE_ARGS+=	--disable-liboil
.endif

.if defined(WITH_SAMPLERATE)
LIB_DEPENDS+=	samplerate.1:${PORTSDIR}/audio/libsamplerate
.else
CONFIGURE_ARGS+=	--disable-libsamplerate --disable-sndfile
.endif

post-patch:
	@${REINPLACE_CMD} -e '/test/s|==|=|g ; \
		 s| $$EXTRAGCCFLAGS||g ; \
		 s| $$EXTRACPPFLAGS||g ; \
		 s| -pthread||g ; \
		 s| -D_THREAD_SAFE||g ; \
		 s| -D_P1003_1B_VISIBLE||' ${WRKSRC}/configure

.include <bsd.port.post.mk>
