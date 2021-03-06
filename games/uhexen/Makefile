# New ports collection makefile for:	U-Hexen
# Date created:				10 February 2003
# Whom:					Maxim Sobolev <sobomax@FreeBSD.org>
#
# $FreeBSD$
#

PORTNAME=	uhexen
PORTVERSION=	0.601
PORTREVISION=	8
CATEGORIES=	games
MASTER_SITES=	http://uhexen.sourceforge.net/
DISTNAME=	${PORTNAME}-latest

MAINTAINER=	danfe@FreeBSD.org
COMMENT=	Yet another port of Hexen, popular game from Raven Software

USE_SDL=	mixer sdl
USE_GMAKE=	yes

OPTIONS=	WAD	"Install demo-version WAD file"		on \
		MUSIC	"Enable support for in-game music"	on

.include <bsd.port.pre.mk>

.if ${ARCH} == "powerpc"
BROKEN=		Does not compile on powerpc
.endif

.if !defined(WITHOUT_WAD)
DISTFILES=	${DISTNAME}${EXTRACT_SUFX} hexen.zip
EXTRACT_ONLY=	${DISTNAME}${EXTRACT_SUFX}
EXTRACT_DEPENDS=	unzip:${PORTSDIR}/archivers/unzip
PLIST_SUB=	WAD=""
.else
PLIST_SUB=	WAD="@comment "
.endif

.if !defined(WITHOUT_MUSIC)
RUN_DEPENDS+=	${LOCALBASE}/lib/timidity/goemon.cfg:${PORTSDIR}/audio/timidity
.endif

WRKSRC=		${WRKDIR}/${PORTNAME}-${PORTVERSION}

PLIST_FILES=	bin/uhexen %%DATADIR%%/.keep_me %%WAD%%%%DATADIR%%/hexen.wad
PLIST_DIRS=	%%DATADIR%%

post-extract:
.if !defined(WITHOUT_WAD)
	@${UNZIP_CMD} -qo ${DISTDIR}/hexen.zip \
		-d ${WRKSRC}
.endif

do-install:
	${INSTALL_PROGRAM} ${WRKSRC}/uhexen ${PREFIX}/bin
	@${MKDIR} ${DATADIR}
.if !defined(WITHOUT_WAD)
	${INSTALL_DATA} ${WRKSRC}/hexen.wad ${DATADIR}
.endif
	@${TOUCH} ${DATADIR}/.keep_me

.include <bsd.port.post.mk>
