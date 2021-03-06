# New ports collection makefile for:	atunes
# Date Created:		29 Dec 2007
# Whom:			Lars Engels <lme@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	atunes
PORTVERSION=	2.0.1
PORTREVISION?=	1
CATEGORIES=	audio java
MASTER_SITES=	SF/${PORTNAME}/${PORTNAME}/aTunes%20${PORTVERSION}

MAINTAINER=	lme@FreeBSD.org
COMMENT=	A full-featured audio player and manager developed in Java

NO_BUILD=	yes
USE_JAVA=	yes
JAVA_VERSION=	1.6+
SUB_FILES=	atunes

WRKSRC=		${WRKDIR}/${PORTNAME}-${PORTVERSION}-SNAPSHOT

OPTIONS=	MPLAYER "Engine:  Use Mplayer" on \
		XINE "Engine:  Use Xine" off \
		CDDA2WAV "Ripper:  Enable cdrtools support" on \
		LAME "Encoder: Enable lame MP3 encoder" on \
		VORBIS "Encoder: Enable vorbis OGG encoder" on \
		FLAC "Encoder: Enable FLAC encoder" off

.include <bsd.port.pre.mk>

pre-everything::
.if !defined(WITH_MPLAYER) && !defined(WITH_XINE)
	@${ECHO_MSG} "You need to select at least one engine, mplayer or xine!"
	@${FALSE}
.endif

.if defined(WITH_MPLAYER)
RUN_DEPENDS+=	mplayer:${PORTSDIR}/multimedia/mplayer
.endif

.if defined(WITH_XINE)
LIB_DEPENDS+=	xine.2:${PORTSDIR}/multimedia/libxine
.endif

.if defined(WITH_CDDA2WAV)
USE_CDRTOOLS=	yes
.endif

.if defined(WITH_LAME)
RUN_DEPENDS+=	lame:${PORTSDIR}/audio/lame
.endif

.if defined(WITH_VORBIS)
RUN_DEPENDS+=	ogg123:${PORTSDIR}/audio/vorbis-tools
.endif

.if defined(WITH_FLAC)
RUN_DEPENDS+=	flac:${PORTSDIR}/audio/flac
.endif

DESKTOP_ENTRIES="aTunes" \
		"Audio player and manager" \
		"${DATADIR}/aTunes.ico" \
		"aTunes" \
		"Application;;AudioVideo;Java;" \
		false

INSTALL_FILES=	aTunes.ico aTunes.jar license.txt

PIXMAPS=	atunes.png

do-install:
	@${MKDIR} ${DATADIR}
	@${MKDIR} ${DATADIR}/lib
	${INSTALL_SCRIPT} ${WRKDIR}/atunes ${PREFIX}/bin
.for file in ${INSTALL_FILES}
	@${INSTALL_DATA} ${WRKSRC}/${file} ${DATADIR}/${file}
.endfor
	cd ${WRKSRC} && ${FIND} lib -type f -name "*.jar" -exec ${INSTALL_DATA} {} \
		${DATADIR}/{} \;
	@${MKDIR} ${PREFIX}/share/pixmaps
.for file in ${PIXMAPS}
	@${INSTALL_DATA} ${WRKSRC}/${file} ${PREFIX}/share/pixmaps
.endfor

.include <bsd.port.post.mk>
